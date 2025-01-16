#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <iomanip>
#include <cstdlib>
#include <ctime>

using namespace std;

// Base class for all ships
class Ship {
protected:
    string symbol;
    int x, y;
    int lives;
    int kills;
    int id;
    bool isDestroyed;

public:
    Ship(string symbol, int x, int y, int id) : symbol(symbol), x(x), y(y), lives(3), kills(0), id(id), isDestroyed(false) {}

    virtual ~Ship() {}

    virtual void look(vector<vector<string>>& map) = 0;
    virtual void move(vector<vector<string>>& map) = 0;
    virtual void shoot(vector<vector<string>>& map, queue<pair<int, int>>& reentryQueue) = 0;

    void takeDamage() {
        lives--;
        if (lives <= 0) {
            isDestroyed = true;
            cout << symbol << " destroyed!" << endl;
        }
    }

    bool isAlive() const { return !isDestroyed; }

    string getSymbol() const { return symbol; }

    void setPosition(int newX, int newY) {
        x = newX;
        y = newY;
    }

    pair<int, int> getPosition() const { return {x, y}; }

    void incrementKills() { kills++; }

    int getKills() const { return kills; }

    virtual void upgrade() {}
};

// Derived class for Battleship
class Battleship : public Ship {
public:
    Battleship(string symbol, int x, int y, int id) : Ship(symbol, x, y, id) {}

    void look(vector<vector<string>>& map) override {
        cout << symbol << " looks around." << endl;
        int startX = max(0, x - 1), endX = min((int)map.size() - 1, x + 1);
        int startY = max(0, y - 1), endY = min((int)map[0].size() - 1, y + 1);

        for (int i = startX; i <= endX; ++i) {
            for (int j = startY; j <= endY; ++j) {
                if (map[i][j] != "" && map[i][j] != symbol) {
                    cout << "Enemy detected at (" << i + 1 << ", " << j + 1 << ")!" << endl;
                }
            }
        }
    }

    void move(vector<vector<string>>& map) override {
        int direction = rand() % 4;
        int newX = x, newY = y;
        if (direction == 0 && x > 0) newX--;        // Up
        else if (direction == 1 && x < map.size() - 1) newX++; // Down
        else if (direction == 2 && y > 0) newY--;        // Left
        else if (direction == 3 && y < map[0].size() - 1) newY++; // Right

        if (map[newX][newY] == "") {
            map[x][y] = ""; // Clear old position
            x = newX;
            y = newY;
            map[x][y] = symbol; // Update to new position
            cout << symbol << " moved to (" << x + 1 << ", " << y + 1 << ")" << endl;
        } else {
            cout << symbol << " could not move! Position occupied." << endl;
        }
    }

    void shoot(vector<vector<string>>& map, queue<pair<int, int>>& reentryQueue) override {
        int targetX = x + rand() % 3 - 1; // Random target around the ship
        int targetY = y + rand() % 3 - 1;

        if (targetX >= 0 && targetX < map.size() && targetY >= 0 && targetY < map[0].size() && map[targetX][targetY] != "" && map[targetX][targetY] != symbol) {
            cout << symbol << " fired at (" << targetX + 1 << ", " << targetY + 1 << ") and hit " << map[targetX][targetY] << "!" << endl;
            map[targetX][targetY] = ""; // Destroy the target
            incrementKills();
            reentryQueue.push({targetX, targetY});
        } else {
            cout << symbol << " missed at (" << targetX + 1 << ", " << targetY + 1 << ")!" << endl;
        }
    }

    void upgrade() override {
        if (kills >= 3) {
            cout << symbol << " upgraded to Destroyer!" << endl;
            symbol = "D" + to_string(id);
        }
    }
};

// Derived class for Cruiser
class Cruiser : public Ship {
public:
    Cruiser(string symbol, int x, int y, int id) : Ship(symbol, x, y, id) {}

    void look(vector<vector<string>>& map) override {
        cout << symbol << " scans its surroundings." << endl;
    }

    void move(vector<vector<string>>& map) override {
        for (int i = max(0, x - 1); i <= min((int)map.size() - 1, x + 1); ++i) {
            for (int j = max(0, y - 1); j <= min((int)map[0].size() - 1, y + 1); ++j) {
                if (map[i][j] != "" && map[i][j] != symbol) {
                    map[x][y] = "";
                    x = i;
                    y = j;
                    map[x][y] = symbol;
                    cout << symbol << " moved to (" << x + 1 << ", " << y + 1 << ") and destroyed enemy!" << endl;
                    incrementKills();
                    return;
                }
            }
        }
        cout << symbol << " could not find an enemy to move to." << endl;
    }

