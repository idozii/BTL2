#include "study_in_pink2.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////
//TODO: 3.1: MAP ELEMENT
MapElement::MapElement(ElementType in_type){
    this->type = type;
};

ElementType MapElement::getType() const {
    return this->type;
};

Path::Path() : MapElement(PATH){
    this->type = PATH;
};

Wall::Wall() : MapElement(WALL){
    this->type = WALL;
};

FakeWall::FakeWall(int in_req_exp) : MapElement(FAKE_WALL){
    this->type = FAKE_WALL;
    req_exp = in_req_exp;
};

int FakeWall::getReqExp() const {
    return req_exp;
};

//TODO: 3.2: MAP
Map::Map(int num_rows, int num_cols, int num_walls, Position* array_walls, int num_fake_walls, Position* array_fake_walls){
    map = new MapElement**[num_rows];
    for(int i = 0; i < num_rows; i++){
        map[i] = new MapElement*[num_cols];
        for(int j = 0; j < num_cols; j++){
            for(int k = 0; k < num_walls; k++){
                if(array_walls[k].isEqual(i,j)){
                    map[i][j] = new Wall();
                }else{
                    map[i][j] = new Path();
                }
            }
            for(int l = 0; l<num_fake_walls; l++){
                if(array_fake_walls[l].isEqual(i,j)){
                    map[i][j] = new FakeWall((i*257 + j*139+89)%900+1);
                }else{
                    map[i][j] = new Path();
                }
            }
        }
    } 
};
Map::~Map(){
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            delete map[i][j];
        }
        delete[] map[i];
    }
    delete[] map;
    num_cols = 0;
    num_rows = 0;
    map = NULL;
};
bool Map::isValid ( const Position & pos , MovingObject * mv_obj ) const {
    if ( (pos.getRow()) < 0 || pos.getRow() >= num_rows || pos.getCol() < 0 || pos.getCol() >= num_cols ) {
        return false;
    }
    if ( map[pos.getRow()][pos.getCol()] == NULL ) {
        return false;
    }
    if ( map[pos.getRow()][pos.getCol()]->getType() == FAKE_WALL ) {
        if(mv_obj->getObjectType() == SHERLOCK || mv_obj->getObjectType() == CRIMINAL || mv_obj->getObjectType() == ROBOT) return true;
        else if(mv_obj->getObjectType() == WATSON){
            if(mv_obj->getExp()>((pos.getRow()*257+pos.getCol()*139+89)%900+1)) return true;
            else return false;
        }
        else return false;
    }
    if ( map[pos.getRow()][pos.getCol()]->getType() == PATH && (mv_obj->getObjectType() == CRIMINAL || mv_obj->getObjectType() == ROBOT)) {
        return true;
    }
    return false;
};

//TODO: 3.3: POSITION
static const Position npos = {-1, -1};
Position::Position(int r = 0, int c= 0){
    this->r = r;
    this->c = c;
};
Position::Position(const string& str_pos){
    r = str_pos[str_pos.find("(")+1, str_pos.find(",")];
    c = str_pos[str_pos.find(",")+1, str_pos.find(")")];
    this->r = r;
    this->c = c;
};
int Position::getRow() const {
    return this->r;
};
int Position::getCol() const {
    return this->c;
};
void Position::setRow(int r){
    if(r>0) this->r = r;
};
void Position::setCol(int c){
    if(c>0) this->c = c;
};
string Position::str() const{
    return "(" + to_string(r) + "," + to_string(c) + ")";
};
bool Position::isEqual(int in_r, int in_c) const{
    if(r == in_r && c == in_c) return true;
    else return false;
};

//TODO: 3.4: MOVING OBJECT
MovingObject::MovingObject(int index, const Position pos, Map * map, const string & name=""){
    this->index = index;
    this->pos = pos;
    this->map = map;
    this->name = "";
};
Position MovingObject::getNextPosition(){
    Position next_pos = pos;
    if(this->map->isValid(this->pos, this)){
        next_pos.setRow(next_pos.getRow() + 1);
        next_pos.setCol(next_pos.getCol() + 1);
    }
    else{
        next_pos = npos;
    }
};
Position MovingObject::getCurrentPosition() const{
    return this->pos;
};
void MovingObject::move(){
    Position next_pos = getNextPosition();
    if (next_pos.isEqual(-1, -1)) return;
    pos = next_pos;
};
string MovingObject::str() const{
    return "MovingObject[index="+to_string(index)+";pos="+pos.str()+"]";
};
int MovingObject::getExp() const{
    return this->exp;
}
MovingObjectType MovingObject::getObjectType() const{
    return MovingObjectType();
};

