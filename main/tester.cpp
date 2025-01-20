#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <queue>
#include <list>

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
    bool upgraded = false;

public:
   ship(string ship, int i, int j, vector<vector<string>> &mapRef): sym(ship), posI(i), posJ(j), gameMap(mapRef) {}

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
    int getLife() { return life; }

    void setkill(int Kill) { kill = Kill; }
    int getkill() { return kill; }

    void setSym(string Sym) { sym = Sym; }
    string getSym() { return sym; }

    bool isWithinBound(int newI,int newJ){
        return newI >= 0 && newI < gameMap.size() && newJ >= 0 && newJ < gameMap[0].size() && gameMap[newI][newJ].empty(); 
    }

    bool isAlly(const string &otherSym) {
    // Team A symbols: *, $, #, @
    // Team B symbols: <, >
    if ((sym[0] == '*' || sym[0] == '$' || sym[0] == '#' || sym[0] == '@') &&
        (otherSym[0] == '*' || otherSym[0] == '$' || otherSym[0] == '#' || otherSym[0] == '@')) {
        return true; // Both are Team A
    } else if ((sym[0] == '<' || sym[0] == '>') &&
               (otherSym[0] == '<' || otherSym[0] == '>')) {
        return true; // Both are Team B
    }
    return false; // Otherwise, they are enemies
    }

    void killIncreament() { kill++; }

    bool checkStatus() {
        if (life <= 0) {
            return false; // Ship is destroyed
        }
        if (gameMap[posI][posJ].empty()) {
             life--;
            cout << "Ship " << sym << " has been shot and lost 1 life, remaining life " <<life<< endl; 
        }
        return life > 0;
    }
    
    // Mark the ship as upgraded
    void markAsUpgraded() { upgraded = true; }

    // Check if the ship has been upgraded
    bool isUpgraded() const { return upgraded; }

   
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
    virtual ~shootingShip() {}
    virtual void actionShooting() = 0;
};

class rammingShip: public virtual ship {
    public:
    virtual ~rammingShip() {}
     virtual void actionRamming() = 0;
};

class Battleship : public movingShip,public seeingShip, public shootingShip {
public:
     

    void action() override {
        actionSeeing();
        actionMoving();
        actionShooting();
    }

    
   Battleship(string sym, vector<vector<string>> &gameMap)
        : ship(sym, -1, -1, gameMap) {
        generateShip(sym);
    }

    void actionMoving() override {
        pair<int, int> location = getLocation();
        int i = location.first;
        int j = location.second;
        int iter = 0;

        int newI, newJ;
        bool moved = false; // Flag to track if the ship successfully moved

        do {
            newI = i + (rand() % 3 - 1); // Generate new random position
            newJ = j + (rand() % 3 - 1);

            if (isWithinBound(newI, newJ) && gameMap[newI][newJ].empty()) {
                // Position is within bounds and not occupied
                moved = true;
                break;
            }
            iter++;
        } while (iter < 100); // Stop after 100 iterations

        if (!moved) {
            // No valid moves after 100 attempts
            cout << "BattleShip: " << sym << " could not move due to lack of valid positions." << endl;
            return;
        }

        // Clear old position
        gameMap[i][j] = "";

        // Place ship at new position
        placeShip(gameMap, sym, newI, newJ);
        cout << "BattleShip: " << sym << " moved to (" << newI + 1 << ", " << newJ + 1 << ")" << endl;

        // Update ship's location
        setLocation(newI, newJ);
    }


   void actionSeeing() override {
        pair<int, int> location = getLocation();
        int i = location.first;
        int j = location.second;

        // Define the relative positions to check around the ship
        vector<pair<int, int>> views = {{1, 1}, {1, 0}, {0, 1}, {-1, 0}, {-1, -1}, {1, -1}, {-1, 1}, {0, -1}};

        for (auto view : views) {
            int newI = i + view.first;
            int newJ = j + view.second;

            // Check if the new position is within bounds
            if (newI >= 0 && newI < gameMap.size() && newJ >= 0 && newJ < gameMap[0].size()) {
                string cellContent = gameMap[newI][newJ];

                // Check if the cell contains an enemy ship
                if (!cellContent.empty() && cellContent != "1") { // "1" represents an island
                    // Determine if the cell contains an enemy ship
                   if (!isAlly(cellContent)) { // If not an ally, it's an enemy
                    cout << "BattleShip " << sym << " detected enemy ship " << cellContent << " at (" << newI + 1 << ", " << newJ + 1 << ")" << endl;
                } else {
                    cout << "BattleShip " << sym << " detected ally ship " << cellContent << " at (" << newI + 1 << ", " << newJ + 1 << ")" << endl;
                    }
                }
                else if(cellContent.empty() && cellContent == "1"){
                    cout << "BattleShip " << sym << " detected nothing  "<< endl;
                }
            }
        }
    }