    void shoot(vector<vector<string>>& map, queue<pair<int, int>>& reentryQueue) override {
        cout << symbol << " does not shoot." << endl;
    }

    void upgrade() override {
        if (kills >= 3) {
            cout << symbol << " upgraded to Destroyer!" << endl;
            symbol = "D" + to_string(id);
        }
    }
};

// Derived class for Frigate
class Frigate : public Ship {
public:
    Frigate(string symbol, int x, int y, int id) : Ship(symbol, x, y, id) {}

    void look(vector<vector<string>>& map) override {
        cout << symbol << " does not look around." << endl;
    }

    void move(vector<vector<string>>& map) override {
        cout << symbol << " does not move." << endl;
    }

    void shoot(vector<vector<string>>& map, queue<pair<int, int>>& reentryQueue) override {
        int direction = kills % 8;
        int dx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
        int dy[] = {0, 1, 1, 1, 0, -1, -1, -1};
        int targetX = x + dx[direction], targetY = y + dy[direction];

        if (targetX >= 0 && targetX < map.size() && targetY >= 0 && targetY < map[0].size() && map[targetX][targetY] != "") {
            cout << symbol << " fired at (" << targetX + 1 << ", " << targetY + 1 << ") and hit " << map[targetX][targetY] << "!" << endl;
            map[targetX][targetY] = "";
            incrementKills();
        } else {
            cout << symbol << " missed." << endl;
        }
    }

    void upgrade() override {
        if (kills >= 3) {
            cout << symbol << " upgraded to Corvette!" << endl;
            symbol = "C" + to_string(id);
        }
    }
};

// Game simulation class
class GameSimulation {
private:
    vector<vector<string>> map;
    vector<Ship*> teamA;
    vector<Ship*> teamB;
    queue<pair<int, int>> reentryQueue;
    int turns;

public:
    GameSimulation(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening file." << endl;
            exit(1);
        }

        string key;
        while (file >> key) {
            if (key == "iterations") {
                file >> turns;
            } else if (key == "width") {
                int width;
                file >> width;
                map.resize(map.size(), vector<string>(width, ""));
            } else if (key == "height") {
                int height;
                file >> height;
                map.resize(height);
            } else if (key == "Team") {
                string teamName;
                int shipTypes;
                file >> teamName >> shipTypes;

                for (int i = 0; i < shipTypes; ++i) {
                    string shipType;
                    char symbol;
                    int count;
                    file >> shipType >> symbol >> count;

                    for (int j = 1; j <= count; ++j) {
                        string shipSymbol = string(1, symbol) + to_string(j);
                        int x = rand() % map.size();
                        int y = rand() % map[0].size();

                        while (map[x][y] != "") {
                            x = rand() % map.size();
                            y = rand() % map[0].size();
                        }

                        Ship* ship;
                        if (shipType == "Battleship")
                            ship = new Battleship(shipSymbol, x, y, j);
                        else if (shipType == "Cruiser")
                            ship = new Cruiser(shipSymbol, x, y, j);
                        else if (shipType == "Frigate")
                            ship = new Frigate(shipSymbol, x, y, j);
                        else
                            continue;

                        if (teamName == "A") {
                            teamA.push_back(ship);
                        } else {
                            teamB.push_back(ship);
                        }

                        map[x][y] = shipSymbol;
                    }
                }
            } else if (key == "0" || key == "1") {
                file.unget();
                for (auto& row : map) {
                    for (auto& cell : row) {
                        char value;
                        file >> value;
                        cell = (value == '1') ? "1" : "";
                    }
                }
            }
        }
        file.close();
    }

    void displayMap() {
        cout << "\nBattlefield:\n";
        for (const auto& row : map) {
            for (const auto& cell : row) {
                cout << setw(5) << (cell.empty() ? "0" : cell);
            }
            cout << endl;
        }
    }

    void simulate() {
        for (int t = 1; t <= turns; ++t) {
            cout << "\nTurn " << t << ":\n";

            for (auto& ship : teamA)
                if (ship->isAlive()) simulateActions(ship);

            for (auto& ship : teamB)
                if (ship->isAlive()) simulateActions(ship);

            displayMap();

            if (teamA.empty() || teamB.empty()) {
                cout << (teamA.empty() ? "Team B wins!" : "Team A wins!") << endl;
                return;
            }
        }
    }

private:
    void simulateActions(Ship* ship) {
        ship->look(map);
        ship->move(map);
        ship->shoot(map, reentryQueue);
        ship->upgrade();
    }
};

int main() {
    GameSimulation game("game.txt");

    // Simulate the game
    game.simulate();

    return 0;
}