//TODO: 3.5.1: CHARACTER
Character::Character(int index, const Position &init_pos, Map* map, const string &name="") : MovingObject(index, pos, map, name){
    this->index = index;
    this->pos = init_pos;
    this->map = map;
    this->name = "Character";
};
Position Character::getNextPosition() {
    Position next_pos = pos;
    if(map->isValid(next_pos, this)){
        next_pos.setRow(next_pos.getRow() + 1);
        next_pos.setCol(next_pos.getCol() + 1);
    }else{
        next_pos = npos;
    }
};
void Character::move(){
    Position next_pos = getNextPosition();
    if (next_pos.isEqual(-1, -1)) return;
    pos = next_pos;
};
string Character::str() const {
    return "Character[index="+to_string(index)+";pos="+pos.str()+"]"; 
};

//TODO: 3.5.2: SHERLOCK
Sherlock::Sherlock(int index, const string & moving_rule, const Position & init_pos, Map * map, const string &name = "", int init_hp, int init_exp) : Character(index, init_pos, map, "Sherlock"){
    this->moving_rule = moving_rule;
    this->index_moving_rule = index_moving_rule;
    this->name = "Sherlock";
    this->hp = init_hp;
    this->exp = init_exp;
};
Position Sherlock::getNextPosition() {
    Position next_pos = pos;
    switch (moving_rule[index_moving_rule]) {
        case 'U':
            next_pos = Position(pos.getRow() - 1, pos.getCol());
            break;
        case 'D':
            next_pos = Position(pos.getRow() + 1, pos.getCol());
            break;
        case 'L':
            next_pos = Position(pos.getRow(), pos.getCol() - 1);
            break;
        case 'R':
            next_pos = Position(pos.getRow(), pos.getCol() + 1);
            break;
        }
    if (map->isValid(next_pos, this)) return next_pos;
    else return npos;
    return next_pos;
};
void Sherlock::move(){
    Position next_pos = getNextPosition();
    if (next_pos.isEqual(-1, -1)) return;
    pos = next_pos;    
};
MovingObjectType Sherlock::getObjectType()const{
    return SHERLOCK;
};
string Sherlock::str() const {
    return "Sherlock[index="+to_string(index)+";pos="+pos.str()+";moving_rule="+moving_rule+"]";
};
int Sherlock::getHp() const{
    return this->hp;
};
int Sherlock::getExp() const{
    return this->exp;
};
int Sherlock::setExp(int init_exp) const{
    if(init_exp < 0) return 0;
    else if(init_exp > 900) return 900;
    else return init_exp;
};
int Sherlock::setHp(int init_hp) const{
    if(init_hp < 0) return 0;
    else if(init_hp > 500) return 500;
    else return init_hp;
};

//TODO: 3.6: WATSON
Watson::Watson(int index, const string & moving_rule, const Position & init_pos, Map * map, const string &name = "", int init_hp, int init_exp) : Character(index, init_pos, map, "Watson"){
    this->moving_rule = moving_rule;
    this->index_moving_rule = index_moving_rule;
    this->name = "Watson";
    this->hp = init_hp;
    this->exp = init_exp;
};
Position Watson::getNextPosition() {
    Position next_pos = pos;
    switch (moving_rule[index_moving_rule]) {
        case 'U':
            next_pos = Position(pos.getRow() - 1, pos.getCol());
            break;
        case 'D':
            next_pos = Position(pos.getRow() + 1, pos.getCol());
            break;
        case 'L':
            next_pos = Position(pos.getRow(), pos.getCol() - 1);
            break;
        case 'R':
            next_pos = Position(pos.getRow(), pos.getCol() + 1);
            break;
        }
    if (map->isValid(next_pos, this)) return next_pos;
    else return npos;
    return next_pos;
};
Position Watson::getCurrentPosition() const {
    return this->pos;
};
void Watson::move(){
    Position next_pos = getNextPosition();
    if (next_pos.isEqual(-1, -1)) return;
    pos = next_pos;
};
MovingObjectType Watson::getObjectType() const {
    return WATSON;
};
string Watson::str() const {
    return "Watson[index="+to_string(index)+";pos="+pos.str()+";moving_rule="+moving_rule+"]";
};
int Watson::getExp() const{
    return this->exp;
};
int Watson::getHp() const{
    return this->hp;
};
int Watson::setExp(int init_exp) const{
    if(init_exp < 0) return 0;
    else if(init_exp > 900) return 900;
    else return init_exp;
};
int Watson::setHp(int init_hp) const{
    if(init_hp < 0) return 0;
    else if(init_hp > 500) return 500;
    else return init_hp;
};

