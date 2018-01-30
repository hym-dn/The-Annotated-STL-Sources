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


// list不能使用STL算法sort()，必须使用自己的sort()member function，
// 因为STL算法sort()只接受RamdonAccessIterator
// 本函数采用quick sort
/*
总体上来说，算法的思想就是准备64个槽(counter[i])，编号为0,1,...,63,
分别表示已经排序长为1,2,...,2的链表．一开始这64个槽为空.
1.如果待排序链表还有未被取出的元素则取出下一个元素，否则转到3
2.将这个元素先和0号槽合并，如果此时0号槽为空则，将合并后结果放入0号槽;
  如果不为空那么合并后长度变为2,则清空0号槽，继续和1号槽合并，依次类推
  一直合并到i号槽，且i号槽为空，则将合并结果放入i号槽．转到1.
3.将这64个槽从低到高依次合并，得到最终的结果.
*/
/*
 举例说明
 下面以链表数据4，2，1，5，6，9，7，8，10为例分析这个迭代版的代码过程
 在while循环里面每次都是从原链表里取出一个节点，然后往counter数组里面归并，
 fill值表明目前counter数组中存有数据的最大的那个数组标号+1,比如说，我们首先
 取出４，此时fill = 0, 直接就把４放在counter[0]链表上，fill变为1，然后取
 出1，就拿2与counter[0]进行merge，得到结果放到count[1],fill变为2，此时
 counter数组的情况是counter[0]为空，counter[1]存放2，4，再加入1时候直接
 放到counter[0], 再加入5时候，1与5merge，得到1，5再继续向上和couter[1]中
 的2,4,merge得到1，2，4，5，如果此时counter[2]不为空，那么继续merge，这里
 为空则1，2，4，5存到counter[2],于是就这样一步步的向上merge，每次merge链表
 长度都是加倍最终取完所有数据之后，counter数组里面的数据需要最终整合一下，代码
 中最后一个for循环就是不断的把couter中的内容向上merge，最终形成最后的结果这里
 其实也就是形成了一颗归并树，时间复杂度依然是O(nlgn)
 */
template<class T,class Alloc>
void list<T,Alloc>::sort(){
    // 以下判断，如果是空链表，或仅有一个元素，就不进行任何操作
    // 使用size()==0||size()==1来判断，虽然也可以，但是比较慢
    if(node->next==node||link_type(node->next)->next==node){
        return;
    }
    //一些新的lists，作为中介数据存放区
    list<T,Alloc> carry;
    list<T,Alloc> counter[64]; //64个槽
    int fill=0;
    while(!empty()){//当前链表非空
        //将当前列表中begin()位置上的元素插入到carry链表begin()位置之前。
        carry.splice(carry.begin(),*this,begin());
        int i=0;
        while(i<fill&&!counter[i].empty()){
            //合并链表
            counter[i].merge(carry);//中间有排序的过程，排序合并
            //交换链表
            carry.swap(counter[i++]);
        }
        //carry链表内容和counter[i]交换
        carry.swap(counter[i++]);
        if(i==fill) ++fill;
    }
    //将槽进行合并
    for(int i=1;i<fill;++i)
        counter[i].merge(counter[i-1]);
    //获取排序后的列表
    swap(counter[fill-1]);
}
