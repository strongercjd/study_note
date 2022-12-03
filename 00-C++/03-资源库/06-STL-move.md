# 1、简介

在**C++11**中，标准库在<utility>中提供了一个有用的函数std::move，std::move并不能移动任何东西，它唯一的功能是将一个左值强制转化为右值引用，继而可以通过右值引用使用该值，以用于移动语义。从实现上讲，std::move基本等同于一个类型转换：static_cast<T&&>(lvalue);

- C++ 标准库使用比如vector::push_back 等这类函数时,会对参数的对象进行复制,连数据也会复制.这就会造成对象内存的额外创建, 本来原意是想把参数push_back进去就行了,通过std::move，可以避免不必要的拷贝操作。
- std::move是将对象的状态或者所有权从一个对象转移到另一个对象，只是转移，没有内存的搬迁或者内存拷贝所以可以提高利用效率,改善性能.。
- 对指针类型的标准库对象并不需要这么做.

# 2、为什么需要move

**值传递方式**

``` cpp
//值传递
void SetName(std::string name) {

}
std::string name = "123";
SetName(name);
```

上面的函数参数是值传递，一般对于标准数据类型，会采用值传递，对于复杂类型，结构体，类对象等并不会用值传递，因为会多一份对象的拷贝

**引用传递**

``` cpp
//引用传递
void SetName(const std::string& name) {

}
std::string name = "123";
SetName(name);
```

引用传递可以看成是指针的传递，并不会有临时对象的构造和析构，所以性能上会好一些。



**无论是哪种传递方式，调用者外部的变量 name 有时候感觉会有一次 "多余的构造和析构"**

``` cpp
class Apples
{
public:
    void Add(const std::string &name)
    {
        // push_back 会拷贝构造一份，然后放入vector中
        names_.push_back(name);
    }

private:
    std::vector<std::string> names_;
};

Apples apple;
std::string one = "one";
//使用 one 变量做一些操作
.......
//引用传递
apple.Add(one);
```

# 3、move简单示例

简单示例

``` cpp
#include <iostream>
#include <utility>
#include <vector>
#include <string>
int main()
{
    std::string str = "Hello";
    std::vector<std::string> v;
    //调用常规的拷贝构造函数，新建字符数组，拷贝数据
    v.push_back(str);
    std::cout << "After copy, str is \"" << str << "\"\n";
    //调用移动构造函数，掏空str，掏空后，最好不要使用str
    v.push_back(std::move(str));
    std::cout << "After move, str is \"" << str << "\"\n";
    std::cout << "The contents of the vector are \"" << v[0]
                                         << "\", \"" << v[1] << "\"\n";
}
```

运行结果

``` cpp
After copy, str is "Hello"
After move, str is ""
The contents of the vector are "Hello", "Hello"
```

std::move是一个用于提示优化的函数，过去的c++98中，由于无法将作为右值的临时变量从左值当中区别出来，所以程序运行时有大量临时变量白白的创建后又立刻销毁，其中又尤其是返回字符串std::string的函数存在最大的浪费。

在C++11中，标准库在中提供了一个有用的函数std::move，std::move并不能移动任何东西，它唯一的功能是将一个左值引用强制转化为右值引用，继而可以通过右值引用使用该值，以用于移动语义。从实现上讲，std::move基本等同于一个类型转换：static_cast<T&&>(lvalue);

# 4、左值右值的问题

**1、左值和右值的概念**

左值是可以放在赋值号左边可以被赋值的值；左值必须要在内存中有实体；

右值当在赋值号右边取出值赋给其他变量的值；右值可以在内存也可以在CPU寄存器。

一个对象被用作右值时，使用的是它的内容(值)，被当作左值时，使用的是它的地址**。**

**2、引用**

引用是C++语法做的优化，引用的本质还是靠指针来实现的。引用相当于变量的别名。

引用可以改变指针的指向，还可以改变指针所指向的值。

引用的基本规则：

1. 声明引用的时候必须初始化，且一旦绑定，不可把引用绑定到其他对象；即引用必须初始化，不能对引用重定义**；**
2. 对引用的一切操作，就相当于对原对象的操作。

