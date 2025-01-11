#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// Base class for ships
class ship {
private:
    int num;
    char sym;

public:
    ship(int num, char sym) : num(num), sym(sym) {}
    virtual void move() = 0;

    int getNum() const {
        return num;
    }

    char getSym() const {
        return sym;
    }

    void generateShip(vector<vector<char>> &gameMap, int num, char sym) {
        // Logic to place ships on the map (not implemented in original code)
    }
};

// Derived ship classes
class BattleShip : public ship {
public:
    BattleShip(int num, char sym) : ship(num, sym) {}
    void move() override {
        cout << "BattleShip moves (" << getNum() << ", " << getSym() << ")\n";
    }
};

class Cruiser : public ship {
public:
    Cruiser(int num, char sym) : ship(num, sym) {}
    void move() override {
        cout << "Cruiser moves (" << getNum() << ", " << getSym() << ")\n";
    }
};

class Destroyer : public ship {
public:
    Destroyer(int num, char sym) : ship(num, sym) {}
    void move() override {
        cout << "Destroyer moves (" << getNum() << ", " << getSym() << ")\n";
    }
};

class Frigate : public ship {
public:
    Frigate(int num, char sym) : ship(num, sym) {}
    void move() override {
        cout << "Frigate moves (" << getNum() << ", " << getSym() << ")\n";
    }
};

// Game configuration class
class GameConfig {
private:
    int iterations;
    int mW; // map width
    int mH; // map height

public:
    vector<int> Anum;
    vector<char> Asym;
    vector<int> Bnum;
    vector<char> Bsym;
    vector<vector<char>> gameMap;

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
                    gameMap.resize(mH, vector<char>(mW));
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
    }

    int getWidth() const {
        return mW;
    }

    int getHeight() const {
        return mH;
    }
};

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

    // Create ship objects
    vector<ship *> ships;

    for (size_t i = 0; i < config.Anum.size(); ++i) {
        switch (config.Asym[i]) {
        case '*':
            ships.push_back(new BattleShip(config.Anum[i], config.Asym[i]));
            break;
        case '$':
            ships.push_back(new Cruiser(config.Anum[i], config.Asym[i]));
            break;
        case '#':
            ships.push_back(new Destroyer(config.Anum[i], config.Asym[i]));
            break;
        case '@':
            ships.push_back(new Frigate(config.Anum[i], config.Asym[i]));
            break;
        default:
            cout << "Unknown ship type: " << config.Asym[i] << endl;
        }
    }

    // Test ship movements
    for (size_t i = 0; i < ships.size(); i++) {
        ships[i]->move();
    }

    // Free allocated memory
    for (auto &ship : ships) {
        delete ship;
    }

    return 0;
}
