#include <iostream>
#include "vector.h"
#include "ship.h"
#include "gameconfig.h"
using namespace std;

void printMap(int r, int c) {
    // 定义一个 10x10 的 2D 数组
    int map[r][c] = {
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

    // 打印地图
    for (int i = 0; i < r && i < 10; i++) {
        for (int j = 0; j < c && j < 10; j++) {
            cout << map[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    GameConfig config("game.txt");
    config.printConfig();
    return 0;
}
