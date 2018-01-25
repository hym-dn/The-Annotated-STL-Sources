//将尾端元素拿掉,并调整大小
void pop_back(){
    --finish; //将尾端标记往前移一格，表示将放弃尾端元素
    //调用析构函数
    destroy(finish); //destroy 是全局函数，见第2章
}

//清除[first,last)中的所有元素
iterator erase(iterator first,iterator last){
    //拷贝last到finish的内容到first上
    iterator i=copy(last,finish,first);// copy是全局函数，第6章
    //调用i~finish的析构函数
    destroy(i,finish); // destroy是全局函数，第2章。
    //更新结尾
    finish=finish-(last-first);
    //返回
    return(first);
}

//清除某个位置上的元素
iterator erase(iterator position){
    //如果不是最后一个元素
    if(position+1!=end()){
        //拷贝position+1到finish之间的数据，到position上
        copy(position+1,finish,position);// copy是全局函数，第6章
    }
    //更新结尾
    --finish;
    //调用析构函数
    destroy(finish); // destroy是全局函数,2.2.3节
    //返回
    return(position);
}

//清除全部内容
void clear(){ 
    erase(begin(),end()); // erase()就定义在上面
} 