//TODO: 3.7: CRIMINAL
Criminal::Criminal(int index, const Position & init_pos, Map * map, const string &name = "", Sherlock * sherlock, Watson * watson) : Character(index, init_pos, map, "Criminal"){
    this->index = index;
    this->sherlock = sherlock;
    this->watson = watson;
    this->name = "Criminal";
};
Position Criminal::getNextPosition() {
    Position next_pos = pos;
    int max_distance = -1;
    Position arr[4];
    // 4 elements of the arr is the position after moving U,L,D,R respectively, ex: (1,2)
    arr[0] = Position(pos.getRow() - 1, pos.getCol());
    arr[1] = Position(pos.getRow(), pos.getCol() - 1);
    arr[2] = Position(pos.getRow() + 1, pos.getCol());
    arr[3] = Position(pos.getRow(), pos.getCol() + 1);
    for (int i = 0; i < 4; i++){
        if (map->isValid(arr[i], this)){
            int distance = abs(arr[i].getRow() - sherlock->getCurrentPosition().getRow()) + abs(arr[i].getCol() - sherlock->getCurrentPosition().getCol()) + abs(arr[i].getRow() - watson->getCurrentPosition().getRow()) + abs(arr[i].getCol() - watson->getCurrentPosition().getCol());
            if (distance > max_distance){
                max_distance = distance;
                next_pos = arr[i];
            }
            else if (distance == max_distance){
                continue;
            }
        }
    }
    return next_pos;
};
void Criminal::move(){
    Position next_pos = getNextPosition();
    if (next_pos.isEqual(-1, -1)) return;
    pos = next_pos;
};
MovingObjectType Criminal::getObjectType() const{
    return CRIMINAL;
}
string Criminal::str() const {
    return "Criminal[index="+to_string(index)+";pos="+pos.str()+"]";
};
bool Criminal::isCreatedRobotNext(int move) const {
    if(move%3==0) return true;
    else return false;
};

//TODO: 3.8: ARRAY MOVING OBJECT
ArrayMovingObject::ArrayMovingObject(int capacity){
    arr_mv_objs = new MovingObject*[capacity];
};
ArrayMovingObject::~ArrayMovingObject(){
    delete[] arr_mv_objs;
};
bool ArrayMovingObject::isFull() const {
    if(count==capacity) return true;
    else return false;
};
bool ArrayMovingObject::add(MovingObject* mv_obj){
    if(isFull()) return false;
    else{
        arr_mv_objs[count] = mv_obj;
        count++;
        return true;
    }
};
void ArrayMovingObject::remove(int index){
    arr_mv_objs[index] = NULL;
    count--;
    return;
};
int ArrayMovingObject::size() const{
    return count;
};
MovingObject* ArrayMovingObject::get(int index) const{
    return arr_mv_objs[index];
};
string ArrayMovingObject::str() const{
    string arraymovingobject;
    int i = 0;
    arraymovingobject = "ArrayMovingObject[count="+to_string(count)+";capacity="+to_string(capacity)+";";
    while ((i+1)==count){
        if(arr_mv_objs[i]!=NULL){
            arraymovingobject += arr_mv_objs[i]->str();
            arraymovingobject += ";";
            i++;
        }
    }
    return arraymovingobject;
};

