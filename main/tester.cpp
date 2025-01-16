#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

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

    virtual void actionMoving() = 0;
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

public:
    void action() override {
        cout << "Battleship moves and sees" << endl;
        actionMoving();
        see(gameMap, sym);
    }

    Battleship() : sym(" ") {}
    Battleship(string sym, vector<vector<string>> &gameMap) : sym(sym) {
        this->gameMap = gameMap;
        generateShip(gameMap, sym);
    }

    void actionMoving() override {
        cout << "Battleship is moving!" << endl;
    }

    void see(const vector<vector<string>> &gameMap, const string &sym) override {
        auto pos = curpos(gameMap, sym);
        if (pos.first != -1 && pos.second != -1) {
            cout << "Battleship " << sym << " is scanning the area around (" << pos.first + 1 << ", " << pos.second + 1 << ")" << endl;
        }
    }
};

class Cruiser : public movingShip {
private:
    string sym;

public:
    void action() override {
        cout << "Cruiser moves" << endl;
    }

    Cruiser() : sym(" ") {}
    Cruiser(string sym, vector<vector<string>> &gameMap) : sym(sym) {
        this->gameMap = gameMap;
        generateShip(gameMap, sym);
    }

    void actionMoving() override {
        cout << "Cruiser is moving!" << endl;
    }
};

class Destroyer : public movingShip {
private:
    string sym;

public:
    void action() override {
        cout << "Destroyer moves" << endl;
    }

    Destroyer() : sym(" ") {}
    Destroyer(string sym, vector<vector<string>> &gameMap) : sym(sym) {
        this->gameMap = gameMap;
        generateShip(gameMap, sym);
    }

    void actionMoving() override {
        cout << "Destroyer is moving!" << endl;
    }
};

class frigate : public movingShip {
private:
    string sym;

public:
    void action() override {
        cout << "Frigate moves" << endl;
    }

    frigate() : sym(" ") {}
    frigate(string sym, vector<vector<string>> &gameMap) : sym(sym) {
        this->gameMap = gameMap;
        generateShip(gameMap, sym);
    }

    void actionMoving() override {
        cout << "Frigate is moving!" << endl;
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
                    gameMap.resize(mH, vector<string>(mW, "")); // Resize the map
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
    srand(time(0)); // Seed the random number generator

    // Load game configuration
    GameConfig config("game.txt");
    vector<vector<string>> gameMap = config.getGameMap();
    vector<string> Asym = config.getAsym(); // SymbolA
    vector<string> Bsym = config.getBsym(); // SymbolB

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

    // Create Team A ships
    vector<ship*> AShips;
    for (int i = 0; i < Asym.size(); i++) {
        if (Asym[i][0] == '*') {
            AShips.push_back(new Battleship(Asym[i], gameMap));
        } else if (Asym[i][0] == '$') {
            AShips.push_back(new Cruiser(Asym[i], gameMap));
        } else if (Asym[i][0] == '#') {
            AShips.push_back(new Destroyer(Asym[i], gameMap));
        } else if (Asym[i][0] == '@') {
            AShips.push_back(new frigate(Asym[i], gameMap));
        } else {
            cout << "Invalid symbol: " << Asym[i] << endl;
        }
    }

    // Print the game map after placing ships
    
    AShips[0]->printMap(gameMap);

    // Free memory
    for (auto shipPtr : AShips) {
        delete shipPtr;
    }

    return 0;
}