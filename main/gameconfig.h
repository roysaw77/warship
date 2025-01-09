#include <iostream>
#include <fstream>
#include <string>
#include "CVector.h"
#include<vector>

using namespace std;


class GameConfig {
private:
    int iterations;
    int mW;//mapheight
    int mH;//mapwidth
    string gameMap; // Dynamically allocated 2D array for the game map

public:
    // Constructor to initialize the game configuration from a file
    CVector<int>num; // Number of each type of ship
    CVector<char>sym; // Symbol for each type of ship
    GameConfig(const string& filename) : iterations(0), mW(0), mH(0) {
        ifstream file(filename);

        if (file.is_open()) {
            string key;

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
                else if(key == "Battleship" || key == "Cruiser" || key == "Destroyer" || key == "Frigate" || key == "Corvette"||key=="Amphibious"){
                     int count;
                     char symbol;
                     file >> symbol>>count;
                     num.push_back(count);
                     sym.push_back(symbol);
                }
                else if (key == "0" || key == "1") {
                   file.unget(); // Push the current character back into the stream
                    char c;
                    for (int i = 0; i < mH; ++i) {
                        for (int j = 0; j < mW; ++j) {
                            file >> c;
                            gameMap += c;
                        }
                        gameMap += '\n'; // Add newline after each row
                    }
                    break;
              }
            }
            file.close();
        } else {
            cout << "Error opening file" << endl;
        }
    }

    // Display the game map
   

    // Getter for iterations
    int getIterations() const {
        return iterations;
    }
    
    string getGameMap()  {
        string s;
        for (int i = 0; i < mH; i++) {
            for (int j = 0; j < mW; j++) {
                s += gameMap[i * (mW + 1) + j];
                s += " ";
            }
            s += "\n";
        }
        return s;
    }
  
};
