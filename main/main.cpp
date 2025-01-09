#include <iostream>
#include "gameconfig.h"
#include "ship.h" 
#include "CVector.h"
using namespace std;


int main() {
    GameConfig config("game.txt");

    // Display iterations, map, and ship info
    cout << "Iterations: " << config.getIterations() << endl;
    config.displayGameMap();

    // Vector of ship pointers
    CVector<ship*> ships;

    // Create ships dynamically based on the configuration
    CVector<int> num ;
    CVector<char> sym ;

    for(int i=0;i<config.num.size();i++){
        num.push_back(config.num[i]);
        sym.push_back(config.sym[i]);
    }

    for (size_t i = 0; i < num.size(); ++i) {
        switch (sym[i]) {
        case '*':
            ships.push_back(new BattleShip(num[i], sym[i]));
            break;
        case '$':
            ships.push_back(new Cruiser(num[i], sym[i]));
            break;
        case '#':
            ships.push_back(new Destroyer(num[i], sym[i]));
            break;
        case '@':
            ships.push_back(new Frigate(num[i], sym[i]));
            break;
        case '&':
            ships.push_back(new Corvette(num[i], sym[i]));
            break;
        case '~':
            ships.push_back(new Amphibious(num[i], sym[i]));
            break;
        default:
            cout << "Unknown ship type: " << sym[i] << endl;
        }
    }
    for (int i = 0; i < 6; i++)
    {
        ships[i]->move();
    }
    
  
    return 0;
}
