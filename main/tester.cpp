#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <utility>

using namespace std;

// Base class for ships
class ship {
private:
    int life = 3;

public:
    virtual void action(vector<vector<char>> &gameMap, char command) = 0;

    void printMap(const vector<vector<char>> &gameMap) {
        for (const auto &row : gameMap) {
            for (char cell : row) {
                cout << cell << " ";
            }
            cout << endl;
        }
    }

    void generateShip(vector<vector<char>> &gameMap, char sym) {
        int ranWid, ranHei;
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distWidth(0, gameMap[0].size() - 1);
        uniform_int_distribution<> distHeight(0, gameMap.size() - 1);

        do {
            ranWid = distWidth(gen);
            ranHei = distHeight(gen);
        } while (gameMap[ranHei][ranWid] != '0'); // Ensure the spot is empty

        gameMap[ranHei][ranWid] = sym; // Place ship on map
    }

    pair<int, int> curpos(const vector<vector<char>> &gameMap, char sym) {
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
    void action(vector<vector<char>> &gameMap, char movement) override {
        pair<int, int> pos = curpos(gameMap, 'B');
        if (pos.first == -1 || pos.second == -1) {
            cout << "Ship not found!" << endl;
            return;
        }

        int i = pos.first;
        int j = pos.second;

        // Determine new position based on movement
        int newI = i, newJ = j;
        switch (movement) {
        case 'W': newI = i - 1; break; // Move up
        case 'A': newJ = j - 1; break; // Move left
        case 'S': newI = i + 1; break; // Move down
        case 'D': newJ = j + 1; break; // Move right
        default:
            cout << "Invalid movement command!" << endl;
            return;
        }

        // Check bounds and ensure the new position is empty
        if (newI >= 0 && newI < gameMap.size() && newJ >= 0 && newJ < gameMap[0].size() && gameMap[newI][newJ] == '0') {
            gameMap[i][j] = '0';    // Clear old position
            gameMap[newI][newJ] = 'B'; // Move to new position
        } else {
            cout << "Invalid move: Out of bounds or position occupied!" << endl;
        }
    }
};

class shootingShip : public virtual ship {
public:
    void action(vector<vector<char>> &gameMap, char command) override {
        cout << "Shooting ship shoots" << endl;
    }
};

class Battleship : public movingShip, public shootingShip {
public:
    void action(vector<vector<char>> &gameMap, char command) override {
        if (command == 'F') {
            cout << "Battleship fires!" << endl;
        } else {
            movingShip::action(gameMap, command); // Delegate movement to movingShip
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
    vector<char> Asym;
    vector<int> Bnum;
    vector<char> Bsym;
    vector<vector<char>> gameMap;

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
                            Asym.push_back(symbol);
                        }
                    } else if (teamName == "B") {
                        for (int i = 0; i < numShips; i++) {
                            string shipType;
                            char symbol;
                            int count;
                            file >> shipType >> symbol >> count;
                            Bnum.push_back(count);
                            Bsym.push_back(symbol);
                        }
                    }
                } else if (key == "0" || key == "1") {
                    gameMap.resize(mH, vector<char>(mW, '0'));
                    file.unget();
                    for (int i = 0; i < mH; i++) {
                        for (int j = 0; j < mW; j++) {
                            file >> gameMap[i][j];
                        }
                    }
                }
            }
            file.close();
        } else {
            cout << "Error opening file" << endl;
        }

        if (gameMap.empty()) {
            gameMap.resize(mH, vector<char>(mW, '0'));
        }
    }

    int getWidth() const { return mW; }
    int getHeight() const { return mH; }
    vector<int> getAnum() { return Anum; }
    vector<char> getAsym() { return Asym; }
    vector<int> getBnum() { return Bnum; }
    vector<char> getBsym() { return Bsym; }
    vector<vector<char>> getGameMap() { return gameMap; }
};

// Main function
int main() {
    GameConfig config("game.txt");
    vector<vector<char>> gameMap = config.getGameMap();

    Battleship b;
    b.generateShip(gameMap, 'B');
    b.printMap(gameMap);

    char command;
    while (true) {
        cout << "Enter command (W/A/S/D for movement, F for fire, Q to quit): ";
        cin >> command;
        if (command == 'Q') break;

        b.action(gameMap, command);
        b.printMap(gameMap);
    }

    return 0;
}
