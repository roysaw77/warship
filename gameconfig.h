#include <iostream>
#include <fstream>
#include <string>
#include "vector.h"
using namespace std;

class GameConfig {
private:
    int iterations;
    int mapWidth;
    int mapHeight;

public:
    GameConfig(const string& filename) {
        iterations = 0;
        mapWidth = 0;
        mapHeight = 0;

        ifstream file(filename);
        if (file.is_open()) {
            string key;
            while (file >> key) {
                if (key == "iterations") {
                    file >> iterations;
                } else if (key == "width") {
                    file >> mapWidth;
                } else if (key == "height") {
                    file >> mapHeight;
                }
            }
            file.close();
        } else {
            cout << "Error opening file" << endl;
        }
    }

    void printConfig() {
        cout << "Iterations: " << iterations << endl;
        cout << "Map Width: " << mapWidth << endl;
        cout << "Map Height: " << mapHeight << endl;
    }
};