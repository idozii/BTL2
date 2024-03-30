#include "study_in_pink2.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////
//TODO: 3.0: DISTANCE FUNCTION(MANHATTAN)
int distance(const Position &pos1, const Position &pos2){
    return abs(pos1.getRow()-pos2.getRow())+abs(pos1.getCol()-pos2.getCol());
};

//TODO: 3.1: MAP ELEMENT
MapElement::MapElement(ElementType in_type){
    this->type = type;
};
MapElement::~MapElement(){
    delete this;
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
    this->num_rows = num_rows;
    this->num_cols = num_cols;
    map = new MapElement**[num_rows];
    for(int i = 0; i < num_rows; i++){
        map[i] = new MapElement*[num_cols];
        for(int j = 0; j < num_cols; j++){
            map[i][j] = new Path();
        }
    }
    for(int i = 0; i < num_walls; i++){
        map[array_walls[i].getRow()][array_walls[i].getCol()] = new Wall();
    }
    for(int i = 0; i < num_fake_walls; i++){
        map[array_fake_walls[i].getRow()][array_fake_walls[i].getCol()] = new FakeWall((array_fake_walls[i].getRow()*257+array_fake_walls[i].getCol()*139+89)%900+1);
    }
    for(int i = 0; i < num_walls; i++){
        for(int j = 0; j < num_fake_walls; j++){
            if(array_walls[i].getRow() == array_fake_walls[j].getRow() && array_walls[i].getCol() == array_fake_walls[j].getCol()){
                map[array_walls[i].getRow()][array_walls[i].getCol()] = new Wall();
            }
        }
    }
};
Map::~Map(){
    for(int i = 0; i < num_rows; i++){
        for(int j = 0; j < num_cols; j++){
            delete map[i][j];
        }
        delete[] map[i];
    }
    delete[] map;
};
int Map::getNumRows() const{
    return this->num_rows;
};
int Map::getNumCols() const{
    return this->num_cols;
};
ElementType Map::getElementType(const Position &pos) const{
    return map[pos.getRow()][pos.getCol()]->getType();
};
bool Map::isValid ( const Position & pos , MovingObject * mv_obj ) const {
    if ( (pos.getRow()) < 0 || pos.getRow() >= num_rows || pos.getCol() < 0 || pos.getCol() >= num_cols ) {
        return false;
    }
    if ( getElementType(pos) == NULL ) {
        return false;
    }
    if ( getElementType(pos) == FAKE_WALL ) {
        if(mv_obj->getObjectType() == SHERLOCK || mv_obj->getObjectType() == CRIMINAL || mv_obj->getObjectType() == ROBOT) return true;
        else if(mv_obj->getObjectType() == WATSON){
            if(mv_obj->getExp()>((pos.getRow()*257+pos.getCol()*139+89)%900+1)) return true;
            else return false;
        }
        else return false;
    }
    if (getElementType(pos) == PATH) {
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
    this->r = stoi(str_pos.substr(1, str_pos.find(",")-1));
    this->c = stoi(str_pos.substr(str_pos.find(",")+1, str_pos.find(")")-1));
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
bool Position::isEqual(const Position &pos) const{
    if(r == pos.getRow() && c == pos.getCol()) return true;
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
    return next_pos;
};
Position MovingObject::getCurrentPosition() const{
    return this->pos;
};
void MovingObject::move(){
    Position next_pos = getNextPosition();
    if (next_pos.isEqual(Position::npos)) return;
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
Character::Character(int index, const Position pos, Map* map, const string &name="") : MovingObject(index, pos, map, name){
    this->name = "Character";
};
Position Character::getNextPosition() {
    Position next_pos = pos;
    if(map->isValid(next_pos, this)){
        next_pos.setRow(next_pos.getRow() + 1);
        next_pos.setCol(next_pos.getCol() + 1);
    }
    else{
        next_pos = npos;
    }
    return next_pos;
};
void Character::move(){
    Position next_pos = getNextPosition();
    if (next_pos.isEqual(Position::npos)) return;
    pos = next_pos;
};
string Character::str() const {
    return "Character[index="+to_string(index)+";pos="+pos.str()+"]"; 
};
MovingObjectType Character::getObjectType() const{
    return MovingObjectType();
};

//TODO: 3.5.2: SHERLOCK
Sherlock::Sherlock(int index, const string & moving_rule, const Position & init_pos, Map * map, const string &name = "", int init_hp, int init_exp) : Character(index, init_pos, map, "Sherlock"){
    this->moving_rule = moving_rule;
    this->index_moving_rule = index_moving_rule;
};
Position Sherlock::getNextPosition() {
    Position next_pos = pos;
    if(moving_rule.length() == 0) return npos;
    if (moving_rule[index_moving_rule] == 'U'){
        next_pos.setRow(next_pos.getRow() - 1);
    }
    else if (moving_rule[index_moving_rule] == 'L'){
        next_pos.setCol(next_pos.getCol() - 1);
    }
    else if (moving_rule[index_moving_rule] == 'D'){
        next_pos.setRow(next_pos.getRow() + 1);
    }
    else if (moving_rule[index_moving_rule] == 'R'){
        next_pos.setCol(next_pos.getCol() + 1);
    }
    index_moving_rule++;
    if (index_moving_rule == moving_rule.length()){
        index_moving_rule = 0;
    }
    if (map->isValid(next_pos, this)) return next_pos;
    else return npos;
};
void Sherlock::move(){
    Position next_pos = getNextPosition();
    if (next_pos.isEqual(Position::npos)) return;
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
void Sherlock::meet(RobotS* robotS){
    if (distance(pos, robotS->getCurrentPosition()) == 1){
        this->meet(robotS);
        if (exp > 400){
            delete robotS;
        }
        else{
            exp = exp*0.9;
        }
    }
};
void Sherlock::meet(RobotC* robotC){
    if (distance(pos, robotC->getCurrentPosition()) == 1){
        this->meet(robotC);
        if (exp > 500){
            delete robotC;
        }
        else{
            delete robotC;
        }
    }
};
void Sherlock::meet(RobotSW* robotSW){
    if (distance(pos, robotSW->getCurrentPosition()) == 1){
        this->meet(robotSW);
        if (exp > 300 && hp > 335){
            delete robotSW;
        }
        else{
            hp = hp*0.85;
            exp = exp*0.85;
        }
    }
};
void Sherlock::meet(RobotW* robotW){
    if (distance(pos, robotW->getCurrentPosition()) == 1){
        this->meet(robotW);
        delete robotW;
    }
};
void Sherlock::meet(Watson* watson){

};

//TODO: 3.6: WATSON
Watson::Watson(int index, const string & moving_rule, const Position & init_pos, Map * map, const string &name = "", int init_hp, int init_exp) : Character(index, init_pos, map, "Watson"){
    this->moving_rule = moving_rule;
    this->index_moving_rule = index_moving_rule;
};
Position Watson::getNextPosition() {
    Position next_pos = pos;
    if(moving_rule.length() == 0) return npos;
    if (moving_rule[index_moving_rule] == 'U'){
        next_pos.setRow(next_pos.getRow() - 1);
    }
    else if (moving_rule[index_moving_rule] == 'L'){
        next_pos.setCol(next_pos.getCol() - 1);
    }
    else if (moving_rule[index_moving_rule] == 'D'){
        next_pos.setRow(next_pos.getRow() + 1);
    }
    else if (moving_rule[index_moving_rule] == 'R'){
        next_pos.setCol(next_pos.getCol() + 1);
    }
    index_moving_rule++;
    if (index_moving_rule == moving_rule.length()){
        index_moving_rule = 0;
    }
    if (map->isValid(next_pos, this)) return next_pos;
    else return npos;
};
void Watson::move(){
    Position next_pos = getNextPosition();
    if (next_pos.isEqual(Position::npos)) return;
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
void Watson::meet(RobotS* robotS){
    if(distance(pos, robotS->getCurrentPosition()) == 1){
        ;
    }
};
void Watson::meet(RobotC* robotC){
    if (distance(pos, robotC->getCurrentPosition()) == 1){
        this->meet(robotC);
        delete robotC;
    }
};
void Watson::meet(RobotSW* robotSW){
    if (distance(pos, robotSW->getCurrentPosition()) == 1){
        this->meet(robotSW);
        if (exp > 600 && hp > 165){
            delete robotSW;
        }
        else{
            hp = hp*0.85;
            exp = exp*0.85;
        }
    }
};
void Watson::meet(RobotW* robotW){
    if (distance(pos, robotW->getCurrentPosition()) == 1){
        this->meet(robotW);
        if (hp > 350){
            delete robotW;
        }
        else{
            hp = hp*0.95;
        }
    }
};
void Watson::meet(Sherlock* sherlock){

};

//TODO: 3.7: CRIMINAL
Criminal::Criminal(int index, const Position & init_pos, Map * map, const string &name = "", Sherlock * sherlock, Watson * watson) : Character(index, init_pos, map, "Criminal"){
    this->sherlock = sherlock;
    this->watson = watson;
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
    if (next_pos.isEqual(Position::npos)) return;
    pos = next_pos;
    move_count++;
};
MovingObjectType Criminal::getObjectType() const{
    return CRIMINAL;
}
string Criminal::str() const {
    return "Criminal[index="+to_string(index)+";pos="+pos.str()+"]";
};
bool Criminal::isCreatedRobotNext(int move) const {
    if (move % 3 == 0) return true;
    else return false;
};

//TODO: 3.8: ARRAY MOVING OBJECT
ArrayMovingObject::ArrayMovingObject(int capacity){
    this->capacity = capacity;
    this->count = 0;
    arr_mv_objs = new MovingObject*[capacity];
    for(int i = 0; i < capacity; i++){
        arr_mv_objs[i] = NULL;
    }
};
ArrayMovingObject::~ArrayMovingObject(){
    for(int i = 0; i < capacity; i++){
        delete arr_mv_objs[i];
    }
    delete[] arr_mv_objs;
};
bool ArrayMovingObject::isFull() const {
    if(count==capacity) return true;
    else return false;
};
bool ArrayMovingObject::add(MovingObject* mv_obj){
    if(isFull()) return false;
    for(int i = 0; i < capacity; i++){
        if(arr_mv_objs[i]==NULL){
            arr_mv_objs[i] = mv_obj;
            count++;
            return true;
        }
    }
    return false;
};
void ArrayMovingObject::remove(int index){
    if(arr_mv_objs[index]!=NULL){
        delete arr_mv_objs[index];
        arr_mv_objs[index] = NULL;
        count--;
    }
};
int ArrayMovingObject::size() const{
    return count;
};
MovingObject* ArrayMovingObject::get(int index) const{
    if(arr_mv_objs[index]!=NULL) return arr_mv_objs[index];
    else return NULL;
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
bool ArrayMovingObject::checkMeet(int index){
    if(arr_mv_objs[index]->getObjectType() == SHERLOCK){
        if(arr_mv_objs[index]->getCurrentPosition().isEqual(arr_mv_objs[index]->getNextPosition())){
            return true;
        }
        else return false;
    }
    else if(arr_mv_objs[index]->getObjectType() == WATSON){
        if(arr_mv_objs[index]->getCurrentPosition().isEqual(arr_mv_objs[index]->getNextPosition())){
            return true;
        }
        else return false;
    }
    else if(arr_mv_objs[index]->getObjectType() == CRIMINAL){
        if(arr_mv_objs[index]->getCurrentPosition().isEqual(arr_mv_objs[index]->getNextPosition())){
            return true;
        }
        else return false;
    }
    else if(arr_mv_objs[index]->getObjectType() == ROBOT){
        if(arr_mv_objs[index]->getCurrentPosition().isEqual(arr_mv_objs[index]->getNextPosition())){
            return true;
        }
        else return false;
    }
    else return false;
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
                sherlock_init_pos.setRow(stoi(line.substr(line.find("(")+1, line.find(",")-1)));
                sherlock_init_pos.setCol(stoi(line.substr(line.find(",")+1, line.find(")")-1)));
            }
            else if(line.find("SHERLOCK_INIT_HP")!= string::npos){
                sherlock_init_hp = stoi(line.substr(line.find("=")+1));
            }
            else if(line.find("SHERLOCk_INIT_EXP")!= string::npos){
                sherlock_init_exp = stoi(line.substr(line.find("=")+1));
            }
            else if(line.find("WATSON_MOVING_RULE")!= string::npos){
                watson_moving_rule = line.substr(line.find("=")+1);
            }
            else if(line.find("WATSON_INIT_POS")!= string::npos){
                watson_init_pos.setRow(stoi(line.substr(line.find("(")+1, line.find(",")-1)));
                watson_init_pos.setCol(stoi(line.substr(line.find(",")+1, line.find(")")-1)));
            }
            else if(line.find("WATSON_INIT_HP")!= string::npos){
                watson_init_hp = stoi(line.substr(line.find("=")+1));
            }
            else if(line.find("WATSON_INIT_EXP")!= string::npos){
                watson_init_exp = stoi(line.substr(line.find("=")+1));
            }
            else if(line.find("CRIMINAL_INIT_POS")!= string::npos){
                criminal_init_pos.setRow(stoi(line.substr(line.find("(")+1, line.find(",")-1)));
                criminal_init_pos.setCol(stoi(line.substr(line.find(",")+1, line.find(")")-1)));
            }
            else if(line.find("ARRAY_WALLS")!= string::npos) {
                num_walls = stoi(line.substr(line.find("=")+1));
                arr_walls = new Position[num_walls];
                for(int i = 0; i < num_walls; i++){
                    arr_walls[i].setRow(stoi(line.substr(line.find("(")+1, line.find(",")-1)));
                    arr_walls[i].setCol(stoi(line.substr(line.find(",")+1, line.find(")")-1)));
                }
            }
            else if(line.find("ARRAY_FAKE_WALLS")!= string::npos){
                num_fake_walls = stoi(line.substr(line.find("=")+1));
                arr_fake_walls = new Position[num_fake_walls];
                for(int i = 0; i < num_fake_walls; i++){
                    arr_fake_walls[i].setRow(stoi(line.substr(line.find("(")+1, line.find(",")-1)));
                    arr_fake_walls[i].setCol(stoi(line.substr(line.find(",")+1, line.find(")")-1)));
                }
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
    cout<<"SHERLOCK_INIT_HP"<<sherlock_init_hp<<endl;
    cout<<"SHERLOCK_INIT_EXP"<<sherlock_init_exp<<endl;
    cout<<"WATSON_MOVING_RULE="<<watson_moving_rule<<endl;
    cout<<"WATSON_INIT_POS="<<watson_init_pos.str()<<endl;
    cout<<"WATSON_INIT_HP="<<watson_init_hp<<endl;
    cout<<"WATSON_INIT_EXP="<<watson_init_exp<<endl;
    cout<<"CRIMINAL_INIT_POS="<<criminal_init_pos.str()<<endl;
    cout<<"NUM_STEPS="<<num_steps<<endl;
    cout<<"]";
};

//TODO: 3.10: ROBOT
Robot::Robot(int index , const Position pos , Map * map , RobotType robot_type, Criminal* criminal, const string &name = "") : MovingObject(index, pos, map, "Robot"){
    this->robot_type = robot_type;
    this->criminal = criminal;
};
Robot* Robot::create(int index, Map* map, Criminal* criminal, Sherlock* sherlock, Watson* watson){
    if(criminal->isCreatedRobotNext(criminal->move_count)){
        return new RobotC(index, criminal->getCurrentPosition(), map, C, criminal);
    }    
    else if(distance(criminal->getCurrentPosition(), sherlock->getCurrentPosition()) > distance(criminal->getCurrentPosition(), watson->getCurrentPosition())){
        return new RobotS(index, criminal->getCurrentPosition(), map, S, criminal, sherlock);
    }
    else if(distance(criminal->getCurrentPosition(), sherlock->getCurrentPosition()) < distance(criminal->getCurrentPosition(), watson->getCurrentPosition())){
        return new RobotW(index, criminal->getCurrentPosition(), map, W, criminal, watson);
    }
    else return new RobotSW(index, criminal->getCurrentPosition(), map, SW, criminal, sherlock, watson);
};
MovingObjectType Robot::getObjectType() const {
    return ROBOT;
};
RobotType Robot::getType() const {
    return robot_type;
};
int Robot::getDistance() const{
    return 0;
};
string Robot::str() const{
    return "Robot[pos="+pos.str()+"type="+to_string(robot_type)+"dist="+to_string(getDistance())+"]";
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
    if (next_pos.isEqual(Position::npos)) return;
    pos = next_pos;
};
RobotType RobotC::getType() const{
    return C;
}
int RobotC::getDistance(Sherlock* sherlock) const{
    return distance(pos, sherlock->getCurrentPosition());
};
int RobotC::getDistance(Watson* watson) const{
    return distance(pos, watson->getCurrentPosition());
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
    Position next_pos = npos;
    int min_distance = 1000;
    Position arr[4];
    arr[0] = Position(pos.getRow() - 1, pos.getCol());
    arr[1] = Position(pos.getRow(), pos.getCol() - 1);
    arr[2] = Position(pos.getRow() + 1, pos.getCol());
    arr[3] = Position(pos.getRow(), pos.getCol() + 1);
    for (int i = 0; i < 4; i++){
        if (map->isValid(arr[i], this)){
            int distance = abs(arr[i].getRow() - sherlock->getCurrentPosition().getRow()) + abs(arr[i].getCol() - sherlock->getCurrentPosition().getCol());
            if (distance < min_distance){
                min_distance = distance;
                next_pos = arr[i];
            }
            else if (distance == min_distance){
                continue;
            }
        }
    }
    return next_pos;
};
void RobotS::move(){
    Position next_pos = getNextPosition();
    if (next_pos.isEqual(Position::npos)) return;
    pos = next_pos;
};
RobotType RobotS::getType() const{
    return S;
};
int RobotS::getDistance() const{
    return distance(pos, sherlock->getCurrentPosition());
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
    Position next_pos = npos;
    int min_distance = 1000;
    Position arr[4];
    arr[0] = Position(pos.getRow() - 1, pos.getCol());
    arr[1] = Position(pos.getRow(), pos.getCol() - 1);
    arr[2] = Position(pos.getRow() + 1, pos.getCol());
    arr[3] = Position(pos.getRow(), pos.getCol() + 1);
    for (int i = 0; i < 4; i++){
        if (map->isValid(arr[i], this)){
            int distance = abs(arr[i].getRow() - watson->getCurrentPosition().getRow()) + abs(arr[i].getCol() - watson->getCurrentPosition().getCol());
            if (distance < min_distance){
                min_distance = distance;
                next_pos = arr[i];
            }
            else if (distance == min_distance){
                continue;
            }
        }
    }
    return next_pos;
};
void RobotW::move(){
    Position next_pos = getNextPosition();
    if (next_pos.isEqual(Position::npos)) return;
    pos = next_pos;
};
RobotType RobotW::getType() const{
    return W;
};
int RobotW::getDistance() const{
    return distance(pos, watson->getCurrentPosition());
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
    int min_distance = 1000;
    Position arr[4];
    arr[0] = Position(pos.getRow() - 1, pos.getCol());
    arr[1] = Position(pos.getRow(), pos.getCol() - 1);
    arr[2] = Position(pos.getRow() + 1, pos.getCol());
    arr[3] = Position(pos.getRow(), pos.getCol() + 1);
    for (int i = 0; i < 4; i++){
        if (map->isValid(arr[i], this)){
            int distance = abs(arr[i].getRow() - sherlock->getCurrentPosition().getRow()) + abs(arr[i].getCol() - sherlock->getCurrentPosition().getCol()) + abs(arr[i].getRow() - watson->getCurrentPosition().getRow()) + abs(arr[i].getCol() - watson->getCurrentPosition().getCol());
            if (distance < min_distance){
                min_distance = distance;
                next_pos = arr[i];
            }
            else if (distance == min_distance){
                continue;
            }
        }
    }
    return next_pos;
};
void RobotSW::move(){
    Position next_pos = getNextPosition();
    if (next_pos.isEqual(Position::npos)) return;
    pos = next_pos;
};
 RobotType RobotSW::getType() const{
    return SW;
};
int RobotSW::getDistance() const{
    return distance(pos, sherlock->getCurrentPosition()) + distance(pos, watson->getCurrentPosition());
};
string RobotSW::str() const{
    return "Robot[pos="+pos.str()+"type="+to_string(robot_type)+"dist="+to_string(getDistance())+"]";
};

//TODO: 3.11: BASE ITEM
BaseItem::BaseItem(ItemType type){
    this->type = type;
};
bool BaseItem::canUse(Character *obj, Robot *robot){
    if(obj->getObjectType() == SHERLOCK || obj->getObjectType() == WATSON){
        return true;
    }
    else{
        return false;
    }
};
void BaseItem::use(Character *obj, Robot *robot){
    if(canUse(obj, robot)){
        ;
    }
};
ItemType BaseItem::getType() const{
    return type;
};
string BaseItem::str() const{
    return to_string(type);
};

//TODO: 3.11.1: MAGIC BOOK
MagicBook::MagicBook(ItemType type, Sherlock* sherlock, Watson* watson) : BaseItem(type){
    this->type = MAGIC_BOOK;
    this->sherlock = sherlock;
    this->watson = watson;
};
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
EnergyDrink::EnergyDrink(ItemType type, Sherlock* sherlock, Watson* watson) : BaseItem(type){
    this->type = ENERGY_DRINK;
    this->sherlock = sherlock;
    this->watson = watson;
};
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
FirstAid::FirstAid(ItemType type, Sherlock* sherlock, Watson* watson) : BaseItem(type){
    this->type = FIRST_AID;
    this->sherlock = sherlock;
    this->watson = watson;
};
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
ExemptionCard::ExemptionCard(ItemType type, Sherlock* sherlock) : BaseItem(type){
    this->type = EXEMPTION_CARD;
    this->sherlock = sherlock;
};
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
PassingCard::PassingCard(ItemType type, Watson* watson, const string &challenge) : BaseItem(type){
    this->type = PASSING_CARD;
    this->watson = watson;
    this->challenge = challenge;
};
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
BaseBag::BaseBag(Character* obj, BaseItem* item){
    this->obj = obj;
    this->item = item;
};
bool BaseBag::insert(BaseItem* item){
    if(this->item == NULL){
        this->item = item;
        return true;
    }
    else return false;
};
BaseItem* BaseBag::get(){
    return item;
};
BaseItem* BaseBag::get(ItemType type){
    if(item->getType() == type){
        return item;
    }
    else return NULL;
};
string BaseBag::str() const{
    return "BaseBag[count="+to_string(count)+";"+item->str()+"]";
};

//TODO: 3.12.1: SHERLOCK BAG
SherlockBag::SherlockBag(Sherlock* sherlock):BaseBag(sherlock, item){
    this->sherlock = sherlock;
    this->count = 0;
    arr_bag = new BaseBag*[sherlock->getExp()];
    for(int i = 0; i < sherlock->getExp(); i++){
        arr_bag[i] = NULL;
    }
};

//TODO: 3.12.2: WATSON BAG
WatsonBag::WatsonBag(Watson* watson):BaseBag(watson, item){
    this->watson = watson;
    this->count = 0;
    arr_bag = new BaseBag*[watson->getExp()];
    for(int i = 0; i < watson->getExp(); i++){
        arr_bag[i] = NULL;
    }
};

//TODO: 3.13: StudyPink
StudyPinkProgram::StudyPinkProgram(const string &config_file_path){
    Configuration config(config_file_path);
    map = new Map(config.map_num_rows, config.map_num_cols, config.num_walls, config.arr_walls, config.num_fake_walls, config.arr_fake_walls);
    arr_mv_objs = new ArrayMovingObject(config.max_num_moving_objects);
    sherlock = new Sherlock(0, config.sherlock_moving_rule, config.sherlock_init_pos, map, "Sherlock", config.sherlock_init_hp, config.sherlock_init_exp);
    watson = new Watson(1, config.watson_moving_rule, config.watson_init_pos, map, "Watson", config.watson_init_hp, config.watson_init_exp);
    criminal = new Criminal(2, config.criminal_init_pos, map, "Criminal", sherlock, watson);
    arr_mv_objs->add(sherlock);
    arr_mv_objs->add(watson);
    arr_mv_objs->add(criminal);
};
bool StudyPinkProgram::isStop() const{
    if(sherlock->getHp() == 0 || watson->getHp() == 0 || sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition()) || watson->getCurrentPosition().isEqual(criminal->getCurrentPosition())){
        return true;
    }
    else return false;
};
void StudyPinkProgram::run(bool verbose){
    for (int istep = 0; istep < config->num_steps; ++istep) {
        for (int i = 0; i < arr_mv_objs->size(); ++i) {
            arr_mv_objs->get(i)->move();
            if (isStop()) {
                printStep(istep);
                break;
            }
            if (verbose) {
                printStep(istep);
            }
        }
    }
    printResult();
};
StudyPinkProgram::~StudyPinkProgram(){
    delete map;
    delete arr_mv_objs;
    delete sherlock;
    delete watson;
    delete criminal;
};


//TODO: base item, base bag, sherlockbag, watsonbag

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////