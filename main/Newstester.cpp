//一个船一个turn

#include <iostream>
#include <iomanip>
#include "vector.h"
#include "list.h"
#include "queue.h"
#include <tuple>
#include <ctime>
#include "gameconfig.h"

using namespace std;

class ship {
protected:
    vector<vector<string>> &gameMap;
    string sym; 
    int posI = -1; 
    int posJ = -1; 
    int life = 3; 
    int kill = 0; 
    bool upgraded = false;
    
    

public:
    ship(string sym, int i, int j, vector<vector<string>> &mapRef, list<ship*> &enemies, queue<ship*> &Queue)
        : sym(sym), posI(i), posJ(j), gameMap(mapRef), enemiesShips(enemies), respawnQueue(Queue) {}
    queue<ship*> &respawnQueue; 
    list<ship*> &enemiesShips;
    virtual ~ship() {}

    virtual void action() = 0;


    void generateShip(const string &sym) {
        int ranWid, ranHei;
        int attempts = 0;
        do {
            ranWid = rand() % gameMap[0].size();
            ranHei = rand() % gameMap.size();
            attempts++;
            if (attempts > 100) { // Prevent infinite loop
                cout << "Could not place ship " << sym << ". Map might be full." << endl;
                return;
            }
        } while (!gameMap[ranHei][ranWid].empty());

        gameMap[ranHei][ranWid] = sym; 
        setLocation(ranHei, ranWid);
        cout << "Placed ship " << sym << " at (" << ranHei + 1 << ", " << ranWid + 1 << ")" << endl;
    }

    void placeShip(vector<vector<string>> &gameMap, const string &sym, int i, int j) {
        gameMap[i][j] = sym; 
        setLocation(i, j);
    }

    void setLocation(int i, int j) { posI = i; posJ = j; }
    pair<int, int> getLocation() { return {posI, posJ}; }

    void setLife(int Life) { life = Life; }

    
    int getLife() { return life; }

    void setKill(int Kill) { kill = Kill; }
    int getKill() { return kill; }

    string getSym() { return sym; }

    bool isWithinBound(int newI, int newJ) {
        return newI >= 0 && newI < gameMap.size() 
            && newJ >= 0 && newJ < gameMap[0].size() 
            && gameMap[newI][newJ].empty();
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

    void killIncrement() { kill++; }

    void lifeDecrement() { life--; }

    bool checkStatus() {
        if (life <= 0) {
            gameMap[posI][posJ] = "";
            return false;
        }
        return true;
    }

    void resetShip() {
        generateShip(sym);
    }
    
    void markAsUpgraded() { upgraded = true; }

    // Check if the ship has been upgraded
    bool isUpgraded() const { return upgraded; }
    

};

class seeingShip : public virtual ship {
public:
    virtual ~seeingShip() {}
    virtual void actionSeeing() = 0;
};

class movingShip : public virtual ship {
public:
    virtual ~movingShip() {}
    virtual void actionMoving() = 0;
};

class rammingShip : public virtual ship {
public:
    virtual ~rammingShip() {}
    virtual void actionRamming() = 0;
};

class shootingShip : public virtual ship {
public:
    virtual ~shootingShip() {}
    virtual void actionShooting() = 0;
};

class Battleship : public seeingShip,public movingShip, public shootingShip {
    public:
    Battleship(string sym, vector<vector<string>> &gameMap, list<ship*> &enemies, queue<ship*> &respawnQueue)
        : ship(sym, -1, -1, gameMap, enemies, respawnQueue) {
        generateShip(sym);
    }

    void action() override {
        actionSeeing();
        actionMoving();
        actionShooting();
    }

    void actionSeeing() override {
        pair<int, int> location = getLocation();
        int i = location.first;
        int j = location.second;

        cout<<"Battleship "<<sym<<" position is ("<<i<<","<<j<<")"<<endl;
    }

    void actionMoving() override {
        pair<int, int> location = getLocation();
        int i = location.first;
        int j = location.second;
        int iter = 0;

        int newI, newJ;
        bool moved = false;

        do {
            do{
                newI = i + (rand() % 3 - 1);
                newJ = j + (rand() % 3 - 1);
            }while(newI==i && newJ==j);

            if (isWithinBound(newI, newJ) && gameMap[newI][newJ].empty()) {
                moved = true;
                break;
            }
            iter++;
        } while (iter < 100);

        if (!moved) {
            cout << "BattleShip: " << sym << " could not move." << endl;
            return;
        }

        gameMap[i][j] = "";
        placeShip(gameMap, sym, newI, newJ);
        setLocation(newI, newJ);
        cout << "BattleShip: " << sym << " moved to (" << newI + 1 << ", " << newJ + 1 << ")" << endl;
    }

