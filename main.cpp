#include <iostream>
#include "vector.h"
#include "ship.h"
using namespace std;


int main(){
    vector<int> row;
    for(int i=1;i<3;i++){
        row.push_back(i);
        cout<<row[i-1]<<endl;
    }
    return 0;
}