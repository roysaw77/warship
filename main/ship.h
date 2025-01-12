#include <iostream>
#include <vector>
#include <random>
using namespace std;

class ship {
  private:
    int life=3;
   public:
   virtual void action() = 0; 
   void printMap(vector<vector<char>> &gameMap){
     for (int i = 0; i < 11; i++) {
       for (int j = 0; j < 11; j++) {
         cout << gameMap[i][j] << " ";
       }
       cout << endl;
     }
   }
   void generateShip(vector<vector<char>> &gameMap, char sym){
     int ranWid = rand() % 11; //random generate width
     int ranHei = rand() % 11; //random generate height
     gameMap[ranWid][ranHei] = sym;//place ship on map
   }
};

class movingShip : public ship {
  public:
  void action() override {
    cout << "Moving ship moves" << endl;
  }
};

class shootingShip : public ship {
  public:
  void action() override {
    cout << "Shooting ship shoots" << endl;
  }
};

class seeingShip : public ship {
  public:
  void action() override {
    cout << "Seeking ship sees" << endl;
  }
};
 class rammingShip : public ship {
   public:
   void action() override {
     cout << "Ramming ship rams" << endl;
         }
 };

class Battleship : public movingShip ,public shootingShip{
  public:
  void action() override {
    cout << "Battleship moves and shoots" << endl;
  }
};

