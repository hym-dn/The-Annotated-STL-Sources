//filename : 4list-test.cpp
#include<list>
#include<iostream>
#include<algorithm>
using namespace std;

int main(){

    int i;
    list<int> ilist;
    cout<<"size="<<ilist.size()<<endl; // size=0

    ilist.push_back(0);
    ilist.push_back(1);
    ilist.push_back(2);
    ilist.push_back(3);
    ilist.push_back(4);
    cout<<"size="<<ilist.size()<<endl; // size=5

    list<int>::iterator ite;
    for(ite=ilist.begin();ite!=ilist.end();++ite){
        cout<<*ite<<' '; // 0 1 2 3 4
    }
    cout<<endl;

    ite=find(ilist.begin(),ilist.end(),3);
    if(ite!=ilist.end()){
        ilist.insert(ite,99);
    }

    cout<<"size="<<ilist.size()<<endl; //size=6
    cout<<*ite<<endl; //3

    for(ite=ilist.begin();ite!=ilist.end();++ite){
        cout<<*ite<<' '; // 0 1 2 99 3 4
    }
    cout<<endl;

    ite=find(ilist.begin(),ilist.end(),1);
    if(ite!=ilist.end()){
        cout<<*(ilist.erase(ite))<<endl; // 2
    }

    for(ite=ilist.begin();ite!=ilist.end();++ite){
        cout<<*ite<<' '; //0 2 99 3 4
    }
    cout<<endl;

}