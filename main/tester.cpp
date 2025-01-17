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
    vector<vector<string>> &gameMap;
    string sym; // Ship symbol
    int posI = -1; // Position I
    int posJ = -1; // Position J
    int life = 3; // Ship life
    int kill = 0; // Ship kill

public:
   ship(string ship, int i, int j, vector<vector<string>> &mapRef)
        : sym(ship), posI(i), posJ(j), gameMap(mapRef) {}

    virtual ~ship() {}
    virtual void action() = 0;

    void generateShip(const string &sym) {
        int ranWid, ranHei;
        do {
            ranWid = rand() % gameMap[0].size();
            ranHei = rand() % gameMap.size();
        } while (!gameMap[ranHei][ranWid].empty()); // Ensure the spot is valid

        gameMap[ranHei][ranWid] = sym; // Place ship on map
        setLocation(ranHei, ranWid);
        cout << "Placed ship " << sym << " at (" << ranHei + 1 << ", " << ranWid + 1 << ")" << endl;
    }

    void placeShip(vector<vector<string>> &gameMap, const string &sym,int i ,int j) {
        gameMap[i][j] = sym; // Place ship on map
        setLocation(i, j);
    }

    void setLocation(int i, int j) { posI = i; posJ = j; }
    pair<int, int> getLocation() { return {posI, posJ}; }

    void setLife(int Life) { life = Life; }
    int getLive() { return life; }

    void setkill(int Kill) { kill = Kill; }
    int getkill() { return kill; }

    void setSym(string Sym) { sym = Sym; }
    string getSym() { return sym; }

    bool isWithinBound(int newI,int newJ){
        return newI >= 0 && newI < gameMap.size() && newJ >= 0 && newJ < gameMap[0].size() && gameMap[newI][newJ].empty(); 
    }
};

// Derived ship classes
class movingShip : public virtual ship {
public:
    virtual ~movingShip() {}
    virtual void actionMoving() = 0;
};

class seeingShip : public virtual ship {
public:
    virtual ~seeingShip() {}
    virtual void actionSeeing() = 0;
};

class shootingShip : public virtual ship {
public:
    void action() override {
        cout << "Shooting ship shoots" << endl;
    }
};

class Battleship : public movingShip, public seeingShip {
public:
    void action() override {
        actionMoving();
    }

    
   Battleship(string sym, vector<vector<string>> &gameMap)
        : ship(sym, -1, -1, gameMap) {
        generateShip(sym);
    }

    void actionMoving() override {
        pair<int, int> location = getLocation();
        int i = location.first;
        int j = location.second;

        int moveI = rand() % 3 - 1; // Random move for row (-1, 0, 1)
        int moveJ = rand() % 3 - 1; // Random move for column (-1, 0, 1)

        int newI = i + moveI;
        int newJ = j + moveJ;

        // Check if the new position is within bounds and not occupied
        if (isWithinBound(newI, newJ)) {
            gameMap[i][j] = ""; // Clear old position
            placeShip(gameMap, sym, newI, newJ);
            cout << "Ship: " << sym << " moved to (" << newI + 1 << ", " << newJ + 1 << ")" << endl;
        } else {
            cout << "Invalid move for ship: " << sym << endl;
        }
    }

    void actionSeeing() override {
        cout << "Battleship is seeing!" << endl;
    }
};

class Cruiser : public movingShip {
public:
    void action() override {
        actionMoving();
    }


     Cruiser(string sym, vector<vector<string>> &gameMap)
        : ship(sym, -1, -1, gameMap) {
        generateShip(sym);
    }

    void actionMoving() override {
        pair<int, int> location = getLocation();
        int i = location.first;
        int j = location.second;

        int moveI = rand() % 3 - 1; // Random move for row (-1, 0, 1)
        int moveJ = rand() % 3 - 1; // Random move for column (-1, 0, 1)

        int newI = i + moveI;
        int newJ = j + moveJ;

        // Check if the new position is within bounds and not occupied
        if (isWithinBound(newI, newJ)) {
            gameMap[i][j] = ""; // Clear old position
            placeShip(gameMap, sym, newI, newJ);
            cout << "Ship: " << sym << " moved to (" << newI + 1 << ", " << newJ + 1 << ")" << endl;
        } else {
            cout << "Invalid move for ship: " << sym << endl;
        }
    }
};