   void actionShooting() override {
    pair<int, int> location = getLocation();
    int i = location.first;
    int j = location.second;

    vector<pair<int, int>> shoot = {{1, 1}, {1, 0}, {0, 1}, {-1, 0}, {-1, -1}, {1, -1}, {-1, 1}, {0, -1}};

    for (int count = 0; count < 3; count++) { // Allow up to 3 shots
        srand(time(0) + count); // Different seed for randomness
        int ran = rand() % shoot.size(); // Random index within the shoot vector

        int newI = i + shoot[ran].first;
        int newJ = j + shoot[ran].second;

        // Ensure the target is within bounds
        if (newI >= 0 && newI < gameMap.size() && newJ >= 0 && newJ < gameMap[0].size()) {
            string &target = gameMap[newI][newJ];
            
            // Check if the target is not an ally and not an island
            if (!target.empty() && target != "1" && !isAlly(target)) {
                cout << "Battleship " << sym << " shoots at (" << newI + 1 << ", " << newJ + 1 
                     << ") and hits enemy " << target << endl;
                target = ""; // Clear the target
                killIncreament();
                cout<<sym<<" Kill: "<<kill<<endl;
            } else {
                cout << "Battleship " << sym << " shoots at (" << newI + 1 << ", " << newJ + 1 
                     << ") but misses." << endl;
            }
        } else {
            cout << "Battleship " << sym << " shoots out of bounds at (" 
                 << newI + 1 << ", " << newJ + 1 << ")." << endl;
        }
    }
}

};

class Cruiser : public movingShip , public rammingShip{
public:
    void action() override {
        actionMoving();
        actionRamming();
    }


     Cruiser(string sym, vector<vector<string>> &gameMap)
        : ship(sym, -1, -1, gameMap) {
        generateShip(sym);
    }

    void actionMoving() override {
        pair<int, int> location = getLocation();
        int i = location.first;
        int j = location.second;
        int iter = 0;

        int newI, newJ;
        bool moved = false; // Flag to track if the ship successfully moved

        do {
            newI = i + (rand() % 3 - 1); // Generate new random position
            newJ = j + (rand() % 3 - 1);

            if (isWithinBound(newI, newJ) && gameMap[newI][newJ].empty()) {
                // Position is within bounds and not occupied
                moved = true;
                break;
            }
            iter++;
        } while (iter < 100); // Stop after 100 iterations

        if (!moved) {
            // No valid moves after 100 attempts
            cout << "BattleShip: " << sym << " could not move due to lack of valid positions." << endl;
            return;
        }

        // Clear old position
        gameMap[i][j] = "";

        // Place ship at new position
        placeShip(gameMap, sym, newI, newJ);
        cout << "BattleShip: " << sym << " moved to (" << newI + 1 << ", " << newJ + 1 << ")" << endl;

        // Update ship's location
        setLocation(newI, newJ);
        }

    void actionRamming() override {
        pair<int, int> location = getLocation();
        int i = location.first;
        int j = location.second;

        vector<pair<int, int>> ram = { {1, 0}, {0, 1}, {-1, 0}, {0, -1}};
        srand(time(0));
        int ran = rand() % ram.size();

        int newI = i + ram[ran].first;
        int newJ = j + ram[ran].second;

        // Ensure the target is within bounds
        if (newI >= 0 && newI < gameMap.size() && newJ >= 0 && newJ < gameMap[0].size()) {
            string &target = gameMap[newI][newJ];

            // Check if the target is not an ally and not an island
            if (!target.empty() && target != "1" && !isAlly(target)) {
                cout << "Cruiser " << sym << " rammed enemy " << target << " at (" << newI + 1 << ", " << newJ + 1 << ")" << endl;
                target = ""; // Clear the target
                killIncreament();
                cout<<sym<<" Kill: "<<kill<<endl;
            }
            else{cout<<"Cruiser " << sym << " rammed nothing at (" << newI + 1 << ", " << newJ + 1 << ")"<<endl;}
        }
    }
};