    void actionShooting() override {
        pair<int, int> location = getLocation();
        int i = location.first;
        int j = location.second;

        const vector<pair<std::pair<int, int>, string>> shootDirections = {
            {{1, 1}, "South-East"}, {{1, 0}, "South"}, {{0, 1}, "East"},
            {{-1, 0}, "North"}, {{-1, -1}, "North-West"}, {{1, -1}, "South-West"},
            {{-1, 1}, "North-East"}, {{0, -1}, "West"}
        };

        for (int count = 0; count < 2; count++) { // Allow up to 2 shots
            int ran = rand() % shootDirections.size();
            pair<int, int> direction = shootDirections[ran].first;
            string directionName = shootDirections[ran].second;

            int newI = i + direction.first;
            int newJ = j + direction.second;

            if (newI >= 0 && newI < gameMap.size() && newJ >= 0 && newJ < gameMap[0].size()) {
                string &target = gameMap[newI][newJ];

                if (!target.empty() && target != "1" && !isAlly(target)) {
                  for (auto it = enemiesShips.begin(); it != enemiesShips.end(); ) {
                        ship* enemyShip = *it;
                        pair<int, int> enemyLoc = enemyShip->getLocation();

                        if (enemyLoc.first == newI && enemyLoc.second == newJ) {
                            enemyShip->lifeDecrement();
                            cout << "Ship " << getSym() << " shoots " << directionName 
                                << " at (" << newI + 1 << ", " << newJ + 1 << ")" << endl;
                            cout << enemyShip->getSym() << "'s life is now " << enemyShip->getLife() << endl;

                            if (enemyShip->getLife() > 0) {
                                enemyShip->setLocation(-1, -1);
                                gameMap[newI][newJ] = "";
                                killIncrement();
                                cout << sym << " Kill: " << kill << endl;
                                respawnQueue.push(enemyShip);
                                it = enemiesShips.erase(it); // Erase properly
                            } else {
                                cout << "  " << enemyShip->getSym() << " is permanently destroyed!" << endl;
                                enemyShip->setLocation(-1, -1);
                                gameMap[newI][newJ] = "";
                                killIncrement();
                                cout << sym << " Kill: " << kill << endl;
                                it = enemiesShips.erase(it);
                            }
                            break;
                        } else {
                            ++it;
                        }
                    }
                } else {
                    cout << "Battleship " << sym << " shoots at (" << newI + 1 << ", " << newJ + 1 
                        << ") but misses."<< endl;
                }
            }
        }
        
    }
    
};

class Cruiser : public seeingShip,public movingShip, public rammingShip{
    public:
    Cruiser(string sym, vector<vector<string>> &gameMap, list<ship*> &enemies, queue<ship*> &respawnQueue)
        : ship(sym, -1, -1, gameMap, enemies, respawnQueue) {
        generateShip(sym);
    }

    void action() override {
        actionSeeing();
        actionMoving();
        actionRamming();
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
                    cout << "Cruiser " << sym << " detected enemy ship " << cellContent << " at (" << newI + 1 << ", " << newJ + 1 << ")" << endl;
                } else {
                    cout << "Cruiser " << sym << " detected ally ship " << cellContent << " at (" << newI + 1 << ", " << newJ + 1 << ")" << endl;
                    }
                }
                else if(cellContent.empty() && cellContent == "1"){
                    cout << "Cruiser " << sym << " detected nothing  "<< endl;
                }
            }
        }
    }

    void actionMoving() override {
        pair<int, int> location = getLocation();
        int i = location.first;
        int j = location.second;
        int iter = 0;

        int newI, newJ;
        bool moved = false;

        do {
            do{newI = i + (rand() % 3 - 1);
                newJ = j + (rand() % 3 - 1);
            }while(newI==i && newJ==j);

            if (isWithinBound(newI, newJ) && gameMap[newI][newJ].empty()) {
                moved = true;
                break;
            }
            iter++;
        } while (iter < 100);

        if (!moved) {
            cout << "Cruiser: " << sym << " could not move." << endl;
            return;
        }

        gameMap[i][j] = "";
        placeShip(gameMap, sym, newI, newJ);
        setLocation(newI, newJ);
        cout << "Cruiser: " << sym << " moved to (" << newI + 1 << ", " << newJ + 1 << ")" << endl;
    }

     void actionRamming() override {
        pair<int, int> location = getLocation();
        int i = location.first;
        int j = location.second;

        vector<pair<int, int>> directions = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
        int dir = rand() % directions.size();
        int newI = i + directions[dir].first;
        int newJ = j + directions[dir].second;

        if (newI < 0 || newI >= gameMap.size() || newJ < 0 || newJ >= gameMap[0].size()) return;

        string &targetSymbol = gameMap[newI][newJ];

        if (!targetSymbol.empty() && targetSymbol != "1" && !isAlly(targetSymbol)) {
            ship* enemyShip = nullptr;
            auto it = enemiesShips.begin();
            while (it != enemiesShips.end()) {
                pair<int, int> enemyLoc = (*it)->getLocation();
                if (enemyLoc.first == newI && enemyLoc.second == newJ) {
                    enemyShip = *it;
                    break;
                }
                ++it;
            }

            if (enemyShip) {
                enemyShip->lifeDecrement();
                cout << sym << " rammed " << targetSymbol << " at (" << newI+1 << ", " << newJ+1 << "). ";
                cout << targetSymbol << "'s life is now " << enemyShip->getLife() << endl;

                if (enemyShip->getLife() != 0) {
                    gameMap[newI][newJ] = "";
                    enemyShip->setLocation(-1, -1);
                    respawnQueue.push(enemyShip);
                    enemiesShips.erase(it);
                    killIncrement();
                    gameMap[i][j] = "";
                    setLocation(newI, newJ);
                    gameMap[newI][newJ] = sym;
                    cout << sym << " moved to (" << newI+1 << ", " << newJ+1 << ") after ramming." << endl;
                } else {
                    cout<<"  "<<targetSymbol<<" is permanently destroyed!"<<endl;
                    gameMap[newI][newJ] = "";
                    enemyShip->setLocation(-1, -1);
                    enemiesShips.erase(it);
                }
                cout << sym << " Kill: " << kill << endl;
            }
        }
        else{cout<<"Cruiser "<<sym<<" rammed at ("<<newI+1<<", "<<newJ+1<<") but misses."<<endl;}
    }
};