class Destroyer : public movingShip {
public:
    void action() override {
        actionMoving();
    }

 
      Destroyer(string sym, vector<vector<string>> &gameMap)
        : ship(sym, -1, -1, gameMap) {
        generateShip(sym);
    }

    void actionMoving() override {
        pair<int, int> location = getLocation();
        int i = location.first;
        int j = location.second;

        int moveI = rand() % 3 - 1; // Random move for row (-1, 0, 1)
        int moveJ = rand() % 3 - 1; // Random move for column (-1, 0, 1)

        int newI = i + moveI;
        int newJ = j + moveJ;

        // Check if the new position is within bounds and not occupied
        if (isWithinBound(newI, newJ)) {
            gameMap[i][j] = ""; // Clear old position
            placeShip(gameMap, sym, newI, newJ);
            cout << "Ship: " << sym << " moved to (" << newI + 1 << ", " << newJ + 1 << ")" << endl;
        } else {
            cout << "Invalid move for ship: " << sym << endl;
        }
    }
};

class Frigate : public movingShip {
public:
    void action() override {
        actionMoving();
    }
     Frigate(string sym, vector<vector<string>> &gameMap)
        : ship(sym, -1, -1, gameMap) {
        generateShip(sym);
    }

    void actionMoving() override {
        pair<int, int> location = getLocation();
        int i = location.first;
        int j = location.second;

        int moveI = rand() % 3 - 1; // Random move for row (-1, 0, 1)
        int moveJ = rand() % 3 - 1; // Random move for column (-1, 0, 1)

        int newI = i + moveI;
        int newJ = j + moveJ;

        // Check if the new position is within bounds and not occupied
        if (isWithinBound(newI, newJ)) {
            gameMap[i][j] = ""; // Clear old position
            placeShip(gameMap, sym, newI, newJ); // Move to new position
            cout << "Ship: " << sym << " moved to (" << newI + 1 << ", " << newJ + 1 << ")" << endl;
        } else {
            cout << "Invalid move for ship: " << sym << endl;
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

    vector<vector<string>> getGameMap() { return gameMap; }
    vector<string> getAsym() { return Asym; }
    vector<string> getBsym() { return Bsym; }
    int getIterations() { return iterations; }
};

// Main function
int main() {
    srand(time(0)); // Seed the random number generator

    // Load game configuration
    GameConfig config("game.txt");
    vector<vector<string>> gameMap = config.getGameMap();
    vector<string> Asym = config.getAsym(); // SymbolA
    vector<string> Bsym = config.getBsym(); // SymbolB
    int iterations = config.getIterations();

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
    vector<ship*> Bships;
    for (int i = 0; i < Asym.size(); i++) {
        if (Asym[i][0] == '*') {
            AShips.push_back(new Battleship(Asym[i], gameMap));
        } else if (Asym[i][0] == '$') {
            AShips.push_back(new Cruiser(Asym[i], gameMap));
        } else if (Asym[i][0] == '#') {
            AShips.push_back(new Destroyer(Asym[i], gameMap));
        } else if (Asym[i][0] == '@') {
            AShips.push_back(new Frigate(Asym[i], gameMap));
        } else {
            cout << "Invalid symbol: " << Asym[i] << endl;
        }
    }

    for (int i = 0; i < Bsym.size(); i++) {
        if (Bsym[i][0] == '<') {
            Bships.push_back(new Battleship(Bsym[i], gameMap));
        } else if (Bsym[i][0] == '>') {
            Bships.push_back(new Cruiser(Bsym[i], gameMap));
        } else {
            cout << "Invalid symbol: " << Bsym[i] << endl;
        }
    }

    // Print the game map after placing ships
    cout << "Initial Map:" << endl;
    config.printMap(gameMap);

    // Simulate iterations
    for (int iter = 0; iter < iterations; iter++) {
        cout << "\nIteration " << iter + 1 << ":" << endl;

        // Move all ships
        for (auto shipPtr : AShips) {
            shipPtr->action();
        }
        for (auto shipPtr : Bships) {
            shipPtr->action();
        }

        // Print the updated map
        config.printMap(gameMap);
    }

    // Free memory
    for (auto shipPtr : AShips) {
        delete shipPtr;
    }
    for (auto shipPtr : Bships) {
        delete shipPtr;
    }

    return 0;
}