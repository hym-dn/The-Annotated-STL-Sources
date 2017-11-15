/**
 * (5) 如果编译器支持模板类偏特化，就定义
 *     __STL_CLASS_PARTIAL_SPECIALIZATION。
 *
 * 本程序用来测试编译器对类模板偏特化的支持。
 */

#include<iostream>
using namespace std;

//泛化
template<class I,class O>
struct testClass{
    testClass(){
        cout<<"I,O"<<endl;
    }
};

//偏特化
template<class T>
struct testClass<T*,T*>{
    testClass(){
        cout<<"T*,T*"<<endl;
    }
};

//偏特化
template<class T>
struct testClass<const T*,T*>{
    testClass(){
        cout<<"const T*,T*"<<endl;
    }
};

int main(){
    testClass<int,char> obj1;
    testClass<int*,int*> obj2;
    testClass<const int *,int*> obj3;
}
