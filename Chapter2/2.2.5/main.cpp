
#if 0
#include<new>
#define __THROW_BAD_ALLOC throw bad_alloc
#elif !defined(__THROW_BAD_ALLOC)
#include<iostream.h>
#define __THROW_BAD_ALLOC cerr<<"out of memory"<<endl;exit(1)
#endif

//malloc-based allocator. 通常比稍后介绍的default alloc 速度慢。

//一般而言是thread-safe，并且对于空间的运用比较高效（efficient）。
//以下是第一级配置器
//注意，无“template型别参数”，至于“非型别参数”inst，则完全没派上用场
template<int inst>
class malloc_alloc_template{
private:
    //一下函数将用来处理内存不足的情况
    //oom: out of memory.
    static void *oom_malloc(size_t);
    static void *oom_realloc(void *,size_t);
    static void (*__malloc_alloc_oom_handler)();//函数指针的定义
public:
    static void *allocate(size_t n){
        //第一级配置器直接使用malloc()
        void *result=malloc(n);
        //以下无法满足需求时，改用oom_malloc()
        if(0==result){
            result=oom_malloc(n);
        }
        //返回分配结果
        return(result);
    }
    static void deallocate(void *p,size_t /*n*/){
        free(p);
    }
    static void *reallocate(void *p,size_t /*old_sz*/,size_t new_sz){
        //第一级配置器直接使用realloc()
        void *result=realloc(p,new_sz);
        //一下无法满足时，改用oom_realloc()
        if(0==result){
            result=omm_realloc(p,new_sz);
        }
        //返回重新分配结果
        return(result);
    }
    //以下仿真C++的set_new_handler()。换句话说，你可以通过它
    //指定你自己的out-of-memory handler
    static void (*set_malloc_handler(void(*f)()))(){
       void (*old)()=__malloc_alloc_oom_handler;
       __malloc_alloc_oom_handler=f;
       return(old);
    }
};

//malloc_alloc out-of-memory handing
//初始值为0。有待客户端设定
template<int inst>
void(*__malloc_alloc_template<inst>::__malloc_alloc_oom_handler)()=0;//函数指针的初始化

template<int inst>
void *__malloc_alloc_template<inst>::oom_malloc(size_t n){
    void(*my_malloc_handler)();
    void *result;
    for(;;){//不断尝试释放、配置、再释放、再配置...
        my_malloc_handler=__malloc_alloc_oom_handler;
        if(0==my_malloc_handler){
            __THROW_BAD_ALLOC;//输出错误，退出进程
        }
        //调用例程，企图释放内存
        (*my_malloc_handler)();
        //再次尝试配置内存
        result=malloc(n);
        //返回
        if(result){
            return(result);
        }
    }
}

template<int inst>
void *__malloc_alloc_template<inst>::oom_realloc(void *p,size_t n){
    void(*my_malloc_handler)();
    void *result;
    for(;;){//不断尝试释放、配置、再释放、再配置...
        my_malloc_handler=__malloc_alloc_oom_handler;
        if(0==my_malloc_handler){
            __THROW_BAD_ALLOC;//输出错误，退出进程
        }
        //调用例程，企图释放内存
        (*my_malloc_handler)();
        //再次尝试配置内存
        result=realloc(p,n);
        //返回
        if(result){
            return(result);
        }
    }
}

//注意，一下直接将参数inst指定为0
typedef __malloc_alloc_template<0> malloc_alloc;
