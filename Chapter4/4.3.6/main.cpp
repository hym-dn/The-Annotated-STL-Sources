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

// 将[first,last)内的所有元素移动到position之前
void transfer(iterator position,iterator first,iterator last){
    if(position!=last){// 插入位置便是当前位置
        (*(link_type((*last.node).prev))).next=position.node;
        (*(link_type((*first.node).prev))).next=last.node;
        (*(link_type((*position.node).prev))).next=first.node;
        link_type tmp=link_type((*position.node).prev);
        (*position.node).prev=(*last.node).prev;
        (*last.node).prev=(*first.node).prev;
        (*first.node).prev=tmp;
    }
}

// 将x接合于position所指位置之前，x必须不同于*this
void splice(iterator position,list &x){
    if(!x.empty())
        transfer(position,x.begin(),x.end());
}

// 将 i 所指元素接合于position所指位置之前，position和i可指向同一个list
void splice(iterator position,list &,iterator i){
    iterator j=i;
    ++j;
    if(position==i||position==j) return;
    transfer(position,i,j);
}

// 将[first,last)内的所有元素接合于position所指位置之前
// position和[first,last)可指向同一个list,
// 但position不能位于[first,last)之内
void splice(iterator position,list &,iterator first,iterator last){
    if(first!=last)
        transfer(position,first,last);
}

// merge()将x合并到*this身上，两个lists的内容都必须先经过递增排序
template<class T,class Alloc>
void list<T,Alloc>::merge(list<T,Alloc> &x){
    iterator first1=begin();
    iterator last1=end();
    iterator first2=x.begin();
    iterator last2=x.end();
    //注意：前提是，两个lists都已经过递增排序
    while(first1!=last1&&first2!=last2)
        if(*first2<*first1){
            iterator next=first2;
            transfer(first1,first2,++next);
            first2=next;
        }else{
            ++first1;
        }
    if(first2!=last2) transfer(last1,first2,last2);
}

// reverse()将*this的内容逆向重置
template<class T,class Alloc>
void list<T,Alloc>::reverse(){
    // 以下判断，如果是空链表，或仅有一个元素，就不进行任何操作
    // 使用size()==0||size()==1来判断，虽然也可以，但是比较慢
    if(node->next==node||link_type(node->next)->next==node)
        return;
    iterator first=begin();
    while(first!=end()){
        iterator old=first;
        ++first;
        transfer(begin(),old,first);
    }
}

