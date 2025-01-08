#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class GameConfig {
private:
    int iterations;
    int mW;
    int mH;
    char** gameMap; // Dynamically allocated 2D array for the game map

public:
    // Constructor to initialize the game configuration from a file
    GameConfig(const string& filename) : iterations(0), mW(0), mH(0), gameMap(nullptr) {
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
                } else if (key == "0" || key == "1") {
                    // Start reading the map
                    file.unget(); // Push the current character back into the stream
                    allocateGameMap();
                    for (int i = 0; i < mH; ++i) {
                        for (int j = 0; j < mW; ++j) {
                            file >> gameMap[i][j];
                        }
                    }
                    break; // Map is completely read; no need to process further
                }
            }
            file.close();
        } else {
            cout << "Error opening file" << endl;
        }
    }

    // Destructor to free the allocated memory
    ~GameConfig() {
        if (gameMap) {
            for (int i = 0; i < mH; ++i) {
                delete[] gameMap[i];
            }
            delete[] gameMap;
        }
    }

    // Allocate memory for the game map
    void allocateGameMap() {
        gameMap = new char*[mH];
        for (int i = 0; i < mH; ++i) {
            gameMap[i] = new char[mW];
        }
    }

    // Display the game map
    void displayGameMap() const {
        for (int i = 0; i < mH; ++i) {
            for (int j = 0; j < mW; ++j) {
                cout << gameMap[i][j] << ' '; // Add a space for better readability
            }
            cout << endl;
        }
    }

    // Getter for iterations
    int getIterations() const {
        return iterations;
    }
};

// Main function
int main() {
    // Create a GameConfig object from the input file
    GameConfig config("game.txt");

    // Display the iterations
    cout << "Iterations: " << config.getIterations() << endl;

    // Display the game map
    config.displayGameMap();

    return 0;
}