class Destroyer : public movingShip,public shootingShip, public rammingShip{
    public:
    Destroyer(string sym, vector<vector<string>> &gameMap, list<ship*> &enemies, queue<ship*> &respawnQueue)
        : ship(sym, -1, -1, gameMap, enemies, respawnQueue) {
        generateShip(sym);
    }

    void action() override {
        actionMoving();
        actionShooting();
        actionRamming();
    }

    void actionMoving() override {
        pair<int, int> location = getLocation();
        int i = location.first;
        int j = location.second;
        int iter = 0;

        int newI, newJ;
        bool moved = false;

        do {
            newI = i + (rand() % 3 - 1);
            newJ = j + (rand() % 3 - 1);

            if (isWithinBound(newI, newJ) && gameMap[newI][newJ].empty()) {
                moved = true;
                break;
            }
            iter++;
        } while (iter < 100);

        if (!moved) {
            cout << "Destroyer: " << sym << " could not move." << endl;
            return;
        }

        gameMap[i][j] = "";
        placeShip(gameMap, sym, newI, newJ);
        setLocation(newI, newJ);
        cout << "Destroyer: " << sym << " moved to (" << newI + 1 << ", " << newJ + 1 << ")" << endl;
    }


     void actionShooting() override {
        pair<int, int> location = getLocation();
        int i = location.first;
        int j = location.second;

        const vector<pair<std::pair<int, int>, string>> shootDirections = {
            {{1, 1}, "South-East"}, {{1, 0}, "South"}, {{0, 1}, "East"},
            {{-1, 0}, "North"}, {{-1, -1}, "North-West"}, {{1, -1}, "South-West"},
            {{-1, 1}, "North-East"}, {{0, -1}, "West"}
        };

        for (int count = 0; count < 2; count++) { // Allow up to 2 shots
            int ran = rand() % shootDirections.size();
            pair<int, int> direction = shootDirections[ran].first;
            string directionName = shootDirections[ran].second;

            int newI = i + direction.first;
            int newJ = j + direction.second;

            if (newI >= 0 && newI < gameMap.size() && newJ >= 0 && newJ < gameMap[0].size()) {
                string &target = gameMap[newI][newJ];

                if (!target.empty() && target != "1" && !isAlly(target)) {
                    for (auto it = enemiesShips.begin(); it != enemiesShips.end(); ) {
                        ship* enemyShip = *it;
                        pair<int, int> enemyLoc = enemyShip->getLocation();

                        if (enemyLoc.first == newI && enemyLoc.second == newJ) {
                            enemyShip->lifeDecrement();
                            cout << "Ship " << getSym() << " shoots " << directionName 
                                << " at (" << newI + 1 << ", " << newJ + 1 << ")" << endl;
                            cout << enemyShip->getSym() << "'s life is now " << enemyShip->getLife() << endl;

                            if (enemyShip->getLife() > 0) {
                                enemyShip->setLocation(-1, -1);
                                gameMap[newI][newJ] = "";
                                killIncrement();
                                cout << sym << " Kill: " << kill << endl;
                                respawnQueue.push(enemyShip);
                                it = enemiesShips.erase(it); // Erase properly
                            } else {
                                cout << "  " << enemyShip->getSym() << " is permanently destroyed!" << endl;
                                enemyShip->setLocation(-1, -1);
                                gameMap[newI][newJ] = "";
                                killIncrement();
                                cout << sym << " Kill: " << kill << endl;
                                it = enemiesShips.erase(it);
                            }
                            break;
                        } else {
                            ++it;
                        }
                    }
                } else {
                    cout << "Destroyer " << sym << " shoots at (" << newI + 1 << ", " << newJ + 1 
                        << ") but misses."<< endl;
                }
            }
        }
        
    }

      void actionRamming() override {
        pair<int, int> location = getLocation();
        int i = location.first;
        int j = location.second;

        vector<pair<int, int>> directions = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
        int dir = rand() % directions.size();
        int newI = i + directions[dir].first;
        int newJ = j + directions[dir].second;

        if (newI < 0 || newI >= gameMap.size() || newJ < 0 || newJ >= gameMap[0].size()) return;

        string &targetSymbol = gameMap[newI][newJ];

        if (!targetSymbol.empty() && targetSymbol != "1" && !isAlly(targetSymbol)) {
            ship* enemyShip = nullptr;
            auto it = enemiesShips.begin();
            while (it != enemiesShips.end()) {
                pair<int, int> enemyLoc = (*it)->getLocation();
                if (enemyLoc.first == newI && enemyLoc.second == newJ) {
                    enemyShip = *it;
                    break;
                }
                ++it;
            }

            if (enemyShip) {
                enemyShip->lifeDecrement();
                cout << sym << " rammed " << targetSymbol << " at (" << newI+1 << ", " << newJ+1 << "). ";
                cout << targetSymbol << "'s life is now " << enemyShip->getLife() << endl;

                if (enemyShip->getLife() != 0) {
                    gameMap[newI][newJ] = "";
                    enemyShip->setLocation(-1, -1);
                    respawnQueue.push(enemyShip);
                    enemiesShips.erase(it);
                    killIncrement();
                    gameMap[i][j] = "";
                    setLocation(newI, newJ);
                    gameMap[newI][newJ] = sym;
                    cout << sym << " moved to (" << newI+1 << ", " << newJ+1 << ") after ramming." << endl;
                } else {
                    cout<<"  "<<targetSymbol<<" is permanently destroyed!"<<endl;
                    gameMap[newI][newJ] = "";
                    enemyShip->setLocation(-1, -1);
                    enemiesShips.erase(it);
                }
                cout << sym << " Kill: " << kill << endl;
            }
        }
        else{cout<<"Destroyer "<<sym<<" rammed at ("<<newI+1<<", "<<newJ+1<<") but misses."<<endl;}
    }
};

