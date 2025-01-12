#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

class GameConfig {
private:
    int iterations;
    int mW; // map width
    int mH; // map height
    vector<int> Anum;
    vector<char> Asym;
    vector<int> Bnum;
    vector<char> Bsym;
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
    vector<int> getAnum() {
        return Anum;
    }

    vector<char>getAsym() {
        return Asym;
    }

    vector<int> getBnum() {
        return Bnum;
    }

    vector<char> getBsym() {
        return Bsym;
    }

    vector<vector<char>> getGameMap() {
        return gameMap;
    }
};