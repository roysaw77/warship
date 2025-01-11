#include <iostream>
#include "gameconfig.h"
#include "ship.h" 

using namespace std;




int main() {
 GameConfig config("game.txt");

    // Display Team A ships
    cout << "Team A ships:" << endl;
    for (size_t i = 0; i < config.Asym.size(); i++) {
        cout << config.Asym[i] << " - " << config.Anum[i] << endl;
    }

    // Display Team B ships
    cout << "Team B ships:" << endl;
    for (size_t i = 0; i < config.Bsym.size(); i++) {
        cout << config.Bsym[i] << " - " << config.Bnum[i] << endl;
    }

    // Display game map
    cout << "Game Map:" << endl;
    for (const auto &row : config.gameMap) {
        for (const auto &cell : row) {
            cout << cell << ' ';
        }
        cout << endl;
    }

    return 0;
}