class Frigate: public shootingShip {   
private:
int currentDirectionIndex = 0;
public:
    Frigate(string sym, vector<vector<string>> &gameMap, list<ship*> &enemies, queue<ship*> &respawnQueue)
        : ship(sym, -1, -1, gameMap, enemies, respawnQueue) {
        generateShip(sym);
    }

    void action() override {
        actionShooting();
    }

   
     void actionShooting() override {
    pair<int, int> location = getLocation();
    int i = location.first;
    int j = location.second;

    // Define possible shooting directions
    const vector<pair<std::pair<int, int>, string>> shootDirections = {
        {{1, 1}, "South-East"},
        {{1, 0}, "South"},
        {{0, 1}, "East"},
        {{-1, 0}, "North"},
        {{-1, -1}, "North-West"},
        {{1, -1}, "South-West"},
        {{-1, 1}, "North-East"},
        {{0, -1}, "West"}
    };

    // Get the current direction
    pair<int, int> direction = shootDirections[currentDirectionIndex].first;
    string directionName = shootDirections[currentDirectionIndex].second;

    // Calculate the target location based on the current location and the selected direction
    int newI = i + direction.first;
    int newJ = j + direction.second;

    // Check if the new position is within bounds
    if (newI >= 0 && newI < gameMap.size() && newJ >= 0 && newJ < gameMap[0].size()) {
        string &targetSymbol = gameMap[newI][newJ];

        if (!targetSymbol.empty() && targetSymbol != "1" && !isAlly(targetSymbol)) {
            ship* enemyShip = nullptr;
            auto it = enemiesShips.begin();

            while (it != enemiesShips.end()) {
                pair<int, int> enemyLoc = (*it)->getLocation();
                if (enemyLoc.first == newI && enemyLoc.second == newJ) {
                    enemyShip = *it;
                    break;
                }
                ++it;
            }

            if (enemyShip) {
                enemyShip->lifeDecrement();
                cout << "Frigate " << getSym() << " shooting in direction: " 
                     << directionName << " to location: (" << newI + 1 << ", " << newJ + 1 << ")" << endl;
                cout << targetSymbol << "'s life is now " << enemyShip->getLife() << endl;

                if (enemyShip->getLife() != 0) {
                    cout << "  " << targetSymbol << " is permanently destroyed!" << endl;
                    gameMap[newI][newJ] = "";
                    enemyShip->setLocation(-1, -1);
                    // Increase kill count for the attacking ship
                    killIncrement();
                    cout << sym << " Kill: " << kill << endl;

                    // Move enemy to respawn queue and remove from list
                    respawnQueue.push(enemyShip);
                    it = enemiesShips.erase(it); // Fix iterator issue

                    cout << targetSymbol << " added to respawn queue." << endl;
                }
                else{
                    cout<<"  "<<targetSymbol<<" is permanently destroyed!"<<endl;
                    gameMap[newI][newJ] = "";
                    enemyShip->setLocation(-1, -1);
                    killIncrement();
                    cout << sym << " Kill: " << kill << endl;
                    it = enemiesShips.erase(it);
                }
            }
        }
        else {
        cout << "Frigate " << getSym() << " shooting in direction: "
             << directionName << " but misses." << endl;
       }
    }

    // Increment the direction index for the next round
    currentDirectionIndex = (currentDirectionIndex + 1) % shootDirections.size();
   }
};

class Supership : public movingShip,public shootingShip, public rammingShip{
    public:
    Supership(string sym, vector<vector<string>> &gameMap, list<ship*> &enemies, queue<ship*> &respawnQueue)
        : ship(sym, -1, -1, gameMap, enemies, respawnQueue) {
        generateShip(sym);
    }

    void action() override {
        actionMoving();
        actionShooting();
        actionRamming();
    }

    void actionMoving() override {
        pair<int, int> location = getLocation();
        int i = location.first;
        int j = location.second;
        int iter = 0;

        int newI, newJ;
        bool moved = false;

        do {
            do{
                newI = i + (rand() % 3 - 1);
                newJ = j + (rand() % 3 - 1);
            }while(newI==i && newJ==j);

            if (isWithinBound(newI, newJ) && gameMap[newI][newJ].empty()) {
                moved = true;
                break;
            }
            iter++;
        } while (iter < 100);

        if (!moved) {
            cout << "Supership: " << sym << " could not move." << endl;
            return;
        }

        gameMap[i][j] = "";
        placeShip(gameMap, sym, newI, newJ);
        setLocation(newI, newJ);
        cout << "Supership: " << sym << " moved to (" << newI + 1 << ", " << newJ + 1 << ")" << endl;
    }


