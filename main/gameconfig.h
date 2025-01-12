#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

class GameConfig {
private:
    int iterations;
    int mW;
    int mH;
    vector<int> Anum;
    vector<string> Asym; // Changed to vector<string>
    vector<int> Bnum;
    vector<string> Bsym; // Changed to vector<string>
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

                            // Generate numbered symbols for Team A
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

                            // Generate numbered symbols for Team B
                            for (int j = 1; j <= count; ++j) {
                                Bsym.push_back(string(1, symbol) + to_string(j));
                            }
                        }
                    }
                } else if (key == "0" || key == "1") {
                    gameMap.resize(mH, vector<char>(mW, '0'));
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

        if (gameMap.empty()) {
            gameMap.resize(mH, vector<char>(mW, '0'));
        }
    }

    int getWidth() const { return mW; }
    int getHeight() const { return mH; }
    vector<int> getAnum() { return Anum; }
    vector<string> getAsym() { return Asym; } // Return vector<string>
    vector<int> getBnum() { return Bnum; }
    vector<string> getBsym() { return Bsym; } // Return vector<string>
    vector<vector<char>> getGameMap() { return gameMap; }
};
