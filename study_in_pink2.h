/*
* Ho Chi Minh City University of Technology
* Faculty of Computer Science and Engineering
* Initial code for Assignment 1
* Programming Fundamentals Spring 2023
* Author: Vu Van Tien
* Date: 02.02.2023
*/

//The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_STUDY_IN_PINK_2_H_
#define _H_STUDY_IN_PINK_2_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

enum MovingObjectType{ SHERLOCK, WATSON, CRIMINAL, ROBOT };
enum ItemType { MAGIC_BOOK, ENERGY_DRINK, FIRST_AID, EXEMPTION_CARD, PASSING_CARD };
enum ElementType { PATH, WALL, FAKE_WALL };
enum RobotType { C=0, S, W, SW };

class MapElement {
    friend class TestStudyInPink;
protected:
    ElementType type;
public:
    MapElement(ElementType in_type);
    virtual ~MapElement();
    virtual ElementType getType() const;
};

class Path : public MapElement {
    friend class TestStudyInPink;
public:
    Path();
};

class Wall : public MapElement {
    friend class TestStudyInPink;
public:
    Wall();
};

class FakeWall : public MapElement {
    friend class TestStudyInPink;
private:
    int req_exp;

public:
    FakeWall(int in_req_exp);
    int getReqExp() const;
};

class Map {
    friend class TestStudyInPink;
private:
    int num_rows, num_cols;
    MapElement*** map;

public:
    Map(int num_rows, int num_cols, int num_walls, Position * array_walls, int num_fake_walls, Position * array_fake_walls);
    ~Map();
    int getNumRows() const;
    int getNumCols() const;
    ElementType getElementType(const Position &pos) const;
    bool isValid(const Position &pos, MovingObject *mv_obj) const;
};

class Position {
    friend class TestStudyInPink;
private:
    int r, c;

public:
    const static Position npos;
    Position(int r=0, int c=0);
    Position(const string & str_pos);
    int getRow() const;
    int getCol() const;
    void setRow(int r);
    void setCol(int c);
    string str() const;
    bool isEqual(const Position &pos) const;
};

class MovingObject {
    friend class TestStudyInPink;
protected:
    int index;
    int exp;
    int hp;
    Position pos;
    Map* map;
    string name;
    
public:
    MovingObject(int index, const Position pos, Map * map, const string & name="");
    virtual Position getNextPosition() = 0;
    Position getCurrentPosition() const;
    virtual void move() = 0;
    virtual string str() const = 0;
    int getExp() const;
    int getHp() const;
    virtual MovingObjectType getObjectType() const = 0;
};

class Character : public MovingObject {
public:
    Character(int index, const Position pos, Map* map, const string &name="") : MovingObject(index, pos, map, name){};
    virtual Position getNextPosition() = 0;
    virtual void move() = 0;
    virtual string str() const = 0;
    virtual int getHp() const = 0;
    virtual int getExp() const = 0;
    virtual int setHp(int init_hp) const = 0;
    virtual int setExp(int init_exp) const = 0;
    virtual MovingObjectType getObjectType() const = 0;
};

class Sherlock : public Character {
    friend class TestStudyInPink;
private:
    string moving_rule;
    int index_moving_rule;
    int hp;
    int exp;
    BaseBag* Bag;

public:
    Sherlock(int index, const string & moving_rule, const Position & init_pos, Map * map, const string &name = "", int init_hp, int init_exp) :Character(index, init_pos, map, name){};
    virtual Position getNextPosition();
    virtual void move();
    MovingObjectType getObjectType() const;
    virtual string str() const;
    virtual int getHp() const;
    virtual int getExp() const;
    virtual int setHp(int init_hp) const;
    virtual int setExp(int init_exp) const;
    void meet(RobotS* robotS);
    void meet(RobotW* robotW);
    void meet(RobotSW* robotSW);
    void meet(RobotC* robotC);
    void meet(Watson* watson);
};

