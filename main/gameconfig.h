#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class GameConfig {
private:
    int iterations;
    int mW;
    int mH;
    char **gameMap;
public:
    GameConfig(const string& filename) {
        string line;
        iterations = 0;
        mW = 0;
        mH = 0;
        char gameMap[mW][mH];
        ifstream file(filename);
        getline(file, line);
        if (file.is_open()) {
            string key;
            while (file >> key) {
                if (key == "iterations") {
                    file >> iterations;
                } else if (key == "width") {
                    file >> mW;
                } else if (key == "height") {
                    file >> mH;
                }
                if(key=="0"||key=="1"){
                    allocateGameMap();
                    for (int i = 0; i < mH; ++i) {
                        for (int j = 0; j < mW; ++j) {
                            file >> gameMap[i][j];
                        }
                    }
                    break;
                }
            }
            file.close();
        } else {
            cout << "Error opening file" << endl;
        }
    }

     ~GameConfig() {
        // Free the dynamically allocated memory
        if (gameMap) {
            for (int i = 0; i < mH; ++i) {
                delete[] gameMap[i];
            }
            delete[] gameMap;
        }
    }
     int getIterations() const {
        return iterations;
    }

      void allocateGameMap() {
        gameMap = new char*[mH];
        for (int i = 0; i < mH; ++i) {
            gameMap[i] = new char[mW];
        }
      }
    void displayGameMap() const {
        for (int i = 0; i < mH; ++i) {
            for (int j = 0; j < mW; ++j) {
                cout << gameMap[i][j];
            }
            cout << endl;
        }
    }
};