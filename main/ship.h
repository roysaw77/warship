#include <iostream>
#include <vector>
#include <random>
using namespace std;

class ship {
  private:
    int life=3;
   public:
   virtual void action() = 0; 
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

 