     void actionShooting() override {
        pair<int, int> location = getLocation();
        int i = location.first;
        int j = location.second;

        const vector<pair<std::pair<int, int>, string>> shootDirections = {
            {{1, 1}, "South-East"}, {{1, 0}, "South"}, {{0, 1}, "East"},
            {{-1, 0}, "North"}, {{-1, -1}, "North-West"}, {{1, -1}, "South-West"},
            {{-1, 1}, "North-East"}, {{0, -1}, "West"}
        };

        for (int count = 0; count < 2; count++) { // Allow up to 2 shots
            int ran = rand() % shootDirections.size();
            pair<int, int> direction = shootDirections[ran].first;
            string directionName = shootDirections[ran].second;

            int newI = i + direction.first;
            int newJ = j + direction.second;

            if (newI >= 0 && newI < gameMap.size() && newJ >= 0 && newJ < gameMap[0].size()) {
                string &target = gameMap[newI][newJ];

                if (!target.empty() && target != "1" && !isAlly(target)) {
                    for (auto it = enemiesShips.begin(); it != enemiesShips.end(); ) {
                        ship* enemyShip = *it;
                        pair<int, int> enemyLoc = enemyShip->getLocation();

                        if (enemyLoc.first == newI && enemyLoc.second == newJ) {
                            enemyShip->lifeDecrement();
                            cout << "Ship " << getSym() << " shoots " << directionName 
                                << " at (" << newI + 1 << ", " << newJ + 1 << ")" << endl;
                            cout << enemyShip->getSym() << "'s life is now " << enemyShip->getLife() << endl;

                            if (enemyShip->getLife() > 0) {
                                enemyShip->setLocation(-1, -1);
                                gameMap[newI][newJ] = "";
                                killIncrement();
                                cout << sym << " Kill: " << kill << endl;
                                respawnQueue.push(enemyShip);
                                it = enemiesShips.erase(it); // Erase properly
                            } else {
                                cout << "  " << enemyShip->getSym() << " is permanently destroyed!" << endl;
                                enemyShip->setLocation(-1, -1);
                                gameMap[newI][newJ] = "";
                                killIncrement();
                                cout << sym << " Kill: " << kill << endl;
                                it = enemiesShips.erase(it);
                            }
                            break;
                        } else {
                            ++it;
                        }
                    }
                } else {
                    cout << "Supership " << sym << " shoots at (" << newI + 1 << ", " << newJ + 1 
                        << ") but misses."<< endl;
                }
            }
        }
        
    }

     void actionRamming() override {
        pair<int, int> location = getLocation();
        int i = location.first;
        int j = location.second;

        vector<pair<int, int>> directions = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
        int dir = rand() % directions.size();
        int newI = i + directions[dir].first;
        int newJ = j + directions[dir].second;

        if (newI < 0 || newI >= gameMap.size() || newJ < 0 || newJ >= gameMap[0].size()) return;

        string &targetSymbol = gameMap[newI][newJ];

        if (!targetSymbol.empty() && targetSymbol != "1" && !isAlly(targetSymbol)) {
            ship* enemyShip = nullptr;
            auto it = enemiesShips.begin();
            while (it != enemiesShips.end()) {
                pair<int, int> enemyLoc = (*it)->getLocation();
                if (enemyLoc.first == newI && enemyLoc.second == newJ) {
                    enemyShip = *it;
                    break;
                }
                ++it;
            }

            if (enemyShip) {
                enemyShip->lifeDecrement();
                cout << sym << " rammed " << targetSymbol << " at (" << newI+1 << ", " << newJ+1 << "). ";
                cout << targetSymbol << "'s life is now " << enemyShip->getLife() << endl;

                if (enemyShip->getLife() != 0) {
                    cout << "  " << targetSymbol << " is permanently destroyed!" << endl;
                    gameMap[newI][newJ] = "";
                    respawnQueue.push(enemyShip);
                    enemiesShips.erase(it);
                    killIncrement();
                    gameMap[i][j] = "";
                    setLocation(newI, newJ);
                    gameMap[newI][newJ] = sym;
                    cout << sym << " moved to (" << newI+1 << ", " << newJ+1 << ") after ramming." << endl;
                } else {
                    cout<<"  "<<targetSymbol<<" is permanently destroyed!"<<endl;
                    enemiesShips.erase(it);
                }
                cout << sym << " Kill: " << kill << endl;
            }
        }
        else{cout<<"Supership "<<sym<<" rammed at ("<<newI+1<<", "<<newJ+1<<") but misses."<<endl;}
    }
};

class Corvette : public shootingShip {
public:
    Corvette(string sym, vector<vector<string>>& gameMap, list<ship*>& enemies, queue<ship*>& respawnQueue)
        : ship(sym, -1, -1, gameMap, enemies, respawnQueue) {
        generateShip(sym); // Place the Corvette on the map
    }

    void action() override {
        actionShooting(); // Corvette only shoots (no movement)
    }

