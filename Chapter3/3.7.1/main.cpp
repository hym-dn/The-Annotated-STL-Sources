struct __true_type{};
struct __false_type{};

template<class type>
struct __type_traits{
    typedef __true_type this_dummy_member_must_be_first;
    /**
     * 不要移除这个成员。它通知“有能力自动将__type_traits特化”的
     * 编译器说，我们现在所看到的这个__type_traits template是特
     * 殊的。这是为了确保万一编译器也使用一个名为__type_traits而
     * 其实于此处定义并无任何关联的template时，所有事情都仍将顺利
     * 运作。
     */

    /**
     * 以下条件应该被遵守，因为编译器有可能自动为各型别产生专属的
     * __type_traits特化版本：
     *     - 你可以重新排列以下的成员次序
     *     - 你可以移除以下任何成员
     *     - 绝对不可以将以下成员重新命名而却没有改变编译器中的对应名称
     *     - 新加入的成员被视为一般成员，除非你在编译器中加上适当支持
     */
    typedef __false_type has_trivial_default_constructor;
    typedef __false_type has_trivial_copy_constructor;
    typedef __false_type has_trivial_assignment_operator;
    typedef __false_type has_trivial_destructor;
    typedef __false_type is_POD_type;//POD全称Plain Old Data。通俗的讲，
                                     //一个类或结构体通过二进制拷贝后还能
                                     //保持其数据不变，那么它就是一个POD类型。

};