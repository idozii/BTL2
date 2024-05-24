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
Path::Path() 
    : MapElement(PATH){
    this->type = PATH;
};
Wall::Wall() 
    : MapElement(WALL){
    this->type = WALL;
};
FakeWall::FakeWall(int in_req_exp) 
        : MapElement(FAKE_WALL){
    this->type = FAKE_WALL;
    this->req_exp = in_req_exp;
};
int FakeWall::getReqExp() const {
    return req_exp;
};

//TODO: 3.2: MAP
Map::Map(int num_rows, int num_cols, int num_walls, Position *array_walls, int num_fake_walls, Position *array_fake_walls){
    this->num_rows = num_rows;
    this->num_cols = num_cols;
    map = new MapElement **[num_rows];
    for (int i = 0; i < num_rows; i++){
        map[i] = new MapElement *[num_cols];
        for (int j = 0; j < num_cols; j++){
            map[i][j] = new Path();
            for (int k = 0; k < num_walls; k++){
                if (array_walls[k].getRow() == i && array_walls[k].getCol() == j){
                    map[i][j] = new Wall();
                }
            }
            for (int k = 0; k < num_fake_walls; k++){
                if (array_fake_walls[k].getRow() == i && array_fake_walls[k].getCol() == j){
                    map[i][j] = new FakeWall((i * 257 + j * 139 + 89) % 900 + 1);
                }
            }
        }
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
    for (int i = 0; i < num_rows; i++){
        for (int j = 0; j < num_cols; j++){
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
ElementType Map::getElementType(Position pos) const{
    return map[pos.getRow()][pos.getCol()]->getType();
};
bool Map::isValid ( const Position & pos , MovingObject * mv_obj ) const {
    if ( (pos.getRow()) < 0 || pos.getRow() >= num_rows || pos.getCol() < 0 || pos.getCol() >= num_cols ) {
        return false;
    }
    if ( getElementType(pos) == FAKE_WALL ) {
        if(mv_obj != NULL) {
            if(mv_obj->getObjectType() == SHERLOCK || mv_obj->getObjectType() == CRIMINAL || mv_obj->getObjectType() == ROBOT) return true;
            else if(mv_obj->getObjectType() == WATSON){
                if(mv_obj->getExp()>((pos.getRow()*257+pos.getCol()*139+89)%900+1)) return true;
                else return false;
            }
        }
        return false;
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
bool Position::isEqual(Position pos) const{
    if(r == pos.getRow() && c == pos.getCol()) return true;
    return false;
};
bool Position::isEqual(int in_r, int in_c) const{
    if(r == in_r && c == in_c) return true;
    return false;
};

//TODO: 3.4: MOVING OBJECT
MovingObject::MovingObject(int index, const Position pos, Map * map, const string & name){
    this->index = index;
    this->pos = pos;
    this->map = map;
    this->name = name;
};
MovingObject::~MovingObject(){};
Position MovingObject::getCurrentPosition() const{
    return this->pos;
};
int MovingObject::getExp() const{
    return this->exp;
};
int MovingObject::getHp() const{
    return this->hp;
};

//TODO: 3.5.1: CHARACTER
Character::Character(int index, const Position & init_pos, Map * map, const string & name) 
         : MovingObject(index, init_pos, map, name){
};
int Character::getExp() const{
    return this->exp;
};
void Character::setExp(int init_exp) {
    if(init_exp < 0) this->exp = 0;
    else if(init_exp > 900) this->exp = 900;
    else this->exp = init_exp;
};
int Character::getHp() const{
    return this->hp;
};
void Character::setHp(int init_hp) {
    if(init_hp < 0) this->hp = 0;
    else if(init_hp > 500) this->hp = 500;
    else this->hp = init_hp;
};

//TODO: 3.5.2: SHERLOCK
Sherlock::Sherlock(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp) 
        : Character(index, init_pos, map, "Sherlock"){
    this->hp = init_hp;
    this->exp = init_exp;
    this->index_moving_rule = 0;
    this->moving_rule = moving_rule;
    sherlockBag = new SherlockBag(this);
}; 
Sherlock::~Sherlock(){
    delete sherlockBag;
    sherlockBag = nullptr;
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
    if (map != NULL && map->isValid(next_pos, this)) return next_pos;
    return Position::npos;
};
void Sherlock::move(){
    if(this->getExp()==0) return;  
    Position next_pos = getNextPosition();
    if (next_pos.isEqual(Position::npos)) return;
    pos = next_pos;    
};
Type Sherlock::getObjectType()const{
    return SHERLOCK;
};
BaseBag* Sherlock::getSherlockBag() const{
    return sherlockBag;
};
string Sherlock::str() const {
    return "Sherlock[index="+to_string(index)+";pos="+pos.str()+";moving_rule="+moving_rule+"]";
};
int Sherlock::getHp() const {
    return this->hp;
};
int Sherlock::getExp() const {
    return this->exp;
};
void Sherlock::setExp(int EXP) {
    if(EXP < 0) this->exp = 0;
    else if(EXP > 900) this->exp = 900;
    else this->exp = EXP;
};
void Sherlock::setHp(int HP) {
    if(HP < 0) this->hp = 0;
    else if(HP > 500) this->hp = 500;
    else this->hp = HP;
};
void Sherlock::setPos(Position POS){
    this->pos = POS;
};
bool Sherlock::meet(RobotS* robotS){
    if (pos.isEqual(robotS->getCurrentPosition())) {
        if (exp > 400){
            sherlockBag->insert(robotS->NewItem());
            delete robotS;
            return false;
        }
        else{
            if(sherlockBag->checkItem(EXCEMPTION_CARD) > 0){
                sherlockBag->insert(robotS->NewItem());
                sherlockBag->get(EXCEMPTION_CARD);
                return false;
            }
            else{
                exp = exp*90/100;
                if(sherlockBag->checkItem(ENERGY_DRINK) > 0) {
                    sherlockBag->get(ENERGY_DRINK);
                    return false;
                }
                else if(sherlockBag->checkItem(FIRST_AID) > 0) {
                    sherlockBag->get(FIRST_AID);
                    return false;
                }
                else if(sherlockBag->checkItem(MAGIC_BOOK) > 0) {
                    sherlockBag->get(MAGIC_BOOK);
                    return false;
                }
            }
        }
    }
    return false;
};
bool Sherlock::meet(RobotC* robotC){
    if (pos.isEqual(robotC->getCurrentPosition())) {
        if (exp > 500){
            this->setPos(robotC->getCrimePosition());
            delete robotC;
            return true;
        }
        else {
            sherlockBag->insert(robotC->NewItem());
            delete robotC;
            return false;
        }
    }
    return false;
};
bool Sherlock::meet(RobotSW* robotSW){
    if (pos.isEqual(robotSW->getCurrentPosition())) {
        if (exp > 300 && hp > 335){
            sherlockBag->insert(robotSW->NewItem());
            delete robotSW;
            return false;
        }
        else{
            if(sherlockBag->checkItem(EXCEMPTION_CARD) > 0){
                sherlockBag->insert(robotSW->NewItem());
                sherlockBag->get(EXCEMPTION_CARD);
                return false;
            }
            else{
                exp = exp*0.85;
                hp = hp*0.85;
                if(sherlockBag->checkItem(ENERGY_DRINK) > 0) {
                    sherlockBag->get(ENERGY_DRINK);
                    return false;
                }
                else if(sherlockBag->checkItem(FIRST_AID) > 0) {
                    sherlockBag->get(FIRST_AID);
                    return false;
                }
                else if(sherlockBag->checkItem(MAGIC_BOOK) > 0) {
                    sherlockBag->get(MAGIC_BOOK);
                    return false;
                }
            }
        }
    }
    return false;
};
bool Sherlock::meet(RobotW* robotW){
    if (pos.isEqual(robotW->getCurrentPosition())) {
        sherlockBag->insert(robotW->NewItem());
        delete robotW;
        return false;
    }
    return false;
};
bool Sherlock::meet(Watson* watson){
    if(pos.isEqual(watson->getCurrentPosition())){
        while(watson->getWatsonBag()->checkItem(EXCEMPTION_CARD) > 0 && sherlockBag->checkItem(PASSING_CARD) > 0) {
            watson->getWatsonBag()->insert(sherlockBag->get(PASSING_CARD));
            sherlockBag->insert(watson->getWatsonBag()->get(EXCEMPTION_CARD));
        }
        return false;
    }
    return false;
};

//TODO: 3.6: WATSON
Watson::Watson(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp) 
      : Character(index, init_pos, map, "Watson"){
    this->hp = init_hp;
    this->exp = init_exp;
    this->index_moving_rule = 0;
    this->moving_rule = moving_rule; 
    watsonBag = new WatsonBag(this);
};
Watson::~Watson(){
    delete watsonBag;
    watsonBag = nullptr;
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
    if (map != NULL && map->isValid(next_pos, this)) return next_pos;
    return Position::npos;
};
void Watson::move(){
    if(this->getExp()==0) return;
    Position next_pos = getNextPosition();
    if (next_pos.isEqual(Position::npos)) return;
    pos = next_pos;
};
Type Watson::getObjectType() const {
    return WATSON;
};
BaseBag* Watson::getWatsonBag() const{
    return watsonBag;
};
string Watson::str() const {
    return "Watson[index="+to_string(index)+";pos="+pos.str()+";moving_rule="+moving_rule+"]";
};
int Watson::getExp() const {
    return this->exp;
};
int Watson::getHp() const {
    return this->hp;
};
void Watson::setExp(int EXP) {
    if(EXP < 0) this->exp = 0;
    else if(EXP > 900) this->exp = 900;
    else this->exp = EXP;
};
void Watson::setHp(int HP) {
    if(HP < 0) this->hp = 0;
    else if(HP > 500) this->hp = 500;
    else this->hp = HP;
};
bool Watson::meet(RobotS* robotS){
    if (pos.isEqual(robotS->getCurrentPosition())){
        delete robotS;
        return false;
    }
    return false;
};
bool Watson::meet(RobotC* robotC){
    if (pos.isEqual(robotC->getCurrentPosition())){
        if(watsonBag->checkItem(PASSING_CARD) > 0){
            watsonBag->insert(robotC->NewItem());
            watsonBag->get(PASSING_CARD);
            return false;
        }
        watsonBag->insert(robotC->NewItem());
        delete robotC;
        return false;
    }
    return false;
};
bool Watson::meet(RobotSW* robotSW){
    if (pos.isEqual(robotSW->getCurrentPosition())){
        if(watsonBag->checkItem(PASSING_CARD) > 0){
            watsonBag->insert(robotSW->NewItem());
            watsonBag->get(PASSING_CARD);
            return false;
        }
        else{
            if(exp > 600 && hp > 165){
                watsonBag->insert(robotSW->NewItem());
                delete robotSW;
                return false;
            }
            else{
                hp = hp*0.85;
                exp = exp*0.85;
                if(watsonBag->checkItem(ENERGY_DRINK) > 0) {
                    watsonBag->get(ENERGY_DRINK);
                    return false;
                }
                else if(watsonBag->checkItem(FIRST_AID) > 0) {
                    watsonBag->get(FIRST_AID);
                    return false;
                }
                else if(watsonBag->checkItem(MAGIC_BOOK) > 0) {
                    watsonBag->get(MAGIC_BOOK);
                    return false;
                }
            }
        }
    }
    return false;
};
bool Watson::meet(RobotW* robotW){
    if (pos.isEqual(robotW->getCurrentPosition())){
        if(watsonBag->checkItem(PASSING_CARD) > 0){
            watsonBag->insert(robotW->NewItem());
            watsonBag->get(PASSING_CARD);
            return false;
        }
        else{
            if (hp > 350){
                watsonBag->insert(robotW->NewItem());
                delete robotW;
                return false;
            }
            else{
                hp = hp*0.95;
                if(watsonBag->checkItem(ENERGY_DRINK) > 0) {
                    watsonBag->get(ENERGY_DRINK);
                    return false;
                }
                else if(watsonBag->checkItem(FIRST_AID) > 0) {
                    watsonBag->get(FIRST_AID);
                    return false;
                }
                else if(watsonBag->checkItem(MAGIC_BOOK) > 0) {
                    watsonBag->get(MAGIC_BOOK);
                    return false;
                }
            }
        }
    }
    return false;
};
bool Watson::meet(Sherlock* sherlock){
    if(pos.isEqual(sherlock->getCurrentPosition())){
        while(watsonBag->checkItem(EXCEMPTION_CARD) > 0 && sherlock->getSherlockBag()->checkItem(PASSING_CARD) > 0) {
            watsonBag->insert(sherlock->getSherlockBag()->get(PASSING_CARD));
            sherlock->getSherlockBag()->insert(watsonBag->get(EXCEMPTION_CARD));
        }
        return false;
    }
    return false;
};

//TODO: 3.7: CRIMINAL
Criminal::Criminal(int index, const Position & init_pos, Map * map, Sherlock * sherlock, Watson * watson) 
        : Character(index, init_pos, map, "Criminal"){
    this->sherlock = sherlock;
    this->watson = watson;
    this->count = 0;
};
Position Criminal::getPrevPos() const{
    if(count == 0) return Position::npos;
    return this->prev_pos;
};
Position Criminal::getNextPosition() {
    Position next_pos = pos;
    prev_pos = pos;
    int max_distance = -1;
    Position arr[4];
    arr[0] = Position(pos.getRow() - 1, pos.getCol());
    arr[1] = Position(pos.getRow(), pos.getCol() - 1);
    arr[2] = Position(pos.getRow() + 1, pos.getCol());
    arr[3] = Position(pos.getRow(), pos.getCol() + 1);
    for (int i = 0; i < 4; i++){
        if (map != NULL && map->isValid(arr[i], this)){
            if(sherlock != NULL && watson != NULL) {
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
    }
    return next_pos;
};
void Criminal::move(){
    Position next_pos = getNextPosition();
    if (next_pos.isEqual(Position::npos)) return;
    pos = next_pos;
    count++;
};
Type Criminal::getObjectType() const{
    return CRIMINAL;
}
string Criminal::str() const {
    return "Criminal[index="+to_string(index)+";pos="+pos.str()+"]";
};
int Criminal::getCount() const {
    return this->count;
};
bool Criminal::isCreatedRobotNext() const {
    if (getCount() % 3 == 0 && getCount()>0) return true;
    return false;
};

//TODO: 3.8: ARRAY MOVING OBJECT
ArrayMovingObject::ArrayMovingObject(int capacity){
    this->capacity = capacity;
    count = 0;
    arr_mv_objs = new MovingObject*[capacity];
    for(int i = 0; i < capacity; i++){
        arr_mv_objs[i] = nullptr;
    }
};
ArrayMovingObject::~ArrayMovingObject(){
    if(arr_mv_objs!=NULL){
        delete arr_mv_objs;
        arr_mv_objs = nullptr;
    }
};
bool ArrayMovingObject::isFull() const {
    if(count==capacity) return true;
    return false;
};
bool ArrayMovingObject::add(MovingObject* mv_obj){
    if(arr_mv_objs != NULL && !isFull()) {
        for(int i = 0; i < capacity; i++){
            if(arr_mv_objs[i]==NULL){
                arr_mv_objs[i] = mv_obj;
                count++;
                return true;
            }
        }
    }
    return false;
};
MovingObject* ArrayMovingObject::get(int index) const{
    if(index >= 0 && index < size() && arr_mv_objs[index]!=NULL) return arr_mv_objs[index];
    return NULL;
};
int ArrayMovingObject::size() const{
    return count;
};
string ArrayMovingObject::str() const{
    string arraymovingobject;
    int i = 0;
    arraymovingobject = "ArrayMovingObject[count="+to_string(count)+";capacity="+to_string(capacity)+";";
    for (int i = 0; i < count; i++){
        arraymovingobject += get(i)->str();
        if (i < count - 1)
            arraymovingobject += ';';
    }
    arraymovingobject += ']';
    return arraymovingobject;
};
bool ArrayMovingObject::checkMeet(int index) const {
    if(arr_mv_objs[index]->getObjectType() == SHERLOCK){
        for(int i = 0; i < count; i++){
            if(arr_mv_objs[i]->getObjectType() == CRIMINAL){
                if(arr_mv_objs[i]->getCurrentPosition().isEqual(arr_mv_objs[index]->getCurrentPosition())){
                    return true;
                }
            }
            else if(arr_mv_objs[i]->getObjectType() == ROBOT){
                if(arr_mv_objs[i]->getCurrentPosition().isEqual(arr_mv_objs[index]->getCurrentPosition())){
                    Sherlock* sherlock = dynamic_cast<Sherlock*>(arr_mv_objs[index]);
                    if(dynamic_cast<RobotC*>(arr_mv_objs[i]) != NULL){
                        RobotC* robotC = dynamic_cast<RobotC*>(arr_mv_objs[i]);
                        sherlock->meet(robotC);
                    }
                    else if(dynamic_cast<RobotS*>(arr_mv_objs[i]) != NULL){
                        RobotS* robotS = dynamic_cast<RobotS*>(arr_mv_objs[i]);
                        sherlock->meet(robotS);
                    }
                    else if(dynamic_cast<RobotSW*>(arr_mv_objs[i]) != NULL){
                        RobotSW* robotSW = dynamic_cast<RobotSW*>(arr_mv_objs[i]);
                        sherlock->meet(robotSW);
                    }
                    else if(dynamic_cast<RobotW*>(arr_mv_objs[i]) != NULL){
                        RobotW* robotW = dynamic_cast<RobotW*>(arr_mv_objs[i]);
                        sherlock->meet(robotW);
                    }
                }
            }
            else if(arr_mv_objs[i]->getObjectType() == WATSON){
                if(arr_mv_objs[i]->getCurrentPosition().isEqual(arr_mv_objs[index]->getCurrentPosition())){
                    Sherlock* sherlock = dynamic_cast<Sherlock*>(arr_mv_objs[index]);
                    Watson* watson = dynamic_cast<Watson*>(arr_mv_objs[i]);
                    sherlock->meet(watson);
                }
            }
        }
    }
    else if(arr_mv_objs[index]->getObjectType() == WATSON){
        for(int i = 0; i < count; i++){
            if(arr_mv_objs[i]->getObjectType() == CRIMINAL){
                if(arr_mv_objs[i]->getCurrentPosition().isEqual(arr_mv_objs[index]->getCurrentPosition())){
                    return true;
                }
            }
            else if(arr_mv_objs[i]->getObjectType() == ROBOT){
                if(arr_mv_objs[i]->getCurrentPosition().isEqual(arr_mv_objs[index]->getCurrentPosition())){
                    Watson* watson = dynamic_cast<Watson*>(arr_mv_objs[index]);
                    if(dynamic_cast<RobotC*>(arr_mv_objs[i]) != NULL){
                        RobotC* robotC = dynamic_cast<RobotC*>(arr_mv_objs[i]);
                        watson->meet(robotC);
                    }
                    else if(dynamic_cast<RobotS*>(arr_mv_objs[i]) != NULL){
                        RobotS* robotS = dynamic_cast<RobotS*>(arr_mv_objs[i]);
                        watson->meet(robotS);
                    }
                    else if(dynamic_cast<RobotSW*>(arr_mv_objs[i]) != NULL){
                        RobotSW* robotSW = dynamic_cast<RobotSW*>(arr_mv_objs[i]);
                        watson->meet(robotSW);
                    }
                    else if(dynamic_cast<RobotW*>(arr_mv_objs[i]) != NULL){
                        RobotW* robotW = dynamic_cast<RobotW*>(arr_mv_objs[i]);
                        watson->meet(robotW);
                    }
                }
            }
            else if(arr_mv_objs[i]->getObjectType() == SHERLOCK){
                if(arr_mv_objs[i]->getCurrentPosition().isEqual(arr_mv_objs[index]->getCurrentPosition())){
                    Watson* watson = dynamic_cast<Watson*>(arr_mv_objs[index]);
                    Sherlock* sherlock = dynamic_cast<Sherlock*>(arr_mv_objs[i]);
                    watson->meet(sherlock);
                }
            }
        } 
    }
    else if(arr_mv_objs[index]->getObjectType() == CRIMINAL){
        for(int i = 0; i < count; i++){
            if(arr_mv_objs[i]->getObjectType() == SHERLOCK){
                if(arr_mv_objs[i]->getCurrentPosition().isEqual(arr_mv_objs[index]->getCurrentPosition())){
                    return true;
                }
            }
            else if(arr_mv_objs[i]->getObjectType() == WATSON){
                if(arr_mv_objs[i]->getCurrentPosition().isEqual(arr_mv_objs[index]->getCurrentPosition())){
                    return true;
                }
            }
            else if(arr_mv_objs[i]->getObjectType() == ROBOT){
                if(arr_mv_objs[i]->getCurrentPosition().isEqual(arr_mv_objs[index]->getCurrentPosition())){
                    return false;
                }
            }
        }
    }
    else if(arr_mv_objs[index]->getObjectType() == ROBOT){
        for(int i = 0; i < count; i++){
            if(arr_mv_objs[i]->getObjectType() == SHERLOCK){
                if(arr_mv_objs[i]->getCurrentPosition().isEqual(arr_mv_objs[index]->getCurrentPosition())){
                    Sherlock* sherlock = dynamic_cast<Sherlock*>(arr_mv_objs[index]); 
                    if(dynamic_cast<RobotC*>(arr_mv_objs[i]) != NULL){
                        RobotC* robotC = dynamic_cast<RobotC*>(arr_mv_objs[i]);
                        sherlock->meet(robotC);
                    }
                    else if(dynamic_cast<RobotS*>(arr_mv_objs[i]) != NULL){
                        RobotS* robotS = dynamic_cast<RobotS*>(arr_mv_objs[i]);
                        sherlock->meet(robotS);
                    }
                    else if(dynamic_cast<RobotSW*>(arr_mv_objs[i]) != NULL){
                        RobotSW* robotSW = dynamic_cast<RobotSW*>(arr_mv_objs[i]);
                        sherlock->meet(robotSW);
                    }
                    else if(dynamic_cast<RobotW*>(arr_mv_objs[i]) != NULL){
                        RobotW* robotW = dynamic_cast<RobotW*>(arr_mv_objs[i]);
                        sherlock->meet(robotW);
                    }
                }
            }
            else if(arr_mv_objs[i]->getObjectType() == WATSON){
                if(arr_mv_objs[i]->getCurrentPosition().isEqual(arr_mv_objs[index]->getCurrentPosition())){
                    Watson* watson = dynamic_cast<Watson*>(arr_mv_objs[index]);
                    if(dynamic_cast<RobotC*>(arr_mv_objs[i]) != NULL){
                        RobotC* robotC = dynamic_cast<RobotC*>(arr_mv_objs[i]);
                        watson->meet(robotC);
                    }
                    else if(dynamic_cast<RobotS*>(arr_mv_objs[i]) != NULL){
                        RobotS* robotS = dynamic_cast<RobotS*>(arr_mv_objs[i]);
                        watson->meet(robotS);
                    }
                    else if(dynamic_cast<RobotSW*>(arr_mv_objs[i]) != NULL){
                        RobotSW* robotSW = dynamic_cast<RobotSW*>(arr_mv_objs[i]);
                        watson->meet(robotSW);
                    }
                    else if(dynamic_cast<RobotW*>(arr_mv_objs[i]) != NULL){
                        RobotW* robotW = dynamic_cast<RobotW*>(arr_mv_objs[i]);
                        watson->meet(robotW);
                    }
                }
            }
            else if(arr_mv_objs[i]->getObjectType() == CRIMINAL){
                if(arr_mv_objs[i]->getCurrentPosition().isEqual(arr_mv_objs[index]->getCurrentPosition())){
                    return false;
                }
            }
            else if(arr_mv_objs[i]->getObjectType() == ROBOT){
                if(arr_mv_objs[i]->getCurrentPosition().isEqual(arr_mv_objs[index]->getCurrentPosition())){
                    return false;
                }
            }
        }
    }
    return false;
};

//TODO: 3.9: CONFIGURATION
Configuration::Configuration(const string & filepath){
    string line;
    ifstream ifs(filepath);
    for (int i = 0; i < 15 && !ifs.eof(); i++){
        ifs >> line;
        if (line.find("MAP_NUM_ROWS") == 0)
        {
            map_num_rows = stoi(line.substr(13, line.length() - 13));
            configString[0][0] = "MAP_NUM_ROWS=";
            configString[1][0] = to_string(map_num_rows);
        }
        else if (line.find("MAP_NUM_COLS") == 0)
        {
            map_num_cols = stoi(line.substr(13, line.length() - 13));
            configString[0][1] = "MAP_NUM_COLS=";
            configString[1][1] = to_string(map_num_cols);
        }
        else if (line.find("MAX_NUM_MOVING_OBJECTS") == 0)
        {
            max_num_moving_objects = stoi(line.substr(23, line.length() - 23));
            configString[0][2] = "MAX_NUM_MOVING_OBJECTS=";
            configString[1][2] = to_string(max_num_moving_objects);
        }
        else if (line.find("ARRAY_WALLS") == 0)
        {
            num_walls = (line.length() - 13) / 6;
            configString[0][3] = "NUM_WALLS=";
            configString[1][3] = to_string(num_walls);
            configString[0][4] = "ARRAY_WALLS=";
            configString[1][4] = line.substr(12, line.length() - 12);
            if(num_walls > 0){
                configString[0][3] = "NUM_WALLS=";
                configString[1][3] = to_string(num_walls);
                configString[0][4] = "ARRAY_WALLS=";
                configString[1][4] = line.substr(12, line.length() - 12);
                arr_walls = new Position[num_walls];
                for (int k = 0; k < num_walls; k++)
                {
                    char c;
                    int temp1, temp2;
                    istringstream temp(line.substr(14 + k * 6, 3));
                    temp >> temp1 >> c >> temp2;
                    arr_walls[k] = Position(temp1, temp2);
                }
            }
        }
        else if (line.find("ARRAY_FAKE_WALLS") == 0)
        {
            num_fake_walls = (line.length() - 18) / 6;
            configString[0][5] = "NUM_FAKE_WALLS=";
            configString[1][5] = to_string(num_fake_walls);
            configString[0][6] = "ARRAY_FAKE_WALLS=";
            configString[1][6] = line.substr(17, line.length() - 17);
            if(num_fake_walls > 0){
                configString[0][5] = "NUM_FAKE_WALLS=";
                configString[1][5] = to_string(num_fake_walls);
                configString[0][6] = "ARRAY_FAKE_WALLS=";
                configString[1][6] = line.substr(17, line.length() - 17);
                arr_fake_walls = new Position[num_fake_walls];
                for (int k = 0; k < num_fake_walls; k++)
                {
                    char c;
                    int temp1, temp2;
                    istringstream temp(line.substr(19 + k * 6, 3));
                    temp >> temp1 >> c >> temp2;
                    arr_fake_walls[k] = Position(temp1, temp2);
                }
            }
        }
        else if (line.find("SHERLOCK_MOVING_RULE") == 0)
        {
            sherlock_moving_rule = line.substr(21, line.length() - 21);
            configString[0][7] = "SHERLOCK_MOVING_RULE=";
            configString[1][7] = sherlock_moving_rule;
        }
        else if (line.find("SHERLOCK_INIT_POS") == 0)
        {
            configString[0][8] = "SHERLOCK_INIT_POS=";
            configString[1][8] = line.substr(18, line.length() - 18);
            char c;
            int temp1, temp2;
            istringstream temp(line.substr(19, 3));
            temp >> temp1 >> c >> temp2;
            sherlock_init_pos = Position(temp1, temp2);
        }
        else if (line.find("SHERLOCK_INIT_HP") == 0)
        {
            sherlock_init_hp = stoi(line.substr(17, line.length() - 17));
            configString[0][9] = "SHERLOCK_INIT_HP=";
            configString[1][9] = to_string(sherlock_init_hp);
        }
        else if (line.find("SHERLOCK_INIT_EXP") == 0)
        {
            sherlock_init_exp = stoi(line.substr(18, line.length() - 18));
            configString[0][10] = "SHERLOCK_INIT_EXP=";
            configString[1][10] = to_string(sherlock_init_exp);
        }
        else if (line.find("WATSON_MOVING_RULE") == 0)
        {
            watson_moving_rule = line.substr(19, line.length() - 19);
            configString[0][11] = "WATSON_MOVING_RULE=";
            configString[1][11] = watson_moving_rule;
        }
        else if (line.find("WATSON_INIT_POS") == 0)
        {
            configString[0][12] = "WATSON_INIT_POS=";
            configString[1][12] = line.substr(16, line.length() - 16);
            char c;
            int temp1, temp2;
            istringstream temp(line.substr(17, 3));
            temp >> temp1 >> c >> temp2;
            watson_init_pos = Position(temp1, temp2);
        }
        else if (line.find("WATSON_INIT_HP") == 0)
        {
            watson_init_hp = stoi(line.substr(15, line.length() - 15));
            configString[0][13] = "WATSON_INIT_HP=";
            configString[1][13] = to_string(watson_init_hp);
        }
        else if (line.find("WATSON_INIT_EXP") == 0)
        {
            watson_init_exp = stoi(line.substr(16, line.length() - 16));
            configString[0][14] = "WATSON_INIT_EXP=";
            configString[1][14] = to_string(watson_init_exp);
        }
        else if (line.find("CRIMINAL_INIT_POS") == 0)
        {
            configString[0][15] = "CRIMINAL_INIT_POS=";
            configString[1][15] = line.substr(18, line.length() - 18);
            char c;
            int temp1, temp2;
            istringstream temp(line.substr(19, 3));
            temp >> temp1 >> c >> temp2;
            criminal_init_pos = Position(temp1, temp2);
        }
        else if (line.find("NUM_STEPS") == 0)
        {
            num_steps = stoi(line.substr(10, line.length() - 10));
            configString[0][16] = "NUM_STEPS=";
            configString[1][16] = to_string(num_steps);
        }
    }
};
Configuration::~Configuration(){
    delete[] arr_fake_walls;
    delete[] arr_walls;
    arr_fake_walls = nullptr;
    arr_walls = nullptr;
};
string Configuration::str() const{
    string arr;
    arr = configString[0][0] + configString[1][0] + "\n";
    for (int i = 1; i < 17; i++)
    {
        arr += configString[0][i] + configString[1][i] + "\n";
    }
    return "Configuration[\n" + arr + ']';
};

//TODO: 3.10: ROBOT
Robot::Robot(int index , const Position &pos , Map * map , Criminal* criminal, const string &name) 
     : MovingObject(index, pos, map, "Robot"){
    this->criminal = criminal;
    this->robot_type = robot_type;
    int p = this->getCurrentPosition().getRow() * this->getCurrentPosition().getCol();
    while(p > 9) {
        int sum = 0;
        while(p > 0) {
            sum += p % 10;
            p /= 10;
        }
        p = sum;
    }  
    if(p == 0 || p == 1){
        item = new MagicBook();
    }
    else if(p == 2 || p == 3){
        item = new EnergyDrink();
    }
    else if(p == 4 || p == 5){
        item = new FirstAid();
    }
    else if(p == 6 || p == 7){
        item = new ExcemptionCard();
    }
    else if(p == 8 || p == 9){
        int t = (this->getCurrentPosition().getRow() * 11 + this->getCurrentPosition().getCol()) % 4;
        if(t==0){
            item = new PassingCard("RobotS");
        }
        else if(t==1){
            item = new PassingCard("RobotC");
        }
        else if(t==2){
            item = new PassingCard("RobotSW");
        }
        else if(t==3){
            item = new PassingCard("all");
        }
        else item = NULL;
    }
    else item = NULL;
};
Robot::~Robot(){
    delete item;
    item = nullptr;
};
Robot* Robot::create(int index, Map* map, Criminal* criminal, Sherlock* sherlock, Watson* watson){
    if(criminal->isCreatedRobotNext()){
        return new RobotC(index, criminal->getCurrentPosition(), map, criminal);
    }    
    else if(distance(criminal->getCurrentPosition(), sherlock->getCurrentPosition()) > distance(criminal->getCurrentPosition(), watson->getCurrentPosition())){
        return new RobotS(index, criminal->getCurrentPosition(), map, criminal, sherlock);
    }
    else if(distance(criminal->getCurrentPosition(), sherlock->getCurrentPosition()) < distance(criminal->getCurrentPosition(), watson->getCurrentPosition())){
        return new RobotW(index, criminal->getCurrentPosition(), map, criminal, watson);
    }
    else if(distance(criminal->getCurrentPosition(), sherlock->getCurrentPosition()) == distance(criminal->getCurrentPosition(), watson->getCurrentPosition())){
        return new RobotSW(index, criminal->getCurrentPosition(), map, criminal, sherlock, watson);
    }
    else return NULL;
};
Type Robot::getObjectType() const{
    return ROBOT;
};
RobotType Robot::getType(){
    return robot_type;
};
BaseItem* Robot::NewItem(){
    if(item == NULL) return NULL;
    return item;
};
string Robot::str() const{
    return "Robot[pos="+pos.str()+";type=;dist="+to_string(getDistance())+"]";
};

//TODO: 3.10.1: ROBOTC
RobotC::RobotC(int index, const Position & init_pos, Map* map, Criminal* criminal) 
      : Robot(index, init_pos, map, criminal, "RobotC"){
};
Position RobotC::getNextPosition() {
    return criminal->getPrevPos();
};
Position RobotC::getCrimePosition() {
    return criminal->getCurrentPosition();
};
void RobotC::move(){
    Position next_pos = getNextPosition();
    if (next_pos.isEqual(Position::npos)) return;
    pos = next_pos;
};
RobotType RobotC::getType() const{
    return C;
}
int RobotC::getDistance(Sherlock* sherlock){
    return distance(pos, sherlock->getCurrentPosition());
};
int RobotC::getDistance(Watson* watson){
    return distance(pos, watson->getCurrentPosition());
};
int RobotC::getDistance() const{
    return distance(pos, criminal->getCurrentPosition());
};
string RobotC::str() const{
    return "Robot[pos="+pos.str()+";type=C;dist="+""+"]";
};

//TODO:3.10.2: ROBOT S
RobotS::RobotS(int index, const Position & init_pos, Map* map, Criminal* criminal, Sherlock* sherlock) 
      : Robot(index, init_pos, map, criminal, "RobotS"){
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
    return "Robot[pos="+pos.str()+";type=S;dist="+to_string(getDistance())+"]";
};

//TODO:3.10.3: ROBOT W
RobotW::RobotW(int index, const Position & init_pos, Map* map, Criminal* criminal, Watson* watson) 
      : Robot(index, init_pos, map, criminal, "RobotW"){
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
    return "Robot[pos="+pos.str()+";type=W;dist="+to_string(getDistance())+"]";
};

//TODO:3.10.4: ROBOT SW
RobotSW::RobotSW(int index, const Position & init_pos, Map* map, Criminal* criminal, Sherlock* sherlock, Watson* watson) 
       : Robot(index, init_pos, map, criminal, "RobotSW"){
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
    return "Robot[pos="+pos.str()+";type=SW;dist="+to_string(getDistance())+"]";
};

//TODO: 3.11: BASE ITEM
BaseItem::BaseItem(){};
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

//TODO: 3.11.4: EXCEMPTIONCARD
bool ExcemptionCard::canUse(Character *obj, Robot *robot){
    if(obj->getObjectType() == SHERLOCK && obj->getHp()%2==1 && robot!=nullptr){
        return true;
    }
    return false;
};
void ExcemptionCard::use(Character *obj, Robot *robot){
    if(canUse(obj, robot)){
        obj->setExp(obj->getExp());
        obj->setHp(obj->getHp());
    }
};
ItemType ExcemptionCard::getType() const{
    return EXCEMPTION_CARD;
};
string ExcemptionCard::str() const{
    return "ExcemptionCard";
};

//TODO: 3.11.5: PASSING CARD
PassingCard::PassingCard(string challenge){
    this->challenge = challenge;
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
        else obj->setHp(obj->getHp()-50);
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
    size = 0;
    head = nullptr;
};
BaseBag::~BaseBag(){
    Node* temp = head;
    while (temp != nullptr){
        head = head->next;
        delete temp->item;
        delete temp;
        temp = head;
    }
    head = nullptr;
};
bool BaseBag::insert(BaseItem* item){
    if(isFull()) return false;
    if(item==NULL) return false;
    Node* temp = new Node(item);
    if(temp==NULL) return false;
    else {
        if(size<capacity){
            temp->item = item;
            temp->next = head;
            head = temp;
            size++;
        }
        else return false;
    }
    return true;
};
string BaseBag::str() const{
    string bag = "Bag[count=" + to_string(size) + ";";
    Node *current = head;
    while (current != NULL)
    {
        bag += current->item->str();
        if (current->next != NULL)
        {
            bag += ',';
        }
        current = current->next;
    }
    bag += "]";
    return bag;
};
BaseItem* BaseBag::get(ItemType type){
    Node *current = head;
    Node *prev = nullptr;
    while (current != NULL)
    {
        if (current->item->getType() == type)
        {
            if (prev == nullptr)
            {
                head = current->next;
            }
            else
            {
                prev->next = current->next;
            }
            BaseItem *temp = current->item;
            delete current;
            size--;
            return temp;
        }
        prev = current;
        current = current->next;
    }
    return nullptr;
};
bool BaseBag::isFull() const{
    if(size==capacity) return true;
    return false;
};
bool BaseBag::checkItem(ItemType type) {
    Node *current = head;
    while (current != NULL)
    {
        if (current->item->getType() == type)
        {
            return true;
        }
        current = current->next;
    }
    return false;
};

//TODO: 3.12.1: SHERLOCK BAG
SherlockBag::SherlockBag(Sherlock* sherlock) 
           : BaseBag(13){
    this->sherlock = sherlock;
};
BaseItem *SherlockBag::get(){
    Node *current = head;
    Node *prev = NULL;
    while (current != NULL) {
        if (current->item->canUse(sherlock, NULL)) {
            if (prev != NULL) {
                prev->next = current->next;
            } else {
                head = current->next;
            }
            BaseItem *temp = current->item;
            delete current;
            size--;
            return temp;
        }
        prev = current;
        current = current->next;
    }
    return NULL;
};

//TODO: 3.12.2: WATSON BAG
WatsonBag::WatsonBag(Watson* watson) 
         : BaseBag(15){
    this->watson = watson;
};
BaseItem *WatsonBag::get(){
    Node *current = head;
    Node *prev = NULL;
    while (current != NULL) {
        if (current->item->canUse(watson, NULL)) {
            if (prev != NULL) {
                prev->next = current->next;
            } else {
                head = current->next;
            }
            BaseItem *temp = current->item;
            delete current;
            size--;
            return temp;
        }
        prev = current;
        current = current->next;
    }
    return NULL;
};
//TODO: 3.13: StudyPink
StudyPinkProgram::StudyPinkProgram(const string &config_file_path){
    config = new Configuration(config_file_path);
    map = new Map(config->map_num_rows, config->map_num_cols, config->num_walls, config->arr_walls, config->num_fake_walls, config->arr_fake_walls);
    arr_mv_objs = new ArrayMovingObject(config->max_num_moving_objects);
    sherlock = new Sherlock(1, config->sherlock_moving_rule, config->sherlock_init_pos, map, config->sherlock_init_hp, config->sherlock_init_exp);
    watson = new Watson(2, config->watson_moving_rule, config->watson_init_pos, map, config->watson_init_hp, config->watson_init_exp);
    criminal = new Criminal(0, config->criminal_init_pos, map, sherlock, watson);
    arr_mv_objs->add(criminal);
    arr_mv_objs->add(sherlock);
    arr_mv_objs->add(watson);
};
StudyPinkProgram::~StudyPinkProgram(){
    delete[] map;
    delete[] arr_mv_objs;
    delete[] config;
    map = nullptr;
    arr_mv_objs = nullptr;
    config = nullptr;
};
bool StudyPinkProgram::isStop() const{
    if(sherlock->getHp() == 0 || watson->getHp() == 0 || sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition()) || watson->getCurrentPosition().isEqual(criminal->getCurrentPosition())){
        return true;
    }
    return false;
};
void StudyPinkProgram::run(bool verbose){
    for (int istep = 0; istep < config->num_steps; ++istep) {
        for (int i = 0; i < arr_mv_objs->size(); ++i) {
            arr_mv_objs->get(i)->move();
            if (arr_mv_objs->checkMeet(i)) {
                printStep(istep);
                break;
            }
            if (isStop()) {
                printStep(istep);
                break;
            }
            if(criminal->isCreatedRobotNext()){
                Robot *robot = Robot::create(arr_mv_objs->size(), map, criminal, sherlock, watson);
                if (robot != NULL) {
                    arr_mv_objs->add(robot);
                }
            }
            if (verbose) {
                printStep(istep);
            }
        }
        if (isStop()) {
            break;
        }
    }
    printResult();
};
void StudyPinkProgram::printResult() const {
    if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition())) {
        cout << "Sherlock caught the criminal" << endl;
    }
    else if (watson->getCurrentPosition().isEqual(criminal->getCurrentPosition())) {
        cout << "Watson caught the criminal" << endl;
    }
    else {
        cout << "The criminal escaped" << endl;
    }
};
void StudyPinkProgram::printStep(int si) const {
    cout << "Step: " << setw(4) << setfill('0') << si
        << "--"
        << sherlock->str() << "--|--" << watson->str() << "--|--" << criminal->str() << endl;
};



////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////