    void actionShooting() override {
        pair<int, int> location = getLocation();
        int i = location.first;
        int j = location.second;

        // Define adjacent directions (8 possible)
        const vector<pair<pair<int, int>, string>> directions = {
            {{-1, -1}, "North-West"}, {{-1, 0}, "North"}, {{-1, 1}, "North-East"},
            {{0, -1}, "West"},         {{0, 1}, "East"},
            {{1, -1}, "South-West"},  {{1, 0}, "South"}, {{1, 1}, "South-East"}
        };

        // Shoot once at a random adjacent cell
        int dirIdx = rand() % directions.size();
        auto direction = directions[dirIdx].first;
        string dirName = directions[dirIdx].second;

        int newI = i + direction.first;
        int newJ = j + direction.second;

        // Check if the target is valid
        if (newI >= 0 && newI < gameMap.size() && newJ >= 0 && newJ < gameMap[0].size()) {
            string& target = gameMap[newI][newJ];

            // Skip allies and islands
            if (!target.empty() && target != "1" && !isAlly(target)) {
                ship* enemyShip = nullptr;
                auto it = enemiesShips.begin();

                // Find the enemy ship at the target location
                while (it != enemiesShips.end()) {
                    pair<int, int> enemyLoc = (*it)->getLocation();
                    if (enemyLoc.first == newI && enemyLoc.second == newJ) {
                        enemyShip = *it;
                        break;
                    }
                    ++it;
                }

                if (enemyShip) {
                    enemyShip->lifeDecrement();
                    cout << "Corvette " << sym << " shoots " << dirName << " to ("
                         << newI + 1 << ", " << newJ + 1 << ")" << endl;
                    cout << enemyShip->getSym() << "'s life: " << enemyShip->getLife() << endl;

                    // Handle destruction
                    if (enemyShip->getLife() != 0) {
                    cout << "  " << enemyShip->getSym() << " is permanently destroyed!" << endl;
                    gameMap[newI][newJ] = "";
                    enemyShip->setLocation(-1, -1);
                    // Increase kill count for the attacking ship
                    killIncrement();
                    cout << sym << " Kill: " << kill << endl;

                    // Move enemy to respawn queue and remove from list
                    respawnQueue.push(enemyShip);
                    it = enemiesShips.erase(it); // Fix iterator issue

                    cout << enemyShip->getSym() << " added to respawn queue." << endl;
                    }
                    else{
                        cout<<"  "<<enemyShip->getSym()<<" is permanently destroyed!"<<endl;
                        gameMap[newI][newJ] = "";
                        enemyShip->setLocation(-1, -1);
                        killIncrement();
                        cout << sym << " Kill: " << kill << endl;
                        it = enemiesShips.erase(it);
                    }
                }
            } else {
                cout << "Corvette " << sym << " shoots " << dirName << " but misses." << endl;
            }
        } else {
            cout << "Corvette " << sym << " shoots " << dirName << " out of bounds." << endl;
        }
    }
};

class Amphibious : public movingShip, public  shootingShip{
public:
    Amphibious(string sym, vector<vector<string>>& gameMap, list<ship*>& enemies, queue<ship*>& respawnQueue)
        : ship(sym, -1, -1, gameMap, enemies, respawnQueue) {
        generateShip(sym);
    }

    void action() override {
        actionMoving();
        actionShooting();
    }

    void actionMoving() override {
        pair<int, int> location = getLocation();
        int i = location.first;
        int j = location.second;
        int iter = 0;

        int newI, newJ;
        bool moved = false;

        do {
            do{newI = i + (rand() % 3 - 1);
                newJ = j + (rand() % 3 - 1);
            }while(newI==i && newJ==j&&(gameMap[newI][newJ]=="*"||gameMap[newI][newJ]=="$"||gameMap[newI][newJ]=="#"||gameMap[newI][newJ]=="@"||gameMap[newI][newJ]=="<"||gameMap[newI][newJ]==">"));

            if (newI >= 0 && newI < gameMap.size() && newJ >= 0 && newJ < gameMap[0].size()){
                moved = true;
                break;
            }
            iter++;
        } while (iter < 100);

        if (!moved) {
            cout << "Amphibious: " << sym << " could not move." << endl;
            return;
        }

        gameMap[i][j] = "";
        placeShip(gameMap, sym, newI, newJ);
        setLocation(newI, newJ);
        cout << "Amphibious: " << sym << " moved to (" << newI + 1 << ", " << newJ + 1 << ")" << endl;
    }

    void actionShooting() override {
        pair<int, int> location = getLocation();
        int i = location.first;
        int j = location.second;

        const vector<pair<std::pair<int, int>, string>> shootDirections = {
            {{1, 1}, "South-East"}, {{1, 0}, "South"}, {{0, 1}, "East"},
            {{-1, 0}, "North"}, {{-1, -1}, "North-West"}, {{1, -1}, "South-West"},
            {{-1, 1}, "North-East"}, {{0, -1}, "West"}
        };

        for (int count = 0; count < 2; count++) { // Allow up to 2 shots
            int ran = rand() % shootDirections.size();
            pair<int, int> direction = shootDirections[ran].first;
            string directionName = shootDirections[ran].second;

            int newI = i + direction.first;
            int newJ = j + direction.second;

            if (newI >= 0 && newI < gameMap.size() && newJ >= 0 && newJ < gameMap[0].size()) {
                string &target = gameMap[newI][newJ];

                if (!target.empty() && target != "1" && !isAlly(target)) {
                  for (auto it = enemiesShips.begin(); it != enemiesShips.end(); ) {
                        ship* enemyShip = *it;
                        pair<int, int> enemyLoc = enemyShip->getLocation();

                        if (enemyLoc.first == newI && enemyLoc.second == newJ) {
                            enemyShip->lifeDecrement();
                            cout << "Ship " << getSym() << " shoots " << directionName 
                                << " at (" << newI + 1 << ", " << newJ + 1 << ")" << endl;
                            cout << enemyShip->getSym() << "'s life is now " << enemyShip->getLife() << endl;

                            if (enemyShip->getLife() > 0) {
                                enemyShip->setLocation(-1, -1);
                                gameMap[newI][newJ] = "";
                                killIncrement();
                                cout << sym << " Kill: " << kill << endl;
                                respawnQueue.push(enemyShip);
                                it = enemiesShips.erase(it); // Erase properly
                            } else {
                                cout << "  " << enemyShip->getSym() << " is permanently destroyed!" << endl;
                                enemyShip->setLocation(-1, -1);
                                gameMap[newI][newJ] = "";
                                killIncrement();
                                cout << sym << " Kill: " << kill << endl;
                                it = enemiesShips.erase(it);
                            }
                            break;
                        } else {
                            ++it;
                        }
                    }
                } else {
                    cout << "Amphibious " << sym << " shoots at (" << newI + 1 << ", " << newJ + 1 
                        << ") but misses."<< endl;
                }
            }
        }
        
    }
};

