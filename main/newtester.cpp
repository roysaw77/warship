#include <iostream>
#include <iomanip>
#include "vector.h"
#include <list>
#include <queue>
#include <tuple>
#include <ctime>
#include <algorithm>

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
        if ((sym[0] == '*' && otherSym[0] == '*') || (sym[0] == '<' && otherSym[0] == '<')) {
            return true;
        }
        return false;
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

class Battleship : public movingShip, public shootingShip {
     private:
    int currentDirectionIndex = 0;
    public:
    Battleship(string sym, vector<vector<string>> &gameMap, list<ship*> &enemies, queue<ship*> &respawnQueue)
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
            newI = i + (rand() % 3 - 1);
            newJ = j + (rand() % 3 - 1);

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

        for (int count = 0; count < 3; count++) { // Allow up to 3 shots
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
        for(auto ship : enemiesShips){
            cout<<ship->getSym()<<" "<<endl;
        }
    }
    
};

class Destroyer : public movingShip, public rammingShip{
    public:
    Destroyer(string sym, vector<vector<string>> &gameMap, list<ship*> &enemies, queue<ship*> &respawnQueue)
        : ship(sym, -1, -1, gameMap, enemies, respawnQueue) {
        generateShip(sym);
    }

    void action() override {
        actionMoving();
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
            cout << "BattleShip: " << sym << " could not move." << endl;
            return;
        }

        gameMap[i][j] = "";
        placeShip(gameMap, sym, newI, newJ);
        setLocation(newI, newJ);
        cout << "Destroyer: " << sym << " moved to (" << newI + 1 << ", " << newJ + 1 << ")" << endl;
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
        else{cout<<"Destroyer "<<sym<<" rammed at ("<<newI+1<<", "<<newJ+1<<") but misses."<<endl;}
    }
};



void printMap(const vector<vector<string>> &gameMap) {
    for (const auto &row : gameMap) {
        for (const auto &cell : row) {
            cout << setw(5) << (cell.empty() ? "0" : cell);
        }
        cout << endl;
    }
}

void generateMap(vector<vector<string>> &gameMap) {
    gameMap.resize(5, vector<string>(5, ""));
}

void printQueue(queue<ship*> &respawnQueue) {
    queue<ship*> temp = respawnQueue;
    while (!temp.empty()) {
        cout << temp.front()->getSym() << " ";
        temp.pop();
    }
    cout << endl;
}

ship* upgradeShip(ship* oldShip, vector<vector<string>> &gameMap, queue<ship*> &respawnQueue, list<ship*> &teamShips) {
    if (oldShip == nullptr) {
        cerr << "Error: oldShip is null" << endl;
        return nullptr;
    }

    try {
        // Create a new Destroyer
        ship* newShip = new Destroyer(oldShip->getSym(), gameMap, teamShips, respawnQueue);

        // Preserve the ship's attributes
        newShip->setLife(oldShip->getLife());
        newShip->setKill(oldShip->getKill());
        newShip->markAsUpgraded();

        cout << "Ship " << oldShip->getSym() << " upgraded to Destroyer!" << endl;
     
        delete oldShip; // Free memory of the old ship
        return newShip;
    } catch (const exception& e) {
        cerr << "Upgrade failed: " << e.what() << endl;
        return nullptr;
    }
}


int main() {
    srand(time(0)); 
    vector<vector<string>> gameMap;
    vector<string> shipA{"*1", "*2", "*3"};
    vector<string> shipB{"<1", "<2", "<3"};
    queue<ship*> respawnQueueA;
    queue<ship*> respawnQueueB;

    generateMap(gameMap);
    printMap(gameMap);

    list<ship*> AShips;
    list<ship*> BShips;

    for (const auto &sym : shipA) {
        AShips.push_back(new Battleship(sym, gameMap, BShips, respawnQueueA));
    }
    for (const auto &sym : shipB) {
        BShips.push_back(new Battleship(sym, gameMap, AShips, respawnQueueB));
    }

   for (int turn = 0; turn < 100; turn++) {
    cout << "=== Turn " << turn + 1 << " ===" << endl;

    // Respawning for Team A
    if (!respawnQueueA.empty()) {
        ship* respawned = respawnQueueA.front();
        respawnQueueA.pop();

        if (respawned->getLife() > 0) { // Only respawn if still alive
            respawned->resetShip(); 
            BShips.push_back(respawned);
            cout << respawned->getSym() << " has respawned!" << endl;
        } else {
            delete respawned; // Permanently remove ship
            cout << respawned->getSym() << " is permanently destroyed and removed." << endl;
        }
    }

    // Respawning for Team B
    if (!respawnQueueB.empty()) {
        ship* respawned = respawnQueueB.front();
        respawnQueueB.pop();

        if (respawned->getLife() > 0) { // Only respawn if still alive
            respawned->resetShip(); 
            AShips.push_back(respawned);
            cout << respawned->getSym() << " has respawned!" << endl;
        } else {
            delete respawned; // Permanently remove ship
            cout << respawned->getSym() << " is permanently destroyed and removed." << endl;
        }
    }
    
    // Perform ship actions
    for (auto it = AShips.begin(); it != AShips.end(); ) {
        if ((*it)->getKill() == 3 && !(*it)->isUpgraded()) {
            int i=(*it)->getLocation().first;
            int j=(*it)->getLocation().second;
             gameMap[i][j]="";
            (*it)->setLocation(-1, -1); // Mark as removed from the map
            ship* upgradedShip = upgradeShip(*it, gameMap, respawnQueueA, AShips);
            it = AShips.erase(it); // Remove the old ship from the list
            it = AShips.insert(it, upgradedShip);
            continue;
        }
        (*it)->action();
        ++it;
    }

    for (auto it = BShips.begin(); it != BShips.end(); ) {
        if ((*it)->getKill() == 3 && !(*it)->isUpgraded()) {
            int i=(*it)->getLocation().first;
            int j=(*it)->getLocation().second;
             gameMap[i][j]="";
            (*it)->setLocation(-1, -1); // Mark as removed from the map
           ship* upgradedShip = upgradeShip(*it, gameMap, respawnQueueA, AShips);
            it = AShips.erase(it); // Remove the old ship from the list
            it = AShips.insert(it, upgradedShip);
            continue;
        }
        (*it)->action();
        ++it;
    }
    printMap(gameMap);
    cout << endl;

    cout << "Respawn Queue A: "<<endl;
    printQueue(respawnQueueB);
    cout << "Respawn Queue B: "<<endl;
    printQueue(respawnQueueA);

    // Check if game should end
    if (AShips.empty() && respawnQueueB.empty()) {
        cout << "Team B wins!" << endl;
        break;
    }
    if (BShips.empty() && respawnQueueA.empty()) {
        cout << "Team A wins!" << endl;
        break;
    }
   }
    printMap(gameMap);
}

//respawn 错是之前没有放进对的list里
