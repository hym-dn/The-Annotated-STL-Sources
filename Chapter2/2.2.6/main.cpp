//小型区块上调边界
enum{__ALIGN=8};
//小型区块的上限128Bytes
enum{__MAX_BYTES=128};
//free-lists 个数
enum{__NFREELISTS=__MAX_BYTES/__ALIGN};

//以下是第二级配置器
//注意，无“template型别参数”，且第二参数完全没派上用场
//第一参数用于多线程环境下。本书不讨论多线程环境
template<bool threads,int inst>
class __default_alloc_template{
private:
    //ROUND_UP() 将bytes上调至8的倍数
    //[x，x + 8 - 1]范围内会包括将x向上取整的结果，比如x=7，则[7,14]中包含取整结果8。
    //~是按位取反操作，~(8-1)将8的二进制表示从00001000变为11111000，即把1前面的位都变为0。
    //x+8-1 和 ~(8-1)做按位与&操作，则得到结果。
    //8可以被2整除3次，二进制上也就是向右移动3次，如果期望被8整除，实际上也就是期望被2整除3次
    //也就是2进制最后三位必须为0,方可被8整除。
    static size_t ROUND_UP(size_t bytes){
        return(((bytes)+__ALIGN-1)&~(__ALIGN-1));
    }
private:
    //free-list 的节点构造
    union obj{
        union obj *free_list_link;
        char client_data[1];
    };
private:
    //16个free-lists
    static obj *volatile free_list[__NFREELISTS];
    //以下函数根据区块大小，决定使用第n号free-list。n从0起算。
    static size_t FREELIST_INDEX(size_t bytes){
        return(((bytes)+__ALIGN-1)/__ALIGN-1);
    }
    //返回一个大小为n的对象，并可能加入大小为n的其他区块到free list中
    static void *refill(size_t n);
    //配置一大块空间，可容纳nobjs个大小为"size"的区块
    //如果配置nobjs个区块有所不便，nobjs可能会降低
    static char *chunk_alloc(size_t size,int &nobjs);
    //chunk allocation state
    //内存池起始位置。只在chunk_alloc()中变化
    static char *start_free;
    //内存池结束位置。只在chunk_alloc()中变化
    static char *end_free;
    //
    static size_t heap_size;
public:
    //详述于后
    static void *allocate(size_t n){}
    static void deallocate(void *p,size_t n){}
    static void *reallocate(void *p,size_t old_sz,size_t new_sz);
};

//以下是static data member的定义与初始值设定
template<bool threads,int inst>
char *__default_alloc_template<threads,inst>::state_free=0;
template<bool threads,int inst>
char *__default_alloc_template<threads,ints>::end_free=0;
template<bool threads,int inst>
size_t __default_alloc_template<threads,inst>::heap_size=0;
template<bool threads,int inst>
size_t __default_alloc_template<threads,inst>::heap_size=0;
template<bool threads,int inst>
__default_alloc_template<threads,inst>::obj *volatile 
__default_alloc_template<threads,inst>::free_list[__NFREELISTS]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