class sniperShip : public shootingShip {
public:
    sniperShip(string sym, vector<vector<string>>& gameMap, list<ship*>& enemies, queue<ship*>& respawnQueue)
    : ship(sym, -1, -1, gameMap, enemies, respawnQueue) {
    generateShip(sym); 
    }
    void action() override {
        actionShooting();
    }
    
    void actionShooting() override{
        pair<int, int> location = getLocation();
        int i = location.first;
        int j = location.second;
  
        int newI, newJ;
        newI = i+rand()%5;
        newJ = j+rand()%5;
        
        if (newI >= 0 && newI < gameMap.size() && newJ >= 0 && newJ < gameMap[0].size()) {
            string& target = gameMap[newI][newJ];

            // Skip allies and islands
            if (!target.empty() && target != "1" && !isAlly(target)) {
                ship* enemyShip = nullptr;
                auto it = enemiesShips.begin();

                // Find the enemy ship at the target location
                while (it != enemiesShips.end()) {
                    pair<int, int> enemyLoc = (*it)->getLocation();
                    if (enemyLoc.first == newI && enemyLoc.second == newJ) {
                        enemyShip = *it;
                        break;
                    }
                    ++it;
                }

                if (enemyShip) {
                    enemyShip->lifeDecrement();
                    cout << "SniperShip " << sym << " shoots " << i<< j << " to ("
                         << newI + 1 << ", " << newJ + 1 << ")" << endl;
                    cout << enemyShip->getSym() << "'s life: " << enemyShip->getLife() << endl;

                    // Handle destruction
                    if (enemyShip->getLife() != 0) {
                    cout << "  " << enemyShip->getSym() << " is permanently destroyed!" << endl;
                    gameMap[newI][newJ] = "";
                    enemyShip->setLocation(-1, -1);
                    // Increase kill count for the attacking ship
                    killIncrement();
                    cout << sym << " Kill: " << kill << endl;

                    // Move enemy to respawn queue and remove from list
                    respawnQueue.push(enemyShip);
                    it = enemiesShips.erase(it); // Fix iterator issue

                    cout << enemyShip->getSym() << " added to respawn queue." << endl;
                    }
                    else{
                        cout<<"  "<<enemyShip->getSym()<<" is permanently destroyed!"<<endl;
                        gameMap[newI][newJ] = "";
                        enemyShip->setLocation(-1, -1);
                        killIncrement();
                        cout << sym << " Kill: " << kill << endl;
                        it = enemiesShips.erase(it);
                    }
                }
            } else {
                cout << "SniperShip " << sym << " shoots " << i << j<<" but misses." << endl;
            }
        } else {
            cout << "SniperShip " << sym << " shoots " << i << j << " out of bounds." << endl;
        }
    

    }
};

void printQueue(queue<ship*> &respawnQueue) {
    queue<ship*> temp = respawnQueue;
    while (!temp.empty()) {
        cout << temp.front()->getSym() << " ";
        temp.pop();
    }
    cout << endl;
}

ship* upgradeShip(ship* oldShip, vector<vector<string>> &gameMap, queue<ship*> &respawnQueue, list<ship*> &teamShips) {
    int randomUpgrade= rand()%3;
    if (oldShip == nullptr) {
        cerr << "Error: oldShip is null" << endl;
        return nullptr;
    }

    ship* newShip = nullptr;

    try {
        char sym = oldShip->getSym()[0]; // First character of ship symbol

        if (sym == '*' || sym == '$' || sym == '<' || sym == '>') {
            newShip = new Destroyer(oldShip->getSym(), gameMap, teamShips, respawnQueue);
            cout << "Ship " << oldShip->getSym() << " upgraded to Destroyer!" << endl;
        } 
        else if (sym == '@') {
            if(randomUpgrade==0){
                newShip = new Corvette(oldShip->getSym(), gameMap, teamShips, respawnQueue);
                cout << "Ship " << oldShip->getSym() << " upgraded to Corvette!" << endl;
            }else if(randomUpgrade==1){
                newShip = new Amphibious(oldShip->getSym(), gameMap, teamShips, respawnQueue);
                cout << "Ship " << oldShip->getSym() << " upgraded to Amphibious!" << endl;
            }
            else{
                newShip = new sniperShip(oldShip->getSym(), gameMap, teamShips, respawnQueue);
                cout << "Ship " << oldShip->getSym() << " upgraded to Supership!" << endl;
            }
        } 
        else if (sym == '#') {
            newShip = new Supership(oldShip->getSym(), gameMap, teamShips, respawnQueue);
            cout << "Ship " << oldShip->getSym() << " upgraded to Supership!" << endl;
        } 
        else {
            cerr << "Error: Unknown ship type for upgrade!" << endl;
            return nullptr;
        }

        // Preserve the old ship's attributes
        newShip->setLife(oldShip->getLife());
        newShip->setKill(oldShip->getKill());
        newShip->markAsUpgraded();
       

        delete oldShip; // Free memory of the old ship
        return newShip;

    } catch (const exception& e) {
        cerr << "Upgrade failed: " << e.what() << endl;
        return nullptr;
    }
}



