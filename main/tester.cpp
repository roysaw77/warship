#include <iostream>
#include <fstream>
#include <string>
#include<vector>

using namespace std;


class GameConfig {
private:
    int iterations;
    int mW; // map width
    int mH; // map height
  

public:
     vector<vector<char>> gameMap;
    // Constructor to initialize the game configuration from a file
    GameConfig(const string& filename) : iterations(0), mW(0), mH(0) {
        ifstream file(filename);

        if (file.is_open()) {
            string key;
            char space=' ';
            while (file >> key) {
                if (key == "iterations") {
                    file >> iterations;
                } 
                else if (key == "width") {
                    file >> mW;
                } 
                else if (key == "height") {
                    file >> mH;
                } 
                else if (key == "0"||key == "1") {
                    gameMap.resize(mH, vector<char>(mW));
                    file.unget();
                    for (int i = 0; i < mH; i++) {
                        for (int j = 0; j < mW; j++) {
                            file >> gameMap[i][j];
                            gameMap[i][j+1]=space;
                        }
                    }
                }
            }
            file.close();
        } else {
            cout << "Error opening file" << endl;
        }
    }

    char getWidth() {
        return mW;
    }

    char getHeight() {
        return mH;
    }

   
};

// Main function
int main() {
    // Create a GameConfig object from the input file
    GameConfig config("game.txt");
    

    return 0;
}
