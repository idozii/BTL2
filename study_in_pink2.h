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
class MovingObject;
class Position;
class Configuration;
class Map;

class Sherlock;
class Watson;
class Criminal;
class Character;
class Robot;
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
class Node;

class TestStudyInPink;

enum Type{ SHERLOCK, WATSON, CRIMINAL, ROBOT };
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
    ElementType getElementType(Position pos) const;
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
    bool isEqual(int in_r, int in_c) const;
    bool isEqual(Position pos) const;
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
    virtual ~MovingObject();
    virtual Position getNextPosition() = 0;
    Position getCurrentPosition() const;
    virtual void move() = 0;
    virtual string str() const = 0;
    virtual int getExp() const;
    virtual int getHp() const;
    virtual Type getObjectType() const = 0;
};

class Character : public MovingObject {
    friend class TestStudyInPink;

public:
    Character(int index, const Position &pos, Map* map, const string &name="");
    virtual Position getNextPosition() = 0;
    virtual void move() = 0;
    virtual string str() const = 0;
    virtual int getHp() const;
    virtual int getExp() const;
    virtual void setHp(int init_hp);
    virtual void setExp(int init_exp);
    virtual Type getObjectType() const = 0;
};

class Sherlock : public Character {
    friend class TestStudyInPink;

private:
    string moving_rule;
    int index_moving_rule;
    int hp;
    int exp;
    BaseBag* sherlockBag;
    BaseBag* watsonBag;

public:
    Sherlock(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp);
    ~Sherlock();
    Position getNextPosition();
    void move();
    Type getObjectType() const;
    BaseBag* getSherlockBag() const;
    string str() const;
    int getHp() const;
    int getExp() const;
    void setHp(int hp);
    void setExp(int exp);
    void setPos(Position pos);
    bool meet(RobotS* robotS);
    bool meet(RobotW* robotW);
    bool meet(RobotSW* robotSW);
    bool meet(RobotC* robotC);
    bool meet(Watson* watson);
};

class Watson : public Character {
    friend class TestStudyInPink;

private:
    string moving_rule;
    int index_moving_rule;
    int HP;
    int EXP;
    BaseBag* watsonBag;
    BaseBag* sherlockBag;

public:
    Watson(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp);
    ~Watson();
    Position getNextPosition();
    void move();
    Type getObjectType() const;
    BaseBag* getWatsonBag() const;
    string str() const;
    int getExp() const;
    int getHp() const;
    void setHp(int hp);
    void setExp(int exp);
    bool meet(Sherlock* sherlock);
    bool meet(RobotS* robotS);
    bool meet(RobotW* robotW);
    bool meet(RobotSW* robotSW);
    bool meet(RobotC* robotC);
};

class Criminal : public Character {
    friend class TestStudyInPink;

private:
    Sherlock* sherlock;
    Watson* watson;
    int count;
    Position prev_pos;
    
public:
    Criminal(int index, const Position & init_pos, Map * map, Sherlock * sherlock, Watson * watson);
    Position getNextPosition();
    void move();
    Type getObjectType() const;
    string str() const;
    int getCount() const;
    bool isCreatedRobotNext() const;
    Position getPrevPos() const;
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
    MovingObject * get(int index) const;
    int size() const;
    string str() const;
    bool checkMeet(int index) const;
};

class Configuration {
    friend class StudyPinkProgram;
    friend class TestStudyInPink;

private:
    int map_num_rows;
    int map_num_cols;
    int max_num_moving_objects;
    int num_walls;
    Position *arr_walls;
    int num_fake_walls;
    Position *arr_fake_walls;
    string sherlock_moving_rule;
    Position sherlock_init_pos;
    int sherlock_init_hp;
    int sherlock_init_exp;
    string watson_moving_rule;
    Position watson_init_pos;
    int watson_init_hp;
    int watson_init_exp;
    Position criminal_init_pos;
    int num_steps;
    string configString[2][17] = {};

public:
    Configuration(const string & filepath);
    ~Configuration();
    string str() const;
};

