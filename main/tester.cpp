#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class GameConfig {
private:
    int iterations;
    int mW; // map width
    int mH; // map height

public:
    vector<int> Anum;  // Number of ships of each type for Team A
    vector<char> Asym; // Symbol for each type of ship for Team A
    vector<int> Bnum;  // Number of ships of each type for Team B
    vector<char> Bsym; // Symbol for each type of ship for Team B
    vector<vector<char>> gameMap;

    // Constructor to initialize the game configuration from a file
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
                        // Read ships for Team A
                        for (int i = 0; i < numShips; i++) {
                            string shipType;
                            char symbol;
                            int count;
                            file >> shipType >> symbol >> count;
                            Anum.push_back(count);
                            Asym.push_back(symbol);
                        }
                    } else if (teamName == "B") {
                        // Read ships for Team B
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
                    // Read game map
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

    int getWidth() {
        return mW;
    }

    int getHeight() {
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

    return 0;
}
