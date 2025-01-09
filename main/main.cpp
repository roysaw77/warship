#include <iostream>
#include "gameconfig.h"
#include "vector.h"
using namespace std;


int main() {
    GameConfig config("game.txt");
     cout << "Iterations: " << config.getIterations() << endl;
    config.displayGameMap();
    config.coutNum();
    config.coutSym();
    return 0;
}