class Destroyer : public movingShip,public shootingShip,public rammingShip {
 public:
    void action() override {
        actionMoving();
        actionShooting();
        actionRamming();
    }

 
      Destroyer(string sym, vector<vector<string>> &gameMap,int i,int j)
        : ship(sym, -1, -1, gameMap) {
        placeShip(gameMap, sym, i, j);
    }

       void actionMoving() override {
        pair<int, int> location = getLocation();
        int i = location.first;
        int j = location.second;
        int iter = 0;

        int newI, newJ;
        bool moved = false; // Flag to track if the ship successfully moved

        do {
            newI = i + (rand() % 3 - 1); // Generate new random position
            newJ = j + (rand() % 3 - 1);

            if (isWithinBound(newI, newJ) && gameMap[newI][newJ].empty()) {
                // Position is within bounds and not occupied
                moved = true;
                break;
            }
            iter++;
        } while (iter < 100); // Stop after 100 iterations

        if (!moved) {
            // No valid moves after 100 attempts
            cout << "BattleShip: " << sym << " could not move due to lack of valid positions." << endl;
            return;
        }

        // Clear old position
        gameMap[i][j] = "";

        // Place ship at new position
        placeShip(gameMap, sym, newI, newJ);
        cout << "BattleShip: " << sym << " moved to (" << newI + 1 << ", " << newJ + 1 << ")" << endl;

        // Update ship's location
        setLocation(newI, newJ);
    }

    void actionShooting() override{
         pair<int, int> location = getLocation();
    int i = location.first;
    int j = location.second;

    vector<pair<int, int>> shoot = {{1, 1}, {1, 0}, {0, 1}, {-1, 0}, {-1, -1}, {1, -1}, {-1, 1}, {0, -1}};

    for (int count = 0; count < 3; count++) { // Allow up to 3 shots
        srand(time(0) + count); // Different seed for randomness
        int ran = rand() % shoot.size(); // Random index within the shoot vector

        int newI = i + shoot[ran].first;
        int newJ = j + shoot[ran].second;

        // Ensure the target is within bounds
        if (newI >= 0 && newI < gameMap.size() && newJ >= 0 && newJ < gameMap[0].size()) {
            string &target = gameMap[newI][newJ];
            
            // Check if the target is not an ally and not an island
            if (!target.empty() && target != "1" && !isAlly(target)) {
                cout << "Destroyer " << sym << " shoots at (" << newI + 1 << ", " << newJ + 1 
                     << ") and hits enemy " << target << endl;
                target = ""; // Clear the target
                killIncreament();
                cout<<sym<<" Kill: "<<kill<<endl;
            } else {
                cout << "Destroyer  " << sym << " shoots at (" << newI + 1 << ", " << newJ + 1 
                     << ") but misses." << endl;
            }
        } else {
            cout << "Destroyer " << sym << " shoots out of bounds at (" 
                 << newI + 1 << ", " << newJ + 1 << ")." << endl;
        }
       }
    }

    void actionRamming() override {
        pair<int, int> location = getLocation();
        int i = location.first;
        int j = location.second;

        vector<pair<int, int>> ram = { {1, 0}, {0, 1}, {-1, 0}, {0, -1}};
        srand(time(0));
        int ran = rand() % ram.size();

        int newI = i + ram[ran].first;
        int newJ = j + ram[ran].second;

        // Ensure the target is within bounds
        if (newI >= 0 && newI < gameMap.size() && newJ >= 0 && newJ < gameMap[0].size()) {
            string &target = gameMap[newI][newJ];

            // Check if the target is not an ally and not an island
            if (!target.empty() && target != "1" && !isAlly(target)) {
                cout << "Destroyer  " << sym << " rammed enemy " << target << " at (" << newI + 1 << ", " << newJ + 1 << ")" << endl;
                target = ""; // Clear the target
                killIncreament();
                cout<<sym<<" Kill: "<<kill<<endl;
            }
            else{cout<<"Destroyer " << sym << " rammed nothing at (" << newI + 1 << ", " << newJ + 1 << ")"<<endl;}
        }
    }
};