int main() {
    srand(time(0)); 
    GameConfig config("game.txt");
    vector<vector<string>> gameMap = config.getGameMap();
    vector<string> Asym = config.getAsym(); // SymbolA
    vector<string> Bsym = config.getBsym(); // SymbolB
    int iterations = config.getIterations();
    ofstream outFile("tt3.txt");
    if (!outFile) {
        cerr << "Error opening output.txt" << endl;
        return 1;
    }
    cout.rdbuf(outFile.rdbuf()); // Redirect cout to file



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
    list<ship*> BShips;
    queue<ship*> respawnQueueA;
    queue<ship*> respawnQueueB;

      for (int i = 0; i < Asym.size(); i++) {
        if (Asym[i][0] == '*') {
            AShips.push_back(new Battleship(Asym[i], gameMap,BShips,respawnQueueB));
        } else if (Asym[i][0] == '$') {
            AShips.push_back(new Cruiser(Asym[i], gameMap,BShips,respawnQueueB));
        }  
        else if (Asym[i][0] == '#') {
             AShips.push_back(new Destroyer(Asym[i], gameMap,BShips,respawnQueueB));
         } else if (Asym[i][0] == '@') {
             AShips.push_back(new Frigate(Asym[i], gameMap,BShips,respawnQueueB));
        }
        else {
            cout << "Invalid symbol: " << Asym[i] << endl;
        }
    }

    for (int i = 0; i < Bsym.size(); i++) {
        if (Bsym[i][0] == '<') {
            BShips.push_back(new Battleship(Bsym[i], gameMap,AShips,respawnQueueA));
        } else if (Bsym[i][0] == '>') {
            BShips.push_back(new Cruiser(Bsym[i], gameMap,AShips,respawnQueueA));
        }  
        else {
            cout << "Invalid symbol: " << Bsym[i] << endl;
        }
    }


   for(int turn = 0; turn < iterations; turn++) {
    cout << "=== Turn " << turn + 1 << " ===" << endl;

    // Respawning for Team A
    if (!respawnQueueA.empty()) {
        ship* respawned = respawnQueueA.front();
        respawnQueueA.pop();

        if (respawned->getLife() > 0) {
            respawned->resetShip(); 
            AShips.push_back(respawned);
            cout << respawned->getSym() << " has respawned!" << endl;
        } else {
            delete respawned;
            cout << respawned->getSym() << " is permanently destroyed and removed." << endl;
        }
    }

    // Respawning for Team B
    if (!respawnQueueB.empty()) {
        ship* respawned = respawnQueueB.front();
        respawnQueueB.pop();

        if (respawned->getLife() > 0) {
            respawned->resetShip(); 
            BShips.push_back(respawned);
            cout << respawned->getSym() << " has respawned!" << endl;
        } else {
            delete respawned;
            cout << respawned->getSym() << " is permanently destroyed and removed." << endl;
        }
    }

    // Perform ship actions for Team A
    cout << "Team A ships: " << endl;
    for (auto it = AShips.begin(); it != AShips.end(); ) {
        if ((*it)->getKill() == 3 && !(*it)->isUpgraded()) {
            int i = (*it)->getLocation().first;
            int j = (*it)->getLocation().second;
            gameMap[i][j] = "";
            (*it)->setLocation(-1, -1);
            ship* upgradedShip = upgradeShip(*it, gameMap, respawnQueueA, AShips);
            it = AShips.erase(it);
            it = AShips.insert(it, upgradedShip);
            continue;
        }
        (*it)->action();
        config.printMap(gameMap);
        ++it;
    }

    // Perform ship actions for Team B
    cout << "Team B ships: " << endl;
    for (auto it = BShips.begin(); it != BShips.end(); ) {
        if ((*it)->getKill() == 3 && !(*it)->isUpgraded()) {
            int i = (*it)->getLocation().first;
            int j = (*it)->getLocation().second;
            gameMap[i][j] = "";
            (*it)->setLocation(-1, -1);
            ship* upgradedShip = upgradeShip(*it, gameMap, respawnQueueB, BShips);
            it = BShips.erase(it);
            it = BShips.insert(it, upgradedShip);
            continue;
        }
        (*it)->action();
        config.printMap(gameMap);
        ++it;
    }

    cout << "Respawn Queue A: "<<endl;
    printQueue(respawnQueueA);
    cout << "Respawn Queue B: "<<endl;
    printQueue(respawnQueueB);

    // Increment turn at the end of the loop
    

    // Check if game should end
    if (AShips.empty() && respawnQueueA.empty()) {
        cout << "Team B wins!" << endl;
        break;
    }
    if (BShips.empty() && respawnQueueB.empty()) {
        cout << "Team A wins!" << endl;
        break;
    }
}
    config.printMap(gameMap);
}


