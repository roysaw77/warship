#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>

using namespace std;

// Base class for ships
class ship {
private:
    int life = 3;

public:
    virtual void action() = 0; 

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
        random_device rd; // Seed for randomness
        mt19937 gen(rd()); // Random number generator
        uniform_int_distribution<> distWidth(0, gameMap[0].size() - 1);
        uniform_int_distribution<> distHeight(0, gameMap.size() - 1);

        do {
            ranWid = distWidth(gen); // Random generate width
            ranHei = distHeight(gen); // Random generate height
        } while (gameMap[ranHei][ranWid] != '0'); // Ensure the spot is empty

        gameMap[ranHei][ranWid] = sym; // Place ship on map
    }
};

// Derived ship classes
class movingShip : public virtual ship {
public:
    void action() override {
        cout << "Moving ship moves" << endl;
    }
};

class shootingShip : public virtual ship {
public:
    void action() override {
        cout << "Shooting ship shoots" << endl;
    }
};

class Battleship : public movingShip, public shootingShip {
public:
    void action() override {
        cout << "Battleship moves and shoots" << endl;
    }
};

// Game configuration class
class GameConfig {
private:
    int iterations;
    int mW; // map width
    int mH; // map height
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
                    gameMap.resize(mH, vector<char>(mW, '.'));
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

        // Initialize game map if empty
        if (gameMap.empty()) {
            gameMap.resize(mH, vector<char>(mW, '.'));
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

int main() {
    GameConfig config("game.txt");
    vector<vector<char>> gameMap = config.getGameMap();

    // Generate ship on map
    Battleship b;
    b.generateShip(gameMap, 'B');
    b.printMap(gameMap);

    return 0;
}
