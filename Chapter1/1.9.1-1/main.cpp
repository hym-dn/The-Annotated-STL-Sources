/**
 * (3) 如果编译器无法处理模板类的静态成员，就定义
 *     __STL_STATIC_TEMPLATE_MEMBER_BUG。
 * 
 * 本程序用于测试编译器是否对模板类静态成员提供支持。
 */

#include<iostream>
using namespace std;

template<typename T>
class testClass{
public:
    static int _data;
};

//静态成员初始化
//泛化版本
template<typename T>
int testClass<T>::_data=0;
//特化版本
template<>
int testClass<int>::_data=1;
template<>
int testClass<char>::_data=2;

int main(){

    //输出静态成员
    cout<<testClass<int>::_data<<endl;
    cout<<testClass<char>::_data<<endl;
    cout<<testClass<unsigned int>::_data<<endl;

    //定义类实例
    testClass<int> obji1,obji2;
    testClass<char> objc1,objc2;

    //输出静态成员
    cout<<obji1._data<<endl;
    cout<<obji2._data<<endl;
    cout<<objc1._data<<endl;
    cout<<objc2._data<<endl;

    //静态成员赋值
    obji1._data=3;
    objc2._data=4;

    //输出静态成员
    cout<<obji1._data<<endl;
    cout<<obji2._data<<endl;
    cout<<objc1._data<<endl;
    cout<<objc2._data<<endl;

    //返回
    return(0);

}
