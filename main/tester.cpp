#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <utility>
#include <iomanip>

using namespace std;

// Base class for ships
// Base class for ships
class ship {
protected:
    vector<vector<string>> gameMap;
    string sym;
    int num;
    int live = 3;
    int kill = 0;

public:
    virtual void action() = 0;

    void printMap(const vector<vector<string>> &gameMap) {
        for (const auto &row : gameMap) {
            for (const auto &cell : row) {
                if (cell.empty()) {
                    cout << setw(5) << "0"; // Display "0" for empty cells
                } else {
                    cout << setw(5) << cell; // Display the symbol
                }
            }
            cout << endl;
        }
    }

    void generateShip(vector<vector<string>> &gameMap, const string &sym) {
        int ranWid, ranHei;
        do {
            srand(time(0));
            ranWid = rand() % gameMap[0].size();
            ranHei = rand() % gameMap.size();
        } while (!gameMap[ranHei][ranWid].empty()); // Ensure the spot is valid

        gameMap[ranHei][ranWid] = sym; // Place ship on map
        cout << "Placed ship " << sym << " at (" << ranHei + 1 << ", " << ranWid + 1 << ")" << endl;
    }

    pair<int, int> curpos(const vector<vector<string>> &gameMap, const string &sym) {
        for (int i = 0; i < gameMap.size(); i++) {
            for (int j = 0; j < gameMap[i].size(); j++) {
                if (gameMap[i][j] == sym) {
                    return {i, j};
                }
            }
        }
        return {-1, -1}; // Ship not found
    }
};

// Derived ship classes
class movingShip : public virtual ship {
public:
    void action() override {
        cout << "Moving ship moves" << endl;
    }

    virtual void actionMoving(vector<vector<string>> &gameMap, char movement, string sym) = 0;
};

class seeingShip : public virtual ship {
public:
    void action() override {
        cout << "Seeing ship sees" << endl;
    }

      virtual void see(const vector<vector<string>> &gameMap, const string &sym) = 0; 
};

class shootingShip : public virtual ship {
public:
    void action() override {
        cout << "Shooting ship shoots" << endl;
    }
};


class Battleship : public movingShip, public seeingShip {
private:
    string sym;
    int num;

public:
     void action() override {
        cout << "Battleship moves and sees" << endl;
     }
    Battleship() : sym(" "), num(0) {}
    Battleship(string sym, int num, vector<vector<string>> &gameMap) : sym(sym), num(num) {
        this->gameMap = gameMap;
    }

    void actionMoving(vector<vector<string>> &gameMap, char movement, string sym) override {
        pair<int, int> pos = curpos(gameMap, sym);
        if (pos.first == -1 || pos.second == -1) {
            cout << "Ship not found!" << endl;
            return;
        }

        int i = pos.first;
        int j = pos.second;

        // Determine new position based on movement
        int newI = i, newJ = j;
        switch (movement) {
        case 'W':
        case 'w': newI = i - 1; break; // Move up
        case 'A':
        case 'a': newJ = j - 1; break; // Move left
        case 'S':
        case 's': newI = i + 1; break; // Move down
        case 'D':
        case 'd': newJ = j + 1; break; // Move right
        default:
            cout << "Invalid movement command!" << endl;
            return;
        }

        // Check bounds and ensure the new position is empty
        if (newI >= 0 && newI < gameMap.size() && newJ >= 0 && newJ < gameMap[0].size() && gameMap[newI][newJ].empty()) {
            gameMap[i][j] = "";       // Clear old position
            gameMap[newI][newJ] = sym; // Move to new position
            cout << "Moved ship " << sym << " from (" << i + 1 << ", " << j + 1 << ") to (" << newI + 1 << ", " << newJ + 1 << ")" << endl;
        } else {
            cout << "Invalid move: Out of bounds or position occupied!" << endl;
        }
    }
    
