//在分file之前就在先test你们的class行不行

#include <iostream>
using namespace std;

void map(int row,int column){
    for(int i=0;i<row; i++){
        for(int j=0;j<column; j++){
          cout<<0;
        }
        cout<<endl;
    }
}

int main(){
    int r,c; //initial row column
    cin>>r>>c;
    map(r,c);
    return 0;
}