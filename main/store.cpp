#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <queue>
#include <tuple>
#include <ctime>
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
    list<ship*> &enemiesShips;
    queue<ship*> &respawnQueue; 

public:
    ship(string sym, int i, int j, vector<vector<string>> &mapRef, list<ship*> &enemies, queue<ship*> &Queue)
        : sym(sym), posI(i), posJ(j), gameMap(mapRef), enemiesShips(enemies), respawnQueue(Queue) {}
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

    for (int count = 0; count < 3; count++) { // Allow up to 3 shots
        int ran = rand() % shootDirections.size(); // Get a random direction
        pair<int, int> direction = shootDirections[ran].first;
        string directionName = shootDirections[ran].second;

        int newI = i + direction.first;
        int newJ = j + direction.second;

        // Ensure the target is within bounds
        if (newI >= 0 && newI < gameMap.size() && newJ >= 0 && newJ < gameMap[0].size()) {
            string &target = gameMap[newI][newJ];

            // Check if the target is not an ally and not an island
            if (!target.empty() && target != "1" && !isAlly(target)) {
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
                    cout << "Ship " << getSym() << " shooting in direction: " 
                         << directionName << " to location: (" << newI + 1 << ", " << newJ + 1 << ")" << endl;
                    cout << enemyShip->getSym() << "'s life is now " << enemyShip->getLife() << endl;

                    if (enemyShip->getLife() != 0) {
                        cout << "  " << enemyShip->getSym()  << " is permanently destroyed!" << endl;
                        gameMap[newI][newJ] = "";

                        // Increase kill count for the attacking ship
                        killIncrement();
                        cout << sym << " Kill: " << kill << endl;

                        // Move enemy to respawn queue and remove from list
                        respawnQueue.push(enemyShip);
                        enemiesShips.erase(it); // Corrected erase

                        cout << enemyShip->getSym() << " added to respawn queue." << endl;
                    }
                }
            }
            else{cout << "Battleship " << sym << " shoots at (" << newI + 1 << ", " << newJ + 1 
                 << ") but misses." << endl;}
        }
    }
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

        for (auto &ship : AShips) ship->action();
        for (auto &ship : BShips) ship->action();

        printMap(gameMap);
        cout << endl;

        cout << "Respawn Queue A: "<<endl;
        printQueue(respawnQueueA);
        cout << "Respawn Queue B: "<<endl;
        printQueue(respawnQueueB);
      
        // Check if all ships are destroyed (including respawn queues)
        if (AShips.empty() ) {
            cout << "Team B wins!" << endl;
            break;
        }
        if (BShips.empty() ) {
            cout << "Team A wins!" << endl;
            break;
        }
    }
    // Cleanup
    for (auto s : AShips) delete s;
    for (auto s : BShips) delete s;

   // Check if there are ships in the respawn queue and bring them back
if (!respawnQueueA.empty()) {
    ship* respawned = respawnQueueA.front();
    respawnQueueA.pop();
    respawned->setLife(3); // Reset life
    respawned->resetShip(); // Place it back on the map
    AShips.push_back(respawned);
    cout << respawned->getSym() << " has respawned!" << endl;
}

if (!respawnQueueB.empty()) {
    ship* respawned = respawnQueueB.front();
    respawnQueueB.pop();
    respawned->setLife(3); // Reset life
    respawned->resetShip(); // Place it back on the map
    BShips.push_back(respawned);
    cout << respawned->getSym() << " has respawned!" << endl;
}

    return 0;
}

// class Destroyer : public movingShip, public shootingShip {   
// private:
// int currentDirectionIndex = 0;
// public:
//     Destroyer(string sym, vector<vector<string>> &gameMap, list<ship*> &enemies, queue<ship*> &respawnQueue)
//         : ship(sym, -1, -1, gameMap, enemies, respawnQueue) {
//         generateShip(sym);
//     }

//     void action() override {
//         actionMoving();
//         actionShooting();
//     }

//     void actionMoving() override {
//         pair<int, int> location = getLocation();
//         int i = location.first;
//         int j = location.second;
//         int iter = 0;

//         int newI, newJ;
//         bool moved = false;

//         do {
//             newI = i + (rand() % 3 - 1);
//             newJ = j + (rand() % 3 - 1);

//             if (isWithinBound(newI, newJ) && gameMap[newI][newJ].empty()) {
//                 moved = true;
//                 break;
//             }
//             iter++;
//         } while (iter < 100);

//         if (!moved) {
//             cout << "Destroyer: " << sym << " could not move." << endl;
//             return;
//         }

//         gameMap[i][j] = "";
//         placeShip(gameMap, sym, newI, newJ);
//         setLocation(newI, newJ);
//         cout << "Destroyer: " << sym << " moved to (" << newI + 1 << ", " << newJ + 1 << ")" << endl;
//     }

//      void actionShooting() override {
//     pair<int, int> location = getLocation();
//     int i = location.first;
//     int j = location.second;

//     // Define possible shooting directions
//     const vector<pair<std::pair<int, int>, string>> shootDirections = {
//         {{1, 1}, "South-East"},
//         {{1, 0}, "South"},
//         {{0, 1}, "East"},
//         {{-1, 0}, "North"},
//         {{-1, -1}, "North-West"},
//         {{1, -1}, "South-West"},
//         {{-1, 1}, "North-East"},
//         {{0, -1}, "West"}
//     };

//     // Get the current direction
//     pair<int, int> direction = shootDirections[currentDirectionIndex].first;
//     string directionName = shootDirections[currentDirectionIndex].second;

//     // Calculate the target location based on the current location and the selected direction
//     int newI = i + direction.first;
//     int newJ = j + direction.second;

//     // Check if the new position is within bounds
//     if (newI >= 0 && newI < gameMap.size() && newJ >= 0 && newJ < gameMap[0].size()) {
//         string &targetSymbol = gameMap[newI][newJ];

//         if (!targetSymbol.empty() && targetSymbol != "1" && !isAlly(targetSymbol)) {
//             ship* enemyShip = nullptr;
//             auto it = enemiesShips.begin();

//             while (it != enemiesShips.end()) {
//                 pair<int, int> enemyLoc = (*it)->getLocation();
//                 if (enemyLoc.first == newI && enemyLoc.second == newJ) {
//                     enemyShip = *it;
//                     break;
//                 }
//                 ++it;
//             }

//             if (enemyShip) {
//                 enemyShip->lifeDecrement();
//                 cout << "Ship " << getSym() << " shooting in direction: " 
//                      << directionName << " to location: (" << newI + 1 << ", " << newJ + 1 << ")" << endl;
//                 cout << targetSymbol << "'s life is now " << enemyShip->getLife() << endl;

//                 if (enemyShip->getLife() <= 0) {
//                     cout << "  " << targetSymbol << " is permanently destroyed!" << endl;
//                     gameMap[newI][newJ] = "";

//                     // Increase kill count for the attacking ship
//                     killIncrement();
//                     cout << sym << " Kill: " << kill << endl;

//                     // Move enemy to respawn queue and remove from list
//                     respawnQueue.push(enemyShip);
//                     it = enemiesShips.erase(it); // Fix iterator issue

//                     cout << targetSymbol << " added to respawn queue." << endl;
//                 }
//             }
//         }
//     }

//     // Increment the direction index for the next round
//     currentDirectionIndex = (currentDirectionIndex + 1) % shootDirections.size();
// }

// };