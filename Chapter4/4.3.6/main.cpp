// 插入一个节点，作为头节点
void push_front(const T &x){ insert(begin(),x); }

// 插入一个节点，作为尾节点（上一小节才介绍过）
void push_back(const T &x){ insert(end(),x); }

// 移除迭代器position所指节点
iterator erase(iterator position){
    link_type next_node=link_type(position.node->next);
    link_type prev_node=link_type(position.node->prev);
    prev_node->next=next_node;
    next_node->prev=prev_node;
    destroy_node(position.node);
    return(iterator(next_node));
}

//移除头节点
void pop_front(){ erase(begin()); }
//移除尾节点
void pop_back(){
    iterator temp=end();
    erase(--temp);
}

// 清除所有节点（整个链表）
template<class T,class Alloc>
void list<T,Alloc>::clear(){
    link_type cur=(link_type)node->next; // begin()
    while(cur!=node){ // 遍历每一个节点
        link_type tmp=cur;
        cur=(link_type)cur->next;
        destroy_node(tmp); // 销毁（析构并释放）一个节点
    }
    // 恢复node原始状态
    node->next=node;
    node->prev=node;
}

// 将数值为value之所有元素移除
template<class T,class Alloc>
void list<T,Alloc>::remove(const T &value){
    iterator first=begin();
    iterator last=end();
    while(first!=last){ // 遍历每一个节点
        iterator next=first;
        ++next;
        if(*first==value) erase(first); //找到就移除
        first=next;
    }
}

// 移除数值相同的连续元素。注意，只有“连续而相同的元素”，才会被移除剩一个
template<class T,class Alloc>
void list<T,Alloc>::unique(){
    iterator first=begin();
    iterator last=end();
    if(first=last) return; //空链表，什么都不必做
    iterator next=first;
    while(++next!=last){ // 遍历每一个节点
        if(*first==*next) // 如果在此区段中有相同的元素
            erase(next); //移除之
        else{
            first=next; //调整指针
        }
        next=first; //修正区段范围
    }
}