class Watson : public Character {
    friend class TestStudyInPink;
private:
    string moving_rule;
    int index_moving_rule;
    int hp;
    int exp;
    BaseBag* Bag;

public:
    Watson(int index, const string & moving_rule, const Position & init_pos, Map * map, const string &name = "", int init_hp, int init_exp) : Character(index, init_pos, map, name){};
    virtual Position getNextPosition();
    virtual void move();
    MovingObjectType getObjectType() const;
    virtual string str() const;
    virtual int getExp() const;
    virtual int getHp() const;
    virtual int setHp(int init_hp) const;
    virtual int setExp(int init_exp) const;
    void meet(Sherlock* sherlock);
    void meet(RobotS* robotS);
    void meet(RobotW* robotW);
    void meet(RobotSW* robotSW);
    void meet(RobotC* robotC);
};

class Criminal : public Character {
    friend class TestStudyInPink;
private:
    Sherlock* sherlock;
    Watson* watson;
    int moveCount = 0;
    
public:
    Criminal(int index, const Position & init_pos, Map * map, const string &name = "", Sherlock * sherlock, Watson * watson) : Character(index, init_pos, map, name){};
    virtual Position getNextPosition();
    virtual void move();
    MovingObjectType getObjectType() const;
    virtual string str() const;
    virtual int getExp() const;
    virtual int getHp() const;
    virtual int setHp(int init_hp) const;
    virtual int setExp(int init_exp) const;
    int getCount();
    bool isCreatedRobotNext();
};

class ArrayMovingObject {
    friend class TestStudyInPink;
private:
    int count;
    int capacity;
    MovingObject ** arr_mv_objs;

public:
    ArrayMovingObject(int capacity);
    ~ArrayMovingObject() ;
    bool isFull() const;
    bool add(MovingObject * mv_obj);
    void remove(int index);
    MovingObject * get(int index) const;
    int size() const;
    string str() const;
    bool checkMeet(int index);
};

class Configuration {
    friend class StudyPinkProgram;
    friend class TestStudyInPink;

private:
    int map_num_rows, map_num_cols;
    int max_num_moving_objects;
    int num_walls;
    Position* arr_walls;
    int num_fake_walls;
    Position* arr_fake_walls;
    string sherlock_moving_rule;
    Position sherlock_init_pos;
    int sherlock_init_hp, sherlock_init_exp;
    string watson_moving_rule;
    Position watson_init_pos;
    int watson_init_hp, watson_init_exp;
    Position criminal_init_pos;
    int num_steps;

public:
    Configuration(const string & filepath);
    string str() const;
};

class BaseItem {
    friend class TestStudyInPink;
public:
    virtual bool canUse(Character *obj, Robot *robot) = 0;
    virtual void use(Character *obj, Robot *robot) = 0;
    virtual ItemType getType() const = 0;
    virtual string str() const = 0;
};

class MagicBook : public BaseItem{
    friend class TestStudyInPink;
public:
    bool canUse(Character* obj, Robot *robot);
    void use(Character* obj, Robot *robot);
    ItemType getType() const;
    virtual string str() const;
};

class EnergyDrink : public BaseItem{
    friend class TestStudyInPink;
public:
    bool canUse(Character* obj, Robot *robot);
    void use(Character* obj, Robot *robot);
    ItemType getType() const;
    virtual string str() const;
};

class FirstAid : public BaseItem{
    friend class TestStudyInPink;
public:
    bool canUse(Character* obj, Robot *robot);
    void use(Character* obj, Robot *robot);
    ItemType getType() const;
    virtual string str() const;
};

class ExemptionCard : public BaseItem{
    friend class TestStudyInPink;
public:
    bool canUse(Character* obj, Robot *robot);
    void use(Character* obj, Robot *robot);
    ItemType getType() const;
    virtual string str() const;
};

class PassingCard : public BaseItem{
    friend class TestStudyInPink;
private:
    string challenge;
public:
    PassingCard(int i, int j);
    bool canUse(Character* obj, Robot *robot);
    void use(Character* obj, Robot *robot);
    ItemType getType() const;
    virtual string str() const;
};

class BaseBag {
    friend class TestStudyInPink;
protected:
    class Node{
    public:
        BaseItem* item;
        Node* next;
    public:
        Node(BaseItem* item, Node* next = nullptr) : item(item), next(next){};
    };

protected:
    int size;
    int capacity;
    Node* head;

public:
    BaseBag(int capacity);
    virtual ~BaseBag();
    virtual bool insert(BaseItem* item); 
    virtual BaseItem* get() = 0;
    virtual BaseItem* get(ItemType type);
    virtual string str() const;
};