    void see(const vector<vector<string>> &gameMap, const string &sym)override {
        pair<int, int> pos = curpos(gameMap, sym);
        if (pos.first == -1 || pos.second == -1) {
            cout << "Ship not found!" << endl;
            return;
        }

        int i = pos.first;
        int j = pos.second;

        vector<pair<int, int>> directions = {
            {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}};
        bool enemyFound = false;

        for (const auto &[dx, dy] : directions) {
            int ni = i + dx;
            int nj = j + dy;

            if (ni >= 0 && ni < gameMap.size() && nj >= 0 && nj < gameMap[0].size()) {
                if (gameMap[ni][nj] == "1") {
                    cout << "Enemy found at (" << ni + 1 << ", " << nj + 1 << ")" << endl;
                    enemyFound = true;
                }
            }
        }

        if (!enemyFound) {
            cout << "No enemies near ship " << sym << endl;
        }
    }

};



// Game configuration class
class GameConfig {
private:
    int iterations;
    int mW;
    int mH;
    vector<int> Anum;
    vector<string> Asym;
    vector<int> Bnum;
    vector<string> Bsym;
    vector<vector<string>> gameMap;

public:
    GameConfig(const string &filename) : iterations(0), mW(0), mH(0) {
        ifstream file(filename);

        if (file.is_open()) {
            string key;

            while (file >> key) {
                if (key == "iterations") {
                    file >> iterations;
                } else if (key == "width") {
                    file >> mW;
                } else if (key == "height") {
                    file >> mH;
                } else if (key == "Team") {
                    string teamName;
                    int numShips;
                    file >> teamName >> numShips;

                    if (teamName == "A") {
                        for (int i = 0; i < numShips; i++) {
                            string shipType;
                            char symbol;
                            int count;
                            file >> shipType >> symbol >> count;
                            Anum.push_back(count);

                            for (int j = 1; j <= count; ++j) {
                                Asym.push_back(string(1, symbol) + to_string(j));
                            }
                        }
                    } else if (teamName == "B") {
                        for (int i = 0; i < numShips; i++) {
                            string shipType;
                            char symbol;
                            int count;
                            file >> shipType >> symbol >> count;
                            Bnum.push_back(count);

                            for (int j = 1; j <= count; ++j) {
                                Bsym.push_back(string(1, symbol) + to_string(j));
                            }
                        }
                    }
                } else if (key == "0" || key == "1") {
                    gameMap.resize(mH, vector<string>(mW,""));//resize the map
                    file.unget();
                    for (int i = 0; i < mH; i++) {
                        for (int j = 0; j < mW; j++) {
                            char cell;
                            file >> cell;
                            gameMap[i][j] = (cell == '1' ? "1" : ""); // Mark islands as "1"
                        }
                    }
                }
            }
            file.close();
        } else {
            cout << "Error opening file" << endl;
        }
    }

    vector<vector<string>> getGameMap() { return gameMap; }
    vector<string> getAsym() { return Asym; }
    vector<string> getBsym() { return Bsym; }
};





// Main function
int main() {
    // Load game configuration
    GameConfig config("game.txt");
    vector<vector<string>> gameMapA = config.getGameMap();
    vector<vector<string>> gameMapB = config.getGameMap();
    vector<string> Asym = config.getAsym();
    vector<string> Bsym = config.getBsym();

    // Display ship symbols for testing
    cout << "Team A ships: ";
    for (const auto &sym : Asym) {
        cout << sym << " ";
    }
    cout << endl;

    cout << "Team B ships: ";
    for (const auto &sym : Bsym) {
        cout << sym << " ";
    }
    cout << endl;

    // Place a ship in the game map
    Battleship battleShipA1("A1", 1, gameMapA);
    battleShipA1.generateShip(gameMapA, "A1");

    // Use polymorphism to move the ship
    ship *pShip = &battleShipA1;
    pShip->printMap(gameMapA);


    while(true){
    char command;
    cout << "Enter command (W/A/S/D for movement, F for fire, Q to quit): ";
    cin >> command;
    if (command == 'Q'||'q') break;
    
    // Perform move actions
    
    dynamic_cast<Battleship *>(pShip)->actionMoving(gameMapA, command, "A1");
    pShip->printMap(gameMapA);

   
    }

    return 0;

}
