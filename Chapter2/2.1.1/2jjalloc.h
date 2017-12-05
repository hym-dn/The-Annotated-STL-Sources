// file: 2jjalloc.h
#ifndef __JJALLOC_
#define __JJALLOC_

#include<new> //for placement new
#include<cstddef> // for ptrdiff_t,size_t
#include<cstdlib> // for exit()
#include<climits> // for UINT_MAX
#include<iostream> // for cerr

namespace JJ{

    /**
     * @brief 分配空间
     * @param size - 分配个数
     * @param T - 分配类型
     * @return 返回执行分配区域的指针
     */
    template<class T>
    inline T* _allocate(ptrdiff_t size,T*){
        //清除new失败处理函数
        std::set_new_handler(0);
        //分配内存
        T *tmp=(T*)(::operator new((size_t)(size*sizeof(T))));
        //分配失败
        if(tmp==0){
            //输出错误
            std::cerr<<"out of memory"<<std::endl;
            //终止进程
            exit(1);
        }
        //返回
        return(tmp);
    }

    /**
     * @biref 释放空间
     * @param buffer - 待释放的空间
     */
    template<class T>
    inline void _deallocate(T *buffer){
        ::operator delete(buffer);
    }

    /**
     * @brief 在指定空间上调用带参数的构造函数
     * @param p - 指定空间
     * @param value - 构造函数参数
     */
    template<class T1,class T2>
    inline void _construct(T1 *p,const T2 &value){
        new(p) T1(value);
    }

    /**
     * @brief 调用析构函数
     * @param ptr - 实例指针
     */
    template<class T>
    inline void _destroy(T *ptr){
        ptr->~T();
    }

    template<class T>
    class allocator{
    public:
        typedef T value_type;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T& reference;
        typedef const T& const_reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        //rebind allocator of type U
        template<class U>
        struct rebind{
            typedef allocator<U> other;
        };

        /**
         * @brief 封装全局内存分配函数
         * @param n - 数量
         * @param hint - 无意义
         * @return 返回分配的空间地址
         */
        pointer allocate(size_type n,const void * /*hint*/=0){
            return _allocate((difference_type)n,(pointer)0);
        }

        /**
         * @brief 封装全局内存释放函数
         * @param p - 内存空间
         * @param n - 无意义
         */
        void deallocate(pointer p,size_type /*n*/){
            _deallocate(p);
        }

        /**
         * @breif 拷贝构造函数
         * @param p - 内存空间
         * @param value - 拷贝源
         */
        void construct(pointer p,const T &value){
            _construct(p,value);
        }

        //析构函数
        //p - 内存空间
        void destroy(pointer p){
            _destroy(p);
        }

        //取地址
        pointer address(reference x){
            return((pointer)&x);
        }

        //取地址
        const_pointer const_address(const_reference x){
            return((const_pointer)&x);
        }

        //分配器最大尺寸
        size_type max_size() const{
            return(size_type(UINT_MAX/sizeof(T)));
        }


    }; // class allocator

} // end of namespace JJ


#endif // __JJALLOC_
