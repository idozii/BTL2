#include "study_in_pink2.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////
class MovingObject;
class Position;
class Configuration;
class Map;

class Criminal;
class RobotS;
class RobotW;
class RobotSW;
class RobotC;

class ArrayMovingObject;
class StudyPinkProgram;

class BaseItem;
class BaseBag;
class SherlockBag;
class WatsonBag;

//TODO: 3.0: DISTANCE FUNCTION(MANHATTAN)
int distance(const Position &pos1, const Position &pos2){
    return abs(pos1.getRow()-pos2.getRow())+abs(pos1.getCol()-pos2.getCol());
};

//TODO: 3.1: MAP ELEMENT
MapElement::MapElement(ElementType in_type){
    type = in_type;
};
MapElement::~MapElement(){};
ElementType MapElement::getType() const{
    return type;
};
Path::Path() : MapElement(PATH){
    this->type = PATH;
};
ElementType Path::getType() const{
    return PATH;
};
Wall::Wall() : MapElement(WALL){
    this->type = WALL;
};
ElementType Wall::getType() const{
    return WALL;
};
FakeWall::FakeWall(int in_req_exp) : MapElement(FAKE_WALL){
    this->type = FAKE_WALL;
    this->req_exp = in_req_exp;
};

int FakeWall::getReqExp() const {
    return req_exp;
};
ElementType FakeWall::getType() const{
    return FAKE_WALL;
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
const Position Position::npos = Position(-1,-1);
Position::Position(int r, int c){
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
string Position::str() const {
    return "(" + to_string(r) + "," + to_string(c) + ")";
};
bool Position::isEqual(const Position &pos) const{
    if(r == pos.getRow() && c == pos.getCol()) return true;
    else return false;
};

//TODO: 3.4: MOVING OBJECT
MovingObject::MovingObject(int index, const Position pos, Map * map, const string & name){
    this->index = index;
    this->pos = pos;
    this->map = map;
    this->name = name;
};
MovingObject::~MovingObject(){};
Position MovingObject::getNextPosition(){
    Position next_pos = pos;
    if(this->map->isValid(this->pos, this)){
        next_pos.setRow(next_pos.getRow() + 1);
        next_pos.setCol(next_pos.getCol() + 1);
    }
    else{
        next_pos = Position::npos;
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
};
int MovingObject::getHp() const{
    return this->hp;
};
MovingObjectType MovingObject::getObjectType() const{
    return MovingObjectType();
};

//TODO: 3.5.1: CHARACTER
Character::Character(int index, const Position & init_pos, Map * map, const string & name) : MovingObject(index, init_pos, map, name){
    this->index = index;
    this->pos = init_pos;
    this->map = map;
    this->name = name;
};
Character::~Character(){};
Position Character::getNextPosition() {
    Position next_pos = pos;
    if(map->isValid(next_pos, this)){
        next_pos.setRow(next_pos.getRow() + 1);
        next_pos.setCol(next_pos.getCol() + 1);
    }
    else{
        next_pos = Position::npos;
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
int Character::getExp() const{
    return this->exp;
};
int Character::setExp(int init_exp) const{
    if(init_exp < 0) return 0;
    else if(init_exp > 900) return 900;
    else return init_exp;
};
int Character::getHp() const{
    return this->hp;
};
int Character::setHp(int init_hp) const{
    if(init_hp < 0) return 0;
    else if(init_hp > 500) return 500;
    else return init_hp;
};

//TODO: 3.5.2: SHERLOCK
Sherlock::Sherlock(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp) : Character(index, init_pos, map, "Sherlock"){
    this->hp = init_hp;
    this->exp = init_exp;
    this->moving_rule = moving_rule;
    sherlockBag = new SherlockBag(this);
};
Sherlock::~Sherlock(){
    delete sherlockBag;
};
Position Sherlock::getNextPosition() {
    Position next_pos = pos;
    if(moving_rule.length() == 0) return Position::npos;
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
    else return Position::npos;
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
bool Sherlock::meet(RobotS* robotS){
    if(pos.isEqual(robotS->getCurrentPosition())) {
        if(exp > 400){
            this->meet(robotS);
            delete robotS;
            return true;
        }
        else{
            exp = exp*90/100;
            return false;
        }
    }
};
bool Sherlock::meet(RobotC* robotC){
    if (pos.isEqual(robotC->getCurrentPosition())) {
        this->meet(robotC);
        if (exp > 500){
            delete robotC;
            return true;
        }
        else{
            delete robotC;
            return false;
        }
    }
};
bool Sherlock::meet(RobotSW* robotSW){
    if (pos.isEqual(robotSW->getCurrentPosition())) {
        this->meet(robotSW);
        if (exp > 300 && hp > 335){
            delete robotSW;
            return true;
        }
        else{
            hp = hp*85/100;
            exp = exp*85/100;
            return false;
        }
    }
};
bool Sherlock::meet(RobotW* robotW){
    if (pos.isEqual(robotW->getCurrentPosition())) {
        this->meet(robotW);
        delete robotW;
        return true;
    }
};
bool Sherlock::meet(Watson* watson){
    if(pos.isEqual(watson->getCurrentPosition())){
        if(sherlockBag->getCount() == 0 && watsonBag->getCount() == 0) return false;
        else if(sherlockBag->getCount() == 0 && watson->watsonBag->getCount() != 0){
            for(int i = 0; i < watson->watsonBag->getCount(); i++){
                if(watson->watsonBag->get(i)->getType() == PASSING_CARD){
                    watson->watsonBag->remove(i);
                    sherlockBag->insert(PassingCard());
                }
            }
        }
        else if(sherlockBag->getCount() != 0 && watson->watsonBag->size() == 0){
            for(int i = 0; i < sherlockBag->size(); i++){
                if(sherlockBag->get(i)->getType() == EXCEPTION_CARD){
                    sherlockBag->remove(i);
                    watson->watsonBag->add(new ExceptionCard());
                }
            }
        }
        else if(sherlockBag->getCount() != 0 && watson->watsonBag->size() != 0){
            for(int i = 0; i < sherlockBag->size(); i++){
                if(sherlockBag->get(i)->getType() == EXCEPTION_CARD){
                    sherlockBag->remove(i);
                    watson->watsonBag->add(new ExceptionCard());
                }
            }
            for(int i = 0; i < watson->watsonBag->size(); i++){
                if(watson->watsonBag->get(i)->getType() == PASSING_CARD){
                    watson->watsonBag->remove(i);
                    sherlockBag->add(new PassingCard());
                }
            }
        }
        return true;
    }
};

//TODO: 3.6: WATSON
Watson::Watson(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp) : Character(index, init_pos, map, "Watson"){
    this->hp = init_hp;
    this->exp = init_exp;
    this->moving_rule = moving_rule;
    watsonBag = new WatsonBag(this);
};
Watson::~Watson(){
    delete watsonBag;
};
Position Watson::getNextPosition() {
    Position next_pos = pos;
    if(moving_rule.length() == 0) return Position::npos;
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
    else return Position::npos;
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
bool Watson::meet(RobotS* robotS){
    if(distance(pos, robotS->getCurrentPosition()) == 1){
        ;
    }
};
bool Watson::meet(RobotC* robotC){
    if (distance(pos, robotC->getCurrentPosition()) == 1){
        this->meet(robotC);
        delete robotC;
    }
};
bool Watson::meet(RobotSW* robotSW){
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
bool Watson::meet(RobotW* robotW){
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
bool Watson::meet(Sherlock* sherlock){

};

//TODO: 3.7: CRIMINAL
Criminal::Criminal(int index, const Position & init_pos, Map * map, Sherlock * sherlock, Watson * watson) : Character(index, init_pos, map, "Criminal"){
    this->sherlock = sherlock;
    this->watson = watson;
};
Criminal::~Criminal(){};
Position Criminal::getNextPosition() {
    Position next_pos = pos;
    int max_distance = -1;
    Position arr[4];
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
    moveCount++;
};
MovingObjectType Criminal::getObjectType() const{
    return CRIMINAL;
}
string Criminal::str() const {
    return "Criminal[index="+to_string(index)+";pos="+pos.str()+"]";
};
int Criminal::getCount(){
    return this->moveCount;
};
bool Criminal::isCreatedRobotNext(){
    if (getCount() % 3 == 0 && getCount()>0) return true;
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
Configuration::~Configuration(){
    delete[] arr_fake_walls;
    delete[] arr_walls;
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
Robot::Robot(int index , const Position pos , Map * map , RobotType robot_type, Criminal* criminal, const string &name) : MovingObject(index, pos, map, "Robot"){
    this->robot_type = robot_type;
    this->criminal = criminal;
};
Robot* Robot::create(int index, Map* map, Criminal* criminal, Sherlock* sherlock, Watson* watson){
    if(criminal->isCreatedRobotNext()){
        return new RobotC(index, criminal->getCurrentPosition(), map, C, criminal);
    }    
    else if(distance(criminal->getCurrentPosition(), sherlock->getCurrentPosition()) > distance(criminal->getCurrentPosition(), watson->getCurrentPosition())){
        return new RobotS(index, criminal->getCurrentPosition(), map, S, criminal, sherlock);
    }
    else if(distance(criminal->getCurrentPosition(), sherlock->getCurrentPosition()) < distance(criminal->getCurrentPosition(), watson->getCurrentPosition())){
        return new RobotW(index, criminal->getCurrentPosition(), map, W, criminal, watson);
    }
    else if(distance(criminal->getCurrentPosition(), sherlock->getCurrentPosition()) == distance(criminal->getCurrentPosition(), watson->getCurrentPosition())){
        return new RobotSW(index, criminal->getCurrentPosition(), map, SW, criminal, sherlock, watson);
    }
};
MovingObjectType Robot::getObjectType() const{
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
    else return Position::npos;
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
    Position next_pos = pos;
    int min_distance = 100;
    Position arr[4];
    arr[0] = Position(pos.getRow(), pos.getCol()+1);
    arr[1] = Position(pos.getRow()+1, pos.getCol());
    arr[2] = Position(pos.getRow(), pos.getCol()-1);
    arr[3] = Position(pos.getRow()-1, pos.getCol());
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
        else continue;
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
    Position next_pos = pos;
    int min_distance = 100;
    Position arr[4];
    arr[0] = Position(pos.getRow(), pos.getCol()+1);
    arr[1] = Position(pos.getRow()+1, pos.getCol());
    arr[2] = Position(pos.getRow(), pos.getCol()-1);
    arr[3] = Position(pos.getRow()-1, pos.getCol());
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
        else continue;
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
    Position next_pos = pos;
    int min_distance = 100;
    Position arr[4];
    arr[0] = Position(pos.getRow(), pos.getCol()+2);
    arr[1] = Position(pos.getRow()+2, pos.getCol());
    arr[2] = Position(pos.getRow(), pos.getCol()-2);
    arr[3] = Position(pos.getRow()-2, pos.getCol());
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
        else continue;
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
BaseItem::~BaseItem(){};

//TODO: 3.11.1: MAGIC BOOK
bool MagicBook::canUse(Character *obj, Robot *robot){
    if(obj->getExp()<=350 && robot==nullptr){
        return true;
    }
    return false;
};
void MagicBook::use(Character *obj, Robot *robot){
    if(canUse(obj, robot)){
        obj->setExp(obj->getExp()*125/100);
    }
};
ItemType MagicBook::getType() const{
    return MAGIC_BOOK;
};
string MagicBook::str() const{
    return "MagicBook";
};

//TODO: 3.11.2: ENERGY DRINK
bool EnergyDrink::canUse(Character *obj, Robot *robot){
    if(obj->getHp()<=100 && robot==nullptr){
        return true;
    }
    return false;
};
void EnergyDrink::use(Character *obj, Robot *robot){
    if(canUse(obj, robot)){
        obj->setHp(obj->getHp()*120/100);
    }
};
ItemType EnergyDrink::getType() const{
    return ENERGY_DRINK;
};
string EnergyDrink::str() const{
    return "EnergyDrink";
};

//TODO: 3.11.3: FIRST AID
bool FirstAid::canUse(Character *obj, Robot *robot){
    if((obj->getHp()<=100 || obj->getExp()<=350) && robot==nullptr){
        return true;
    }
    return false;
};
void FirstAid::use(Character *obj, Robot *robot){
    if(canUse(obj, robot)){
        obj->setHp(obj->getHp()*150/100);
    }
};
ItemType FirstAid::getType() const{
    return FIRST_AID;
};
string FirstAid::str() const{
    return "FirstAid";
};

//TODO: 3.11.4: EXEMPTIONCARD
bool ExemptionCard::canUse(Character *obj, Robot *robot){
    if(obj->getObjectType() == SHERLOCK && obj->getHp()%2==1 && robot!=nullptr){
        return true;
    }
    return false;
};
void ExemptionCard::use(Character *obj, Robot *robot){
    if(canUse(obj, robot)){
        obj->setExp(obj->getExp());
        obj->setHp(obj->getHp());
    }
};
ItemType ExemptionCard::getType() const{
    return EXEMPTION_CARD;
};
string ExemptionCard::str() const{
    return "ExemptionCard";
};

//TODO: 3.11.5: PASSING CARD
PassingCard::PassingCard(int i, int j){
    int t = (i*11 + j)%4;
    if(t==0){
        challenge == "RobotS";
    }
    else if(t==1){
        challenge == "RobotC";
    }
    else if(t==2){
        challenge == "RobotSW";
    }
    else if(t==3){
        challenge == "all";
    }
};
bool PassingCard::canUse(Character *obj, Robot *robot){
    if(obj->getObjectType() == WATSON && obj->getExp()%2==0 && robot!=nullptr){
        return true;
    }
    else return false;
};
void PassingCard::use(Character *obj, Robot *robot){
    if(canUse(obj, robot)){
        if(challenge=="RobotS"){
            if(robot->getType()==S){
                obj->setHp(obj->getHp());
                obj->setExp(obj->getExp());
            }
        }
        else if(challenge=="RobotC"){
            if(robot->getType()==C){
                obj->setExp(obj->getExp());
                obj->setHp(obj->getHp());
            }
        }
        else if(challenge=="RobotSW"){
            if(robot->getType()==SW){
                obj->setExp(obj->getExp());
                obj->setHp(obj->getHp());
            }
        }
        else if(challenge=="all"){
            obj->setExp(obj->getExp());
            obj->setHp(obj->getHp());
        }
        else 
            obj->setHp(obj->getHp()-50);
    }
};
ItemType PassingCard::getType() const{
    return PASSING_CARD;
};
string PassingCard::str() const{
    return "PassingCard";
};

//TODO: 3.12: BASE BAG
BaseBag::BaseBag(int capacity){
    this->capacity = capacity;
    this->size = 0;
    head = nullptr;
};
BaseBag::~BaseBag(){
    Node* temp = head;
    while (temp != nullptr){
        head = head->next;
        delete temp;
        temp = head;
    }    
};
bool BaseBag::insert(BaseItem* item){};
BaseItem* BaseBag::get(){};
BaseItem* BaseBag::get(ItemType type){};
int BaseBag::getCount() const{};
string BaseBag::str() const{};

//TODO: 3.12.1: SHERLOCK BAG
SherlockBag::SherlockBag(Sherlock* sherlock) : BaseBag(13){
    this->sherlock = sherlock;
};
SherlockBag::~SherlockBag(){
    Node* temp = head;
    while (temp != nullptr){
        head = head->next;
        delete temp;
        temp = head;
    }    
};
bool SherlockBag::insert(BaseItem* item){
    if(item==NULL) return false;
    Node* temp = new Node(item);
    if(temp==NULL) return false;
    else {
        if(itemNumber<=13){
            temp->item = item;
            temp->next = head;
            head = temp;
            itemNumber++;
        }
        else return false;
    }
};
BaseItem *SherlockBag::get(){
    Node *current = head;
    while (current != NULL)
    {
        if (current->item->canUse(sherlock, NULL))
        {
            current->next = head;
            head = current;
            return current->item;
        }
        current = current->next;
    }
    return NULL;
};
BaseItem* SherlockBag::get(ItemType item_type){
    Node *current = head;
    while (current != NULL)
    {
        if (current->item->canUse(sherlock, NULL) && current->item->getType() == item_type)
        {
            return current->item;
        }
        current = current->next;
    }
    return NULL;
};
string SherlockBag::str() const{
    string str = "SherlockBag[count=" + to_string(itemNumber) + ';';
    Node *current = head;
    while (current != NULL)
    {
        if (current->item != NULL)
        {
            str += current->item->str();
        }
        current = current->next;
        if (current != NULL)
        {
            str += ",";
        }
    }
    str += "]";
    return str;
};
int SherlockBag::getCount() const{
    return itemNumber;
};

//TODO: 3.12.2: WATSON BAG
WatsonBag::WatsonBag(Watson* watson) : BaseBag(15){
    this->watson = watson;
};
WatsonBag::~WatsonBag(){
    Node* temp = head;
    while (temp != nullptr){
        head = head->next;
        delete temp;
        temp = head;
    }    
};
bool WatsonBag::insert(BaseItem* item){
    if(item==NULL) return false;
    Node* temp = new Node(item);
    if(temp==NULL) return false;
    else {
        if(itemNumber<=15){
            temp->item = item;
            temp->next = head;
            head = temp;
            itemNumber++;
        }
        else return false;
    }
};
BaseItem *WatsonBag::get(){
    Node *current = head;
    while (current != NULL)
    {
        if (current->item->canUse(watson, NULL))
        {
            current->next = head;
            head = current;
            return current->item;
        }
        current = current->next;
    }
    return NULL;
};
BaseItem* WatsonBag::get(ItemType item_type){
    Node *current = head;
    while (current != NULL)
    {
        if (current->item->canUse(watson, NULL) && current->item->getType() == item_type)
        {
            return current->item;
        }
        current = current->next;
    }
    return NULL;
};
string WatsonBag::str() const
{
    string str = "WatsonBag[count=" + to_string(itemNumber) + ';';
    Node *current = head;
    while (current != NULL)
    {
        if (current->item != NULL)
        {
            str += current->item->str();
        }
        current = current->next;
        if (current != NULL)
        {
            str += ",";
        }
    }
    str += "]";
    return str;
};
int WatsonBag::getCount() const{
    return itemNumber;
};

//TODO: 3.13: StudyPink
StudyPinkProgram::StudyPinkProgram(const string &config_file_path){
    Configuration config(config_file_path);
    map = new Map(config.map_num_rows, config.map_num_cols, config.num_walls, config.arr_walls, config.num_fake_walls, config.arr_fake_walls);
    arr_mv_objs = new ArrayMovingObject(config.max_num_moving_objects);
    sherlock = new Sherlock(0, config.sherlock_moving_rule, config.sherlock_init_pos, map, config.sherlock_init_hp, config.sherlock_init_exp);
    watson = new Watson(1, config.watson_moving_rule, config.watson_init_pos, map, config.watson_init_hp, config.watson_init_exp);
    criminal = new Criminal(2, config.criminal_init_pos, map, sherlock, watson);
    arr_mv_objs->add(sherlock);
    arr_mv_objs->add(watson);
    arr_mv_objs->add(criminal);
};
StudyPinkProgram::~StudyPinkProgram(){
    delete[] map;
    delete[] arr_mv_objs;
    delete[] config->arr_walls;
    delete[] config->arr_fake_walls;
    delete[] sherlock;
    delete[] watson;
    delete[] criminal;
};
bool StudyPinkProgram::isStop() const{
    if(sherlock->getHp() == 0 || watson->getHp() == 0 || sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition()) || watson->getCurrentPosition().isEqual(criminal->getCurrentPosition())){
        return true;
    }
    return false;
};
void StudyPinkProgram::run(bool verbose){
    for (int istep = 0; istep < config->num_steps; ++istep)
    {
        for (int i = 0; i < arr_mv_objs->size(); ++i)
        {
            MovingObject *robot = nullptr;
            if (arr_mv_objs->get(i)->getObjectType() == MovingObjectType::CRIMINAL)
            {
                robot = Robot::create(arr_mv_objs->size(), map, criminal, sherlock, watson);
            }
            arr_mv_objs->get(i)->move();
            if (robot != nullptr)
            {
                if (criminal->isCreatedRobotNext())
                {
                    arr_mv_objs->add(robot);
                }
                else
                {
                    delete robot;
                }
            }
            printResult();
        }
    }
};


//TODO: CHECK ROBOTC = getpreviousposition, fight

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////