// file: 4heap-test.cpp
#include<vector>
#include<iostream>
#include<algorithm> // heap algorithms
using namespace std;

int main(){
    {
        // test heap （底层以vector完成）
        int ia[9]={0,1,2,3,4,8,9,3,5};
        vector<int> ivec(ia,ia+9);

        make_heap(ivec.begin(),ivec.end());
        for(int i=0;i<ivec.size();++i)
            cout<<ivec[i]<<' '; // 9 5 8 3 4 0 2 3 1
        cout<<endl;

        ivec.push_back(7);
        push_heap(ivec.begin(),ivec.end());
        for(int i=0;i<ivec.size();++i)
            cout<<ivec[i]<<' '; // 9 7 8 3 5 0 2 3 1 4
        cout<<endl;

        pop_heap(ivec.begin(),ivec.end());
        cout<<ivec.back()<<endl; // 9. return but no remove.
        ivec.pop_back(); // remove last elem and no return

        for(int i=0;i<ivec.size();++i)
            cout<<ivec[i]<<' '; // 8 7 4 3 5 0 2 3 1
        cout<<endl;

        sort_heap(ivec.begin(),ivec.end());
        for(int i=0;i<ivec.size();++i)
            cout<<ivec[i]<<' '; // 0 1 2 3 3 4 5 7 8
        cout<<endl;
    }
    {
        // test heap (底层以array完成)
        int ia[9]={0,1,2,3,4,8,9,3,5};
        make_heap(ia,ia+9);
        // array 无法动态改变大小，因此不可以对满载的array进行push_heap()操作。
        // 因为那得先在array尾端增加一个元素。如果对一个满载的array执行push_heap()
        // ,该函数会将最后一个元素视为新增元素，并将其余元素视为一个完成的heap结构
        // （实际上它们的确是），因此执行后的结果等于原先的heap
        sort_heap(ia,ia+9);
        for(int i=0;i<9;++i)
            cout<<ia[i]<<' '; //0 1 2 3 3 4 5 8 9
        cout<<endl;
        // 经过排序之后的heap，不再是合法的heap
        // 重新再做一个heap
        make_heap(ia,ia+9);
        pop_heap(ia,ia+9);
        cout<<ia[8]<<endl; // 9
    }
    {
        // test heap( 底层以array完成 )
        int ia[6]={4,1,7,6,2,5};
        make_heap(ia,ia+6);
        for(int i=0;i<6;++i)
            cout<<ia[i]<<' '; // 7 6 5 1 2 4
        cout<<endl;
    }
}