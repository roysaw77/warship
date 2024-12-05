#include <iostream>
#include "vector.h"
#include "ship.h"
using namespace std;

void printMap(int r, int c) {
    // Define the map as a 2D vector
    vector<vector<int>> map = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 1, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}
    };

    // Print the map
    for (int i = 0; i < r && i < map.length(); i++) {
        for (int j = 0; j < c && j < map[i].length(); j++) {
            cout << map[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    printMap(10, 10);
    return 0;
}