**3、左值引用和右值引用**

  3.1 左值引用
     左值引用的基本语法：type &引用名 = 左值表达式；

  3.2 右值引用

​    右值引用的基本语法type &&引用名 = 右值表达式；

​    右值引用在企业开发人员在代码优化方面会经常用到。

​    右值引用的“&&”中间不可以有空格。

# 5、详解

std::move 的函数原型定义：

``` cpp
template <typename T>
typename remove_reference<T>::type&& move(T&& t)
{
	return static_cast<typename remove_reference<T>::type &&>(t);
}
```

首先，函数参数T&&是一个指向模板类型参数的右值引用，通过引用折叠，此参数可以与任何类型的实参匹配（可以传递左值或右值，这是std::move主要使用的两种场景)。



调用的地方使用 std::move 是错误的，std::move没有做任何实质性的操作，仅仅是个强制类型转换而已

``` cpp
class Apples
{
public:
    void Add(const std::string &name)
    {
        // push_back 会拷贝构造一份，然后放入vector中
        names_.push_back(name);
    }

private:
    std::vector<std::string> names_;
};

Apples apple;
std::string one = "one";
//使用 one 变量做一些操作
//  .......
apple.Add(std::move(one));
```

上述写法是错误的



改造之后如下

``` cpp
class Apples
{
public:
    void Add(const std::string &name)
    {
        names_.push_back(name);
    }
    //新增一个右值参数的函数。
    void Add(std::string &&name)
    {
        names_.push_back(std::move(name));
    }

private:
    std::vector<std::string> names_;
};

Apples apple;
std::string one = "one";
// std::move之后会强制转换成右值对象，这样就可以匹配到Apples 类中的带有右值得函数。
apple.Add(std::move(one));
```

**所以要支持std::move 我们的类也需要提供一些带有右值得函数**

``` cpp
//转移构造
Apples(Apples &&other)
{
    // do something
}
//转移赋值
Apples &operator=(Apples &&other)
{
    // do somthing
}
```

Apples 对象实现了转移构造和转移赋值，所以就可以使用std::move　动态转移， 一定要记住 **std::move仅仅是强制类型转换，真正的转移是在转移构造以及转移赋值中完成的**



那是不是所有的自定义结构体或者类都需要来实现那两个转移函数呢？　

当然并不是，我自己总结了一下，如果你的类或者结构体中所有的成员都已经实现了std::move 语义，并且没有自定义析构函数(还没有搞清楚为什么有自定义的析构就不行)， 那可以不用显式提供，编译器会自动有一个。否则只要成员中有一个没有实现，都需要自己去实现。

不需要自己实现的代码示例:

``` cpp
class Apples
{
public:
    Apples()
    {
        company = "test";
        names_.push_back("abc");
    }

private:
    //下面两个成员变量 是stl中的，已经实现了std::move， 所以Apples 类不需要自己实现
    std::string company;
    std::vector<std::string> names_;
};

Apples apple1;
// 将apple1 直接转移到了 apple2. 其实是调用了 转移构造，但是Apples并没有自己实现转移构造函数。 因为编译器有一个默认的。就像默认的拷贝构造函数一样。
Apples apple2 = std::move(apple1);
```

需要自己实现的代码示例:

``` cpp
class Apples
{
public:
    Apples()
    {
        company_ = "conpany";
        config_ = (char *)malloc(10);
        strcpy_s(config_, 9, "test");
    }

    ~Apples()
    {
        if (nullptr != config_)
        {
            free(config_);
            config_ = nullptr;
        }
    }

    //转移构造。 转移构造的目的很多时候是为了避免"类似深拷贝"， 直接使用"类似浅拷贝"的方式。
    Apples(Apples &&right)
    {
        company_ = std::move(right.company_);
        config_ = right.config_;
        right.config_ = nullptr;
    }

private:
    char *config_ = nullptr;
    std::string company_;
};

Apples apple1;
//会调用到转移构造
Apples apple2 = std::move(apple1);

Apples apple3;
//这里还需要实现一个转移赋值，否则也会出问题。
apple3 = std::move(apple2)
```