//TODO: 3.9: CONFIGURATION
Configuration::Configuration(const string & filepath){
    ifstream infile;
    infile.open(filepath.c_str());
    if(infile.is_open()){
        string line;
        while(getline(infile, line)){
            if(line.find("MAP_NUM_ROWS")!= string::npos){
                map_num_rows = stoi(line.substr(line.find("=")+1));
            }
            else if(line.find("MAP_NUM_COLS")!= string::npos){
                map_num_cols = stoi(line.substr(line.find("=")+1));
            }
            else if(line.find("NUM_STEPS")!= string::npos){
                num_steps = stoi(line.substr(line.find("=")+1));
            }
            else if(line.find("MAX_NUM_MOVING_OBJECTS")!= string::npos){
                max_num_moving_objects = stoi(line.substr(line.find("=")+1));
            }
            else if(line.find("SHERLOCK_MOVING_RULE")!= string::npos){
                sherlock_moving_rule = line.substr(line.find("=")+1);
            }
            else if(line.find("SHERLOCK_INIT_POS")!= string::npos){
                sherlock_init_pos.setRow(stoi(line.substr(line.find("=")+1)));
                sherlock_init_pos.setCol(stoi(line.substr(line.find(",")+1)));
            }
            else if(line.find("WATSON_MOVING_RULE")!= string::npos){
                watson_moving_rule = line.substr(line.find("=")+1);
            }
            else if(line.find("WATSON_INIT_POS")!= string::npos){
                watson_init_pos.setRow(stoi(line.substr(line.find("=")+1)));
                watson_init_pos.setCol(stoi(line.substr(line.find(",")+1)));
            }
            else if(line.find("CRIMINAL_INIT_POS")!= string::npos){
                criminal_init_pos.setRow(stoi(line.substr(line.find("=")+1)));
                criminal_init_pos.setCol(stoi(line.substr(line.find(",")+1)));
            }
            else if(line.find("NUM_WALLS")!= string::npos){
                num_walls = stoi(line.substr(line.find("=")+1));
            }
            else if(line.find("ARRAY_WALLS")!= string::npos) {
                string walls = line.substr(line.find("=")+1);
                for(int i = 0; i < walls.size(); i++){
                    if(walls[i] == '1'){
                        arr_walls[i] = true;
                    }
                    else{
                        arr_walls[i] = false;
                    }
                }
                cout<<"arr_walls: ";
                for(int i = 0; i < walls.size(); i++){
                    
                }
                cout<<endl;
            }
            else if(line.find("NUM_FAKE_WALLS")!= string::npos){
                num_fake_walls = stoi(line.substr(line.find("=")+1));
            }
            else if(line.find("ARRAY_FAKE_WALLS")!= string::npos){
                string walls = line.substr(line.find("=")+1);
                for(int i = 0; i < walls.size(); i++){
                    if(walls[i] == '1'){
                        arr_fake_walls[i] = true;
                    }
                    else{
                        arr_fake_walls[i] = false;
                    }
                }
                cout<<"arr_fake_walls: ";
                for(int i = 0; i < walls.size(); i++){
                    
                }
                cout<<endl;
            }

        }
    }  
};
string Configuration::str() const{
    cout<<"Configuration["<<endl;
    cout<<"MAP_NUM_ROWS="<<map_num_rows<<endl;
    cout<<"MAP_NUM_COLS="<<map_num_cols<<endl;
    cout<<"MAX_NUM_MOVING_OBJECTS="<<max_num_moving_objects<<endl;
    cout<<"NUM_WALLS="<<num_walls<<endl;
    cout<<"ARRAY_WALLS="<<arr_walls<<endl;
    cout<<"NUM_FAKE_WALLS="<<num_fake_walls<<endl;
    cout<<"ARRAY_FAKE_WALLS="<<arr_fake_walls<<endl;
    cout<<"SHERLOCK_MOVING_RULE="<<sherlock_moving_rule<<endl;
    cout<<"SHERLOCK_INIT_POS="<<sherlock_init_pos.str()<<endl;
    cout<<"WATSON_MOVING_RULE="<<watson_moving_rule<<endl;
    cout<<"WATSON_INIT_POS="<<watson_init_pos.str()<<endl;
    cout<<"CRIMINAL_INIT_POS="<<criminal_init_pos.str()<<endl;
    cout<<"NUM_STEPS="<<num_steps<<endl;
    cout<<"]";
};

