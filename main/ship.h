#include <random>

class ship{
   private:
   int num;
   char sym;
   int health=3;
   public:
   ship(int num ,int sym):num(num),sym(sym){};
   virtual void move()=0;
   // virtual void shoot()=0;
   // virtual void see()=0;
   // virtual void ram()=0;
   int getNum(){
   return num;
   }
   char getSym(){
      return sym;
   }
   void generateShip(char &gameMap,int num,char sym){
      
   }
};



class BattleShip:public ship{
   public:
   BattleShip():ship(0,0){}
   BattleShip(int num,char sym):ship(num,sym){}
    void move() override { cout << "BattleShip moves to (" << getNum() << ", " << getSym() << ")\n"; }
};

class Cruiser:public ship{
   public:
   Cruiser():ship(0,0){}
   Cruiser(int num,char sym):ship(num,sym){}
   void move() override { cout << "Cruiser moves to (" << getNum()<< ", " << getSym() << ")\n"; }
};

class Destroyer:public ship{
   public:
   Destroyer():ship(0,0){}
   Destroyer(int num ,char sym):ship(num,sym){}
   void move() override { cout << "Destroyer moves to (" << getNum() << ", " << getSym() << ")\n"; }
};

class Frigate:public ship{
   public:
   Frigate():ship(0,0){}
   Frigate(int num,char sym):ship(num,sym){}
   void move() override { cout << "Frigate moves to (" << getNum()  << ", " <<   getSym()<< ")\n"; }
};

class Corvette:public ship{
   public:
   Corvette():ship(0,0){}
   Corvette(int num,char sym):ship(num,sym){}
   void move() override { cout << "Corvette moves to (" << getNum() << ", " <<getSym() << ")\n"; }
};

class Amphibious:public ship{
   public:
    Amphibious():ship(0,0){}
    Amphibious(int num,char sym):ship(num,sym){}
    void move() override { cout << "Amphibious moves to (" << getNum()<< ", " <<getSym() << ")\n"; }
};

class SuperShip:public ship{
   public:
      SuperShip():ship(0,0){}
      SuperShip(int num,char sym):ship(num,sym){}
      void move() override { cout << "Supership moves to (" <<getNum() << ", " << getSym() << ")\n"; }
};

