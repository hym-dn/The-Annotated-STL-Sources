template<class Item> //Item 可以是单向链表节点或双向链表节点。
struct ListIter{//此处这个迭代特定只为链表服务，因为其独特的operator++之故。
    Item *ptr;//保持与容器之间的一个联系（keep a reference to Container）
    ListIter(Item *p=0)//default ctor
        :ptr(p){}
    //不必实现copy ctor,因为编译器提供的缺省行为以足够
    //不必实现operator=,因为编译器提供的缺省行为为足够
    Item &operator*() const{return *ptr;}
    Item *operator->() const{return ptr;}
    //以下两个operator++遵循标准做法
    //(1) pre-increment operator
    ListIter &operator++(){
        ptr=ptr->next();return(*this);
    }
    //(2)post-increment operator
    ListIter operator++(int){
        ListIter temp=*this;++*this;return tmp;
    }
    bool operator==(cosnt ListIter &i) const{
        return(ptr==i.ptr);
    }
    bool operator!=(const ListIter &i) const{
        return(ptr!=i.ptr);
    }
};

void main(){
    List<int> mylist;
    for(int i=0;i<5;++i){
        mylist.insert_front(i);
        mylist.insert_end(i+2);
    }
    mylist.display();//10(4 3 2 1 0 2 3 4 5 6)
    ListIter<ListItem<int>> begin(mylist.front());
    ListIter<ListItem<int>> end;//default 0,null
    ListIter<ListItem<int>> iter;//default 0,null
    iter=find(begin,end,3);
    if(iter==end){
        cout<<"not found"<<endl;
    }else{
        cout<<"found. "<<iter->value()<<endl;
    }
    //执行结果：found. 3
    iter=find(begin,end,7);
    if(iter==end){
        cout<<"not found"<<endl;
    }else{
        cout<<"found. "<<iter->value()<<endl;
    }
    //执行结果: not found
}