//TODO: 3.10: ROBOT
Robot::Robot(int index , const Position pos , Map * map , RobotType robot_type, Criminal* criminal, const string &name = "") : MovingObject(index, pos, map, "Robot"){
    this->index = index;
    this->pos = pos;
    this->map = map;
    this->robot_type = robot_type;
    this->criminal = criminal;
};
Robot* Robot::create(int index, Map* map, Criminal* criminal, Sherlock* sherlock, Watson* watson){
    Robot* robot = new Robot(index, map, ROBOT, criminal);
    return robot;
};
MovingObjectType Robot::getObjectType() const {
    return ROBOT;
};
RobotType Robot::getType() const {
    return robot_type;
};
int Robot::getDistance() const{
    return ;
};

//TODO: 3.10.1: ROBOTC
RobotC::RobotC(int index, const Position & init_pos, Map* map, RobotType robot_type, Criminal* criminal) : Robot(index, pos, map, robot_type, criminal, "RobotC"){
    this->robot_type = robot_type;
    this->pos = init_pos;
    this->criminal = criminal;
};
Position RobotC::getNextPosition() {
    Position next_pos = criminal->getCurrentPosition();
    if(map->isValid(next_pos, this)) return next_pos;
    else return npos;
};
void RobotC::move(){
    Position next_pos = getNextPosition();
    if (next_pos.isEqual(-1, -1)) return;
    pos = next_pos;
};
RobotType RobotC::getType() const{
    return C;
}
int RobotC::getDistance(Sherlock* sherlock) const{
    return (abs(pos.getRow()-sherlock->getCurrentPosition().getRow())+abs(pos.getCol()-sherlock->getCurrentPosition().getCol()));
};
int RobotC::getDistance(Watson* watson) const{
    return (abs(pos.getRow()-watson->getCurrentPosition().getRow())+abs(pos.getCol()-watson->getCurrentPosition().getCol()));
};
string RobotC::str() const{
    return "Robot[pos="+pos.str()+"type="+to_string(robot_type)+"dist="+""+"]";
};

//TODO:3.10.2: ROBOT S
RobotS::RobotS(int index, const Position & init_pos, Map* map, RobotType robot_type, Criminal* criminal, Sherlock* sherlock) : Robot(index, pos, map, robot_type, criminal, "RobotS"){
    this->robot_type = robot_type;
    this->pos = init_pos;
    this->criminal = criminal;
    this->sherlock = sherlock;
};
Position RobotS::getNextPosition() {
    Position next_pos = sherlock->getNextPosition();
    if(map->isValid(next_pos, this)) return next_pos;\
    else return npos;
};
void RobotS::move(){
    Position next_pos = getNextPosition();
    if (next_pos.isEqual(-1, -1)) return;
    pos = next_pos;
};
RobotType RobotS::getType() const{
    return S;
};
int RobotS::getDistance() const{
    return (abs(pos.getRow()-sherlock->getCurrentPosition().getRow())+abs(pos.getCol()-sherlock->getCurrentPosition().getCol()));
};
string RobotS::str() const{
    return "Robot[pos="+pos.str()+"type="+to_string(robot_type)+"dist="+to_string(getDistance())+"]";
};

//TODO:3.10.3: ROBOT W
RobotW::RobotW(int index, const Position & init_pos, Map* map, RobotType robot_type, Criminal* criminal, Watson* watson) : Robot(index, pos, map, robot_type, criminal, "RobotW"){
    this->robot_type = robot_type;
    this->pos = init_pos;
    this->criminal = criminal;
    this->watson = watson;
};
Position RobotW::getNextPosition() {
    Position next_pos = watson->getCurrentPosition();
    if(map->isValid(next_pos, this)) return next_pos;
    else return npos;
};
void RobotW::move(){
    Position next_pos = getNextPosition();
    if (next_pos.isEqual(-1, -1)) return;
    pos = next_pos;
};
RobotType RobotW::getType() const{
    return W;
};
int RobotW::getDistance() const{
    return (abs(pos.getRow()-watson->getCurrentPosition().getRow())+abs(pos.getCol()-watson->getCurrentPosition().getCol()));
};
string RobotW::str() const{
    return "Robot[pos="+pos.str()+"type="+to_string(robot_type)+"dist="+to_string(getDistance())+"]";
};