class Frigate : public shootingShip {
public:
    void action() override {
        actionShooting();
    }
     Frigate(string sym, vector<vector<string>> &gameMap)
        : ship(sym, -1, -1, gameMap) {
        generateShip(sym);
    }
    void actionShooting() override {
        static int count = 0;
        pair<int, int> location = getLocation();
        int i = location.first;
        int j = location.second;

        vector<pair<int, int>> shoot = {{1, 1}, {1, 0}, {0, 1}, {-1, 0}, {-1, -1}, {1, -1}, {-1, 1}, {0, -1}};

        for (int count = 0; count < 3; count++) { // Allow up to 3 shots
            srand(time(0) + count); // Different seed for randomness
            int ran = rand() % shoot.size(); // Random index within the shoot vector

            int newI = i + shoot[ran].first;
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

ship* upgradeShip(ship* oldShip, vector<vector<string>> &gameMap, queue<string> &destroyerShips) {
    if (!destroyerShips.empty() && oldShip->getkill() >= 3) {
        string newSym = destroyerShips.front();
        destroyerShips.pop();
         
         int i=oldShip->getLocation().first;
         int j=oldShip->getLocation().second;
        // Create a new Destroyer with the new symbol
        ship* newShip = new Destroyer(newSym, gameMap,i,j);
        ship* oldship = oldShip;

        // Transfer the state from the old ship to the new ship
        newShip->setLocation(oldShip->getLocation().first, oldShip->getLocation().second);
        newShip->setLife(oldShip->getLife());
        newShip->setkill(oldShip->getkill());
        cout<<"Ship "<<oldShip->getSym()<<" upgraded to Destroyer "<<newSym<<endl;

        // Delete the old ship
        delete oldship;

        return newShip;
    }
    return oldShip; // Return the old ship if no upgrade is needed
}

// Main function
int main() {
    srand(time(0)); // Seed the random number generator

    // ofstream outFile("output.txt");
    // if (!outFile) {
    //     cerr << "Error opening output.txt" << endl;
    //     return 1;
    // }
    // cout.rdbuf(outFile.rdbuf()); // Redirect cout to file

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
    list<ship*> AShips;
    list<ship*> Bships;
    queue<string> destroyerShips;
    queue<string> frigateShips;
    queue<ship*> destroyedShips;
    for (int i = 0; i < Asym.size(); i++) {
        if (Asym[i][0] == '*') {
            AShips.push_back(new Battleship(Asym[i], gameMap));
        } else if (Asym[i][0] == '$') {
            AShips.push_back(new Cruiser(Asym[i], gameMap));
        }  
        else if (Asym[i][0] == '#') {
             destroyerShips.push(Asym[i]);
         } else if (Asym[i][0] == '@') {
             frigateShips.push(Asym[i]);
        }
        else {
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
   // Simulate iterations
    for (int iter = 0; iter < iterations; iter++) {
    cout << "\nIteration " << iter + 1 << ":" << endl;

    // Team A actions
    for (auto it = AShips.begin(); it != AShips.end();) {
        ship* shipPtr = *it;

        // Check if the ship should be upgraded
        if (shipPtr->getkill() >= 3 && !shipPtr->isUpgraded()) {
            // Upgrade the ship
            ship* upgradedShip = upgradeShip(shipPtr, gameMap, destroyerShips);
           
            it = AShips.erase(it); // Remove the old ship from the list
            it = AShips.insert(it, upgradedShip); // Insert the upgraded ship
            (*it)->markAsUpgraded(); // Mark the upgraded ship
            ++it; // Move to the next ship
            continue; // Skip the rest of the loop for this ship
        }

        // Perform action only if the ship is alive
        if (shipPtr->checkStatus()) {
            shipPtr->action(); // Perform action
            ++it;
        } else {
            // Ship is destroyed
            cout << "Ship " << shipPtr->getSym() << " from Team A has been destroyed." << endl;
             delete shipPtr;
            it = AShips.erase(it); // Remove ship from list
        }
    }

    // Team B actions
    for (auto it = Bships.begin(); it != Bships.end();) {
        ship* shipPtr = *it;
        if (shipPtr->checkStatus()) {
            shipPtr->action(); // Perform action only if alive
            ++it;
        } else {
            cout << "Ship " << shipPtr->getSym() << " from Team B has been destroyed." << endl;
            delete shipPtr;
            it = Bships.erase(it); // Remove ship from list
            
        }
    }

    // Print the updated map
    config.printMap(gameMap);
    cout << endl;

    // Check win conditions
    if (Bships.empty()) {
        cout << "Team A wins!" << endl;
        break; // End the game
    }
    if (AShips.empty()) {
        cout << "Team B wins!" << endl;
        break; // End the game
    }
}
    // Clean up remaining ships in AShips
    for (auto shipPtr : AShips) {
        delete shipPtr;
    }
    AShips.clear();

    // Clean up remaining ships in Bships
    for (auto shipPtr : Bships) {
        delete shipPtr;
    }
    Bships.clear();
    return 0;
}