class SherlockBag : public BaseBag {
    friend class TestStudyInPink;
private:
    Sherlock* sherlock;

public:
    SherlockBag(Sherlock* character);
    BaseItem* get();
};

class WatsonBag : public BaseBag {
    friend class TestStudyInPink;
private:
    Watson* watson;

public:
    WatsonBag(Watson* character);
    BaseItem* get();
};

class Robot : public MovingObject{
    friend class TestStudyInPink;
protected:
    RobotType robot_type;
    Criminal* criminal;
    BaseItem* item;

public:
    Robot(int index , const Position pos , Map * map , RobotType robot_type, Criminal* criminal, const string &name = "");
    MovingObjectType getObjectType() const;
    static Robot* create(int index, Map* map, Criminal* criminal, Sherlock* sherlock, Watson* watson);
    virtual Position getNextPosition() = 0;
    virtual void move() = 0;
    virtual string str() const = 0;
    virtual RobotType getType() const = 0;
    int getDistance() const;
};

class RobotC : public Robot {
    friend class TestStudyInPink;
private:
    Criminal* criminal;
    Sherlock* sherlock;
    Watson* watson;
    RobotType robot_type;
    BaseItem* item;

public:
    RobotC ( int index , const Position & init_pos , Map * map , RobotType robot_type , Criminal * criminal);
    virtual Position getNextPosition();
    virtual void move();
    virtual RobotType getType() const;
    int getDistance(Sherlock* sherlock) const;
    int getDistance(Watson* watson) const;
    virtual string str() const;

};

class RobotS : public Robot {
    friend class TestStudyInPink;
private:
    Criminal* criminal;
    Sherlock* sherlock;
    RobotType robot_type;
    BaseItem* item;

public:
    RobotS ( int index , const Position & init_pos , Map * map , RobotType robot_type , Criminal * criminal , Sherlock * Sherlock);
    virtual Position getNextPosition();
    virtual void move();
    virtual RobotType getType() const;
    int getDistance() const;
    virtual string str() const;
};

class RobotW : public Robot {
    friend class TestStudyInPink;
private:
    RobotType robot_type;
    Criminal* criminal;
    Watson* watson;
    BaseItem* item;

public:
    RobotW ( int index , const Position & init_pos , Map * map , RobotType robot_type , Criminal * criminal , Watson * watson);
    virtual Position getNextPosition();
    virtual void move();
    virtual RobotType getType() const;
    int getDistance() const;
    virtual string str() const;
};

class RobotSW : public Robot {
    friend class TestStudyInPink;
private:
    RobotType robot_type;
    Criminal* criminal;
    Sherlock* sherlock;
    Watson* watson;
    BaseItem* item;

public:
    RobotSW ( int index , const Position & init_pos , Map * map , RobotType robot_type , Criminal * criminal , Sherlock * sherlock , Watson* watson);
    virtual Position getNextPosition();
    virtual void move();
    virtual RobotType getType() const;
    int getDistance() const;
    virtual string str() const;
};

class StudyPinkProgram {
private:
    // Sample attributes
    Configuration * config;
    Sherlock * sherlock;
    Watson * watson;
    Criminal * criminal;
    
    Map * map;
    ArrayMovingObject * arr_mv_objs;


public:
    StudyPinkProgram(const string & config_file_path);
    bool isStop() const;
    void printResult() const {
        if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition())) {
            cout << "Sherlock caught the criminal" << endl;
        }
        else if (watson->getCurrentPosition().isEqual(criminal->getCurrentPosition())) {
            cout << "Watson caught the criminal" << endl;
        }
        else {
            cout << "The criminal escaped" << endl;
        }
    }

    void printStep(int si) const {
        cout << "Step: " << setw(4) << setfill('0') << si
            << "--"
            << sherlock->str() << "--|--" << watson->str() << "--|--" << criminal->str() << endl;
    }

    void run(bool verbose);
    ~StudyPinkProgram();
};

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /* _H_STUDY_IN_PINK_2_H_ */
