class ship{
   private:
   int x,y,health;
   public:
   ship(int x,int y,int health);
   virtual void move(int x,int y)=0;
   virtual void shoot()=0;
   virtual void see()=0;
   virtual void ram()=0;
};


class BattleShip:public ship{
   public:
   void move(int x,int y);
   void shoot();
   void see();
   void ram();
};

class Cruiser:public BattleShip{

};

class Destroyer:public Cruiser{

};

class Frigate:public BattleShip{

};

class Amphibious:public BattleShip{

};

class SuperShip:public Cruiser{

};

