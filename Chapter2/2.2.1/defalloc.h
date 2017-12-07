#ifndef DEFALLOC_H
#define DEFALLOC_H

#include<new.h>
#include<stddef.h>
#include<stdlib.h>
#include<limits.h>
#include<iostream.h>
#include<algobase.h>

/**
 * @brief 分配内存
 * @param size - 元素个数
 * @return 返回内存首地址
 */
template<class T>
inline T *allocate(ptrdiff_t size,T*){
    //取消分配异常处理
    set_new_handler(0);
    //分配内存
    T *tmp=(T*)(::operator new((size_t)(size*sizeof(T))));
    //失败
    if(tmp<0){
        //出错
        cerr<<"out of memory"<<endl;
        //退出
        exit(1);
    }
    //返回
    return(tmp);
}

/**
 * @brief 释放内存
 * @param buffer - 内存首地址
 */
template<class T>
inline void deallocate(T *buffer){
    ::operator delete(buffer);
}

template<class T>
class allocator{

public:

    //类型定义
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    /**
     * @brief 内存分配
     * @param n - 元素个数
     */
    pointer allocate(size_type n){
        return(::allocate((difference_type)n,(pointer)0));
    }

    /**
     * @brief 内存销毁
     * @param p - 待销毁内存
     */
    void deallocate(pointer p){
        ::deallocate(p);
    }

    /**
     * @brief 取地址
     * @param x - 待取地址元素
     */
    pointer address(reference x){
        return((pointer)&x);
    }

    /**
     * @brief 取只读地址
     * @param x - 待取地址元素
     */
    const_pointer const_address(const_reference x){
        return((const_pointer)&x);
    }

    /**
     * @brief 初始页数
     */
    size_type init_page_size(){
        return(max(size_type(1),size_type(4096/sizeof(T))));
    }

    /**
     * @brief 最大页数
     */
    size_type max_size() const{
        return(max(size_type(1),size_type(UINT_MAX/sizeof(T))));
    }

};

//特化版本
class allocator<void>{
public:
    typedef void *pointer;
};

#endif
