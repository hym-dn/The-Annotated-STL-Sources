#include<iostream>
#include<cstddef>
using namespace std;

class alloc{
};

template<class T,class Alloc=alloc,size_t Bufsiz=0>
class deque{
public:
    deque(){
        cout<<"deque"<<endl;
    }
};

template<class T,class Sequence=deque<T> >
class stack{
public:
    stack(){
        cout<<"stack"<<endl;
    }
private:
    Sequence c;
};

int main(){
    stack<int> x;
}
