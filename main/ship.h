class ship{
   private:
   int x,y;
   public:
   ship(int x,int y);
   virtual void move(int x,int y)=0;
   virtual void shoot()=0;
   virtual void see()=0;
   virtual void ram()=0;
};


class Cruiser:public ship{

};

class Destroyer:public Cruiser{

};

class Frigate:public ship{

};

class Amphibious:public ship{

};

class SuperShip:public Cruiser{

};

