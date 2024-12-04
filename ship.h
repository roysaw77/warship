class ship{
   private:
   int row,column,health;
   public:
};

class MovingShip:public ship{

};

class ShootingShip:public ship{
    
};


class SeeingRobot:public ship{

};

class Ramship:public ship{

};

class BattleShip:public MovingShip,public ShootingShip,public SeeingRobot,public Ramship{

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