class BaseItem {
    friend class TestStudyInPink;

public:
    ~BaseItem();
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
    string str() const;
};

class EnergyDrink : public BaseItem{
    friend class TestStudyInPink;

public:
    bool canUse(Character* obj, Robot *robot);
    void use(Character* obj, Robot *robot);
    ItemType getType() const;
    string str() const;
};

class FirstAid : public BaseItem{
    friend class TestStudyInPink;

public:
    bool canUse(Character* obj, Robot *robot);
    void use(Character* obj, Robot *robot);
    ItemType getType() const;
    string str() const;
};

class ExemptionCard : public BaseItem{
    friend class TestStudyInPink;

public:
    bool canUse(Character* obj, Robot *robot);
    void use(Character* obj, Robot *robot);
    ItemType getType() const;
    string str() const;
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
    string str() const;
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
    Character *obj;
    int size;
    int capacity;
    Node* head;

public:
    BaseBag(int capacity);
    virtual ~BaseBag();
    virtual bool insert(BaseItem* item); 
    virtual BaseItem* get() = 0;
    virtual BaseItem* get(ItemType type);
    virtual int getCount() const;
    virtual void remove(ItemType type);
    virtual string str() const;
};

class SherlockBag : public BaseBag {
    friend class TestStudyInPink;

private:
    Sherlock* sherlock;
    int itemNumber;

public:
    SherlockBag(Sherlock* sherlock);
    BaseItem* get();
};

class WatsonBag : public BaseBag {
    friend class TestStudyInPink;

private:
    Watson* watson;
    int itemNumber;

public:
    WatsonBag(Watson* watson);
    BaseItem* get();
};

class Robot : public MovingObject{
    friend class TestStudyInPink;

protected:
    RobotType robot_type;
    Criminal* criminal;
    BaseItem* item;

public:
    Robot(int index , const Position &pos , Map * map , Criminal* criminal, const string &name = "");
    ~Robot();
    Type getObjectType() const;
    static Robot* create(int index, Map* map, Criminal* criminal, Sherlock* sherlock, Watson* watson);
    virtual Position getNextPosition() = 0;
    virtual void move() = 0;
    virtual string str() const = 0;
    virtual RobotType getType();
    virtual int getDistance() const;
    BaseItem* NewItem();
};

class RobotC : public Robot {
    friend class TestStudyInPink;

public:
    RobotC ( int index , const Position & init_pos , Map * map , Criminal * criminal);
    Position getNextPosition();
    void move();
    RobotType getType() const;
    int getDistance(Sherlock* sherlock);
    int getDistance(Watson* watson);
    string str() const;

};

class RobotS : public Robot {
    friend class TestStudyInPink;

private:
    Sherlock* sherlock;

public:
    RobotS ( int index , const Position & init_pos , Map * map , Criminal * criminal , Sherlock * Sherlock);
    Position getNextPosition();
    void move();
    RobotType getType() const;
    int getDistance() const;
    string str() const;
};

class RobotW : public Robot {
    friend class TestStudyInPink;

private:
    Watson* watson;

public:
    RobotW ( int index , const Position & init_pos , Map * map , Criminal * criminal , Watson * watson);
    Position getNextPosition();
    void move() override;
    RobotType getType() const;
    int getDistance() const;
    string str() const;
};

class RobotSW : public Robot {
    friend class TestStudyInPink;

private:
    Sherlock* sherlock;
    Watson* watson;

public:
    RobotSW ( int index , const Position & init_pos , Map * map , Criminal * criminal , Sherlock * sherlock , Watson* watson);
    Position getNextPosition();
    void move();
    RobotType getType() const;
    int getDistance() const;
    string str() const;
};

class StudyPinkProgram {
    friend class TestStudyInPink;

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
    ~StudyPinkProgram();
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
};

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /* _H_STUDY_IN_PINK_2_H_ */
