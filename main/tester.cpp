#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <utility>
#include <iomanip>

using namespace std;

// Base class for ships
class ship {
private:
    int life = 3;

public:
    virtual void action(vector<vector<string>> &gameMap, char command,string sym) = 0;

    void printMap(const vector<vector<string>> &gameMap) {
    for (const auto &row : gameMap) {
        for (const auto &cell : row) {
            // Print each cell with a fixed width of 3
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
            ranWid = rand() % gameMap[0].size();
            ranHei = rand() % gameMap.size();
        } while (gameMap[ranHei][ranWid]!="1"); // Ensure the spot is valid

        gameMap[ranHei][ranWid] = sym; // Place ship on map
        cout << "Placed ship " << sym << " at (" << ranHei+1 << ", " << ranWid+1 << ")" << endl;
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
    void action(vector<vector<string>> &gameMap, char movement,string sym) override {
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
            gameMap[i][j] = "0";       // Clear old position
            gameMap[newI][newJ] = sym; // Move to new position
            cout<<"Moved ship "<<sym<<" from ("<<i+1<<", "<<j+1<<") to ("<<newI+1<<", "<<newJ+1<<")"<<endl;
        } else {
            cout << "Invalid move: Out of bounds or position occupied!" << endl;
        }//not complete it cant need to have while loop to input again invalid move
    }
};

class shootingShip : public virtual ship {
public:
    void action(vector<vector<string>> &gameMap, char command,string sym) override {
        cout << "Shooting ship shoots" << endl;
    }
};

class Battleship : public movingShip, public shootingShip {
public:
    void action(vector<vector<string>> &gameMap, char command,string sym) override {
        if (command == 'F') {
            cout << "Battleship fires!" << endl;
        } else {
            movingShip::action(gameMap, command,sym); // Delegate movement to movingShip
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
    GameConfig config("game.txt");
    vector<vector<string>> gameMap = config.getGameMap();
    vector<string> Asym = config.getAsym();
    vector<string> Atest= {"A1","A2","A3"};
    Battleship B;
    for(const auto &sym : Atest){
        B.generateShip(gameMap, sym);
    }
    B.printMap(gameMap);

    char command;
    while (true) {
        for(const auto &sym : Atest){
            cout << "Enter command (W/A/S/D for movement, F for fire, Q to quit): ";
            cin>>command;
            if (command == 'Q'||command == 'q') return false;
            B.action(gameMap, command,sym);
            
        }
        B.printMap(gameMap);

    }
    return 0;

}
