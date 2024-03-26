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
    if ( pos.getRow() < 0 || pos.getRow() >= num_rows || pos.getCol() < 0 || pos.getCol() >= num_cols ) {
        return false;
    }
    if ( map[pos.getRow()][pos.getCol()] == NULL ) {
        return false;
    }
    if ( map[pos.getRow()][pos.getCol()]->getType() == FAKE_WALL ) {
        if(mv_obj->getName() == "Sherlock" || mv_obj->getName() == "Criminal") return true;
        else if(mv_obj->getName() == "Watson"){
            if(((pos.getRow()*257+pos.getCol()*139+89)%900+1)) return true;
            //TODO:CHECK WATSON EXP
            else return false;
        }
        else return false;
    }
    if ( map[pos.getRow()][pos.getCol()]->getType() == PATH ) {
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
    }
    else{
        next_pos = npos;
    }
};
Position MovingObject::getCurrentPosition() const{
    return this->pos;
};
void MovingObject::move(){
    pos = getNextPosition();
};
string MovingObject::str() const{
    return "MovingObject[index="+to_string(index)+";pos="+pos.str()+"]";
}
string MovingObject::getName() const{
    return this->name;
};

//TODO: 3.5.1: CHARACTER
Character::Character(int index, const Position &init_pos, Map* map, const string &name="") : MovingObject(index, pos, map, name){
    this->index = index;
    this->pos = init_pos;
    this->map = map;
    this->name = name;
};
Position Character::getNextPosition() {
    Position next_pos = pos;
    if(map->isValid(next_pos, this)){
        next_pos.setRow(next_pos.getRow() + 1);
    }else{
        next_pos = npos;
    }
};
Position Character::getCurrentPosition() const {
    return this->pos;
};
void Character::move(){
    pos = getNextPosition();
};
string Character::str() const {
    return "Character[index="+to_string(index)+";pos="+pos.str()+"]"; 
};

//TODO: 3.5.2: SHERLOCK
Sherlock::Sherlock(int index, const string & moving_rule, const Position & init_pos, Map * map, const string &name = "", int init_hp, int init_exp) : Character(index, init_pos, map, name){
    this->moving_rule = moving_rule;
    this->index_moving_rule = index_moving_rule;
    this->name = "Sherlock";
};
Position Sherlock::getNextPosition() {
    Position next_pos = npos;
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
    if (map->isValid(next_pos, this))
        return next_pos;
    else
        return npos;
};
Position Sherlock::getCurrentPosition() const {
    return this->pos;
};
void Sherlock::move(){
    pos = getNextPosition();
};
string Sherlock::str() const {
    return "Sherlock[index="+to_string(index)+";pos="+pos.str()+";moving_rule="+moving_rule+"]";
};
int Sherlock::setInitExp(int init_exp) const{
    if(init_exp < 0) return 0;
    else if(init_exp > 900) return 900;
    else return init_exp;
};
int Sherlock::setInitHP(int init_hp) const{
    if(init_hp < 0) return 0;
    else if(init_hp > 500) return 500;
    else return init_hp;
};

//TODO: 3.6: WATSON
Watson::Watson(int index, const string & moving_rule, const Position & init_pos, Map * map, const string &name = "Watson", int init_hp, int init_exp) : Character(index, init_pos, map, name){
    this->moving_rule = moving_rule;
    this->index_moving_rule = index_moving_rule;
    this->name = "Watson";
};
Position Watson::getNextPosition() {
    Position next_pos = npos;
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
    if (map->isValid(next_pos, this))
        return next_pos;
    else
        return npos;
};
Position Watson::getCurrentPosition() const {
    return this->pos;
};
void Watson::move(){
    pos = getNextPosition();
};
string Watson::str() const {
    return "Watson[index="+to_string(index)+";pos="+pos.str()+";moving_rule="+moving_rule+"]";
}
int Watson::setInitExp(int init_exp) const{
    if(init_exp < 0) return 0;
    else if(init_exp > 900) return 900;
    else return init_exp;
};
int Watson::setInitHP(int init_hp) const{
    if(init_hp < 0) return 0;
    else if(init_hp > 500) return 500;
    else return init_hp;
};

//TODO: 3.7: CRIMINAL
Criminal::Criminal(int index, const Position & init_pos, Map * map, const string &name = "", Sherlock * sherlock, Watson * watson) : Character(index, init_pos, map, name){
    this->index = index;
    this->sherlock = sherlock;
    this->watson = watson;
    this->name = "Criminal";
};
Position Criminal::getNextPosition() {
    //TODO: MANHATTAN EQUATION
    Position next_pos = npos;
    return next_pos;
};
Position Criminal::getCurrentPosition() const {
    return this->pos;
};
void Criminal::move(){
    pos = getNextPosition();
};
string Criminal::str() const {
    return "Criminal[index="+to_string(index)+";pos="+pos.str()+"]";
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
    return "ArrayMovingObject[count="+to_string(count)+"]";
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
                    cout<<arr_walls[i]<<" ";
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
                    cout<<arr_fake_walls[i]<<" ";
                }
                cout<<endl;
            }

        }
    }  
};
string Configuration::str() const{
    cout<<"Configuration["<<endl<<"MAP_NUM_ROWS="<<map_num_rows<<endl<<"MAP_NUM_COLS="<<map_num_cols<<endl<<"MAX_NUM_MOVING_OBJECTS="<<max_num_moving_objects<<endl<<"NUM_WALLS="<<num_walls<<endl<<"ARRAY_WALLS="<<arr_walls<<endl<<"NUM_FAKE_WALLS="<<num_fake_walls<<endl<<"ARRAY_FAKE_WALLS="<<arr_fake_walls<<endl<<"SHERLOCK_MOVING_RULE="<<sherlock_moving_rule<<endl<<"SHERLOCK_INIT_POS="<<sherlock_init_pos.str()<<endl<<"WATSON_MOVING_RULE="<<watson_moving_rule<<endl<<"WATSON_INIT_POS="<<watson_init_pos.str()<<endl<<"CRIMINAL_INIT_POS="<<criminal_init_pos.str()<<endl<<"NUM_STEPS="<<num_steps<<endl<<"]";
};

//TODO: 3.10: ROBOT
Robot::Robot(RobotType in_type, const Position& in_pos, int in_hp, int in_exp) : MovingObject(index, pos, map, name){
    this->robot_type = in_type;
    this->pos = in_pos;
    this->hp = in_hp;
    this->exp = in_exp;
};
Position Robot::getNextPosition() {
    //TODO: MANHATTAN EQUATION
    Position next_pos = npos;
    return next_pos;
};
Position Robot::getCurrentPosition() const{
    return this->pos;
};
void Robot::move(){
    pos = getNextPosition();
};
string Robot::str() const{
    return "Robot[pos="+pos.str()+"type="+to_string(robot_type)+"]";
};

//TODO:3.10.2: ROBOT C, S, W, SW
//TODO: 3.11: BASE ITEM
//TODO: 3.12: BASE BAG


////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////