//TODO:3.10.4: ROBOT SW
RobotSW::RobotSW(int index, const Position & init_pos, Map* map, RobotType robot_type, Criminal* criminal, Sherlock* sherlock, Watson* watson) : Robot(index, pos, map, robot_type, criminal, "RobotSW"){
    this->robot_type = robot_type;
    this->pos = init_pos;
    this->criminal = criminal;
    this->sherlock = sherlock;
    this->watson = watson;
};
Position RobotSW::getNextPosition() {
    Position next_pos = npos;
    return next_pos;
};
void RobotSW::move(){
    Position next_pos = getNextPosition();
    if (next_pos.isEqual(-1, -1)) return;
    pos = next_pos;
};
 RobotType RobotSW::getType() const{
    return SW;
};
int RobotSW::getDistance() const{
    return (abs(pos.getRow()-watson->getCurrentPosition().getRow())+abs(pos.getCol()-watson->getCurrentPosition().getCol())) + (abs(pos.getRow()-sherlock->getCurrentPosition().getRow())+abs(pos.getCol()-sherlock->getCurrentPosition().getCol())); 
};
string RobotSW::str() const{
    return "Robot[pos="+pos.str()+"type="+to_string(robot_type)+"dist="+to_string(getDistance())+"]";
};

//TODO: 3.11: BASE ITEM
bool BaseItem::canUse(Character *obj, Robot *robot){
    if(obj->getObjectType() == SHERLOCK || obj->getObjectType() == WATSON){
        return true;
    }
    else{
        return false;
    }
};
//TODO: 3.11.1: MAGIC BOOK
bool MagicBook::canUse(Character *obj, Robot *robot){
    if(obj->getObjectType() == SHERLOCK && sherlock->getExp()<=350){
        return true;
    }
    else if(obj->getObjectType() == WATSON && watson->getExp()<=350){
        return true;
    }
    else{
        return false;
    }
};
void MagicBook::use(Character *obj, Robot *robot){
    if(canUse(sherlock, robot)){
        sherlock->setExp(sherlock->getExp()*1.25);
    }
    if(canUse(watson, robot)){
        watson->setExp(watson->getExp()*1.25);
    }
};

//TODO: 3.11.2: ENERGY DRINK
bool EnergyDrink::canUse(Character *obj, Robot *robot){
    if(obj->getObjectType() == SHERLOCK && sherlock->getHp()<=100){
        return true;
    }
    else if(obj->getObjectType() == WATSON && watson->getHp()<=100){
        return true;
    }
    else{
        return false;
    }
};
void EnergyDrink::use(Character *obj, Robot *robot){
    if(canUse(sherlock, robot)){
        sherlock->setHp(sherlock->getHp()*1.2);
    }
    if(canUse(watson, robot)){
        watson->setHp(watson->getHp()*1.2);
    }
};

//TODO: 3.11.3: FIRST AID
bool FirstAid::canUse(Character *obj, Robot *robot){
    if((obj->getObjectType() == SHERLOCK && sherlock->getHp()<=100) || (obj->getObjectType()== SHERLOCK && sherlock->getExp()<=350) ){
        return true;
    }
    else if((obj->getObjectType() == WATSON && watson->getHp()<=100) || (obj->getObjectType()== WATSON && watson->getExp()<=350) ){
        return true;
    }
    else{
        return false;
    }
};
void FirstAid::use(Character *obj, Robot *robot){
    if(canUse(sherlock, robot)){
        sherlock->setHp(sherlock->getHp()*1.5);
    }
    if(canUse(watson, robot)){
        watson->setHp(watson->getHp()*1.5);
    }
};

//TODO: 3.11.4: EXEMPTIONCARD
bool ExemptionCard::canUse(Character *obj, Robot *robot){
    if(obj->getObjectType() == SHERLOCK && (sherlock->getHp()%2!=0) ){
        return true;
    }
    else{
        return false;
    }
};
void ExemptionCard::use(Character *obj, Robot *robot){
    if(canUse(sherlock, robot)){
        sherlock->setHp(sherlock->getHp());
        sherlock->setExp(sherlock->getExp());
    }
};

//TODO: 3.11.5: PASSING CARD
bool PassingCard::canUse(Character *obj, Robot *robot){
    if(obj->getObjectType() == WATSON && (watson->getHp()%2==0) ){
        return true;
    }
    else{
        return false;
    }
};
void PassingCard::use(Character *obj, Robot *robot){
    if(canUse(watson, robot)){
        
    }
};

//TODO: 3.12: BASE BAG
//TODO: getnextposition robotS,W,SW
//TODO: class Robot create
//TODO: configuration

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////