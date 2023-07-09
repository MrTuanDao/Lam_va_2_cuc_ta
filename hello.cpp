#include <iostream>

using namespace std;

int main(){
    bool flag=true;
    while(flag){
        cout<<"Your name:";
        string name;
        fflush(stdin);
        cin>>name;
        if(name=="exit") flag = false; 
        cout<<"Hello "<<name<<endl;
    }
}