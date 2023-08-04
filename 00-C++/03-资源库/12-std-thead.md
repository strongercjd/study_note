# 基本的用法

最简单的 std::thread 范例如下所示，调用 thread 将立即同时开始执行这个新建立的线程，之后 main() 的主线程也会继续执行，基本上这就是一个基本的建立多线程的功能
``` cpp
#include <iostream>
#include <thread>
#include <unistd.h> //linux下用来调用sleep函数延时的

void myfunc2()
{
    std::cout << "myfunc2\n";
}

void myfunc1()
{
    std::cout << "myfunc1\n";
}

int main()
{
    std::thread t1(myfunc1);
    std::cout << "creat t1\n";
    sleep(2); // 延迟2秒
    std::thread t2(myfunc2);
    std::cout << "creat t2\n";
    t1.join();//阻塞当前线程等待t1运行完毕
    t2.join();
    return 0;
}
```
编译报错如下
``` cpp
$ g++ demo.cpp 
/usr/bin/ld: /tmp/ccgaUGZ4.o: in function `std::thread::thread<void (&)(), , void>(void (&)())':
demo.cpp:(.text._ZNSt6threadC2IRFvvEJEvEEOT_DpOT0_[_ZNSt6threadC5IRFvvEJEvEEOT_DpOT0_]+0x33): undefined reference to `pthread_create'
collect2: error: ld returned 1 exit status
```
这个错误时由于链接错误引起的，需要在编译时链接多线程库(pthread),在操作系统中，线程由pthread库实现的，为了解决这个问题，需要在编译时添加"-pthread"选项
``` cpp
$ g++ -pthread demo.cpp 
$ ./a.out 
creat t1
myfunc1
creat t2
myfunc2
```
> 线程创建完毕立刻执行的，调用t1.join();只是会阻塞当前线程直至t1线程执行完毕
>
> 在多线程中常常会互相对某函数或变量进行操作，需要对这些进行上锁，这不是本文的重点，其他文章详细说

# 常用成员函数

以下为 c++ std::thread 常用的成员函数
- get_id(): 取得目前的线程 id，回传一个为 std::thread::id 的类型
- joinable(): 检查是否可join
- join(): 等待线程完成
- detach(): 与该线程分离，一旦该线程执行完后它所分配的资源会被释放
- native_handle(): 取得平台原生的native handle

其他相关的常用函数有
- sleep_for(): 停止目前线程一段指定的时间
- yield(): 暂时放弃CPU一段时间，让给其它线程

# 线程执行类的成员函数

c++ std::thread 的构建可以传入 class 类别中的函数，如下范例所示

AA::start 分别建立 t1、t2 两个线程，而 t1 传入 AA::a1 类别函数，AA::a1 前面记得要加上&，第二参数代表的是哪个类别，之后的参数为传入函数所需的参数
``` cpp
#include <iostream>
#include <thread>

class AA
{
public:
    void a1()
    {
        std::cout << "a1\n";
    }

    void a2(int n)
    {
        std::cout << "a2 " << n << "\n";
    }

    void start()
    {
        std::thread t1(&AA::a1, this);
        std::thread t2(&AA::a2, this, 10);

        t1.join();
        t2.join();
    }
};

int main()
{
    AA a;
    a.start();

    return 0;
}
```
执行结果
``` cpp
$ g++ -pthread demo.cpp
$ ./a.out 
a1
a2 10
```

# join和detach

在 main 主线程建立 t1 线程后，主线程便继续往下执行，如果主线程需要等 t1 执行完毕后才能继续执行的话就需要使用 join，即等待 t1 线程执行完 foo 后主先线程才能继续执行，否则主线程会一直阻塞在 join 这一行
``` cpp
#include <iostream>
#include <thread>
#include <chrono>

void foo()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::cout << "foo" << std::endl;
}

int main()
{
    std::thread t1(foo);
    std::cout << "main 1" << std::endl;
    t1.join();
    std::cout << "main 2" << std::endl;
    return 0;
}
```
执行结果
``` cpp
$ g++ -pthread demo.cpp
$ ./a.out 
main 1
foo
main 2
```
如果主线程不想等或是可以不用等待 t1 线程的话。就可以使用 detach 来让 t1 线程分离，接着主线程就可以继续执行，t1线程也在继续执行，在整个程序结束前最好养成好习惯确保所有子线程都已执行完毕，因为在 linux 系统如果主线程执行结束还有子线程在执行的话会报错
``` cpp
#include <iostream>
#include <thread>
#include <chrono>

void foo()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::cout << "foo" << std::endl;
}

int main()
{
    std::thread t1(foo);
    std::cout << "main 1" << std::endl;
    t1.detach();
    std::cout << "main 2" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    return 0;
}
```
执行结果
``` cpp
$ g++ -pthread demo.cpp
$ ./a.out 
main 1
main 2
foo
```

# 使用数组或vector建立多个线程

使用数组建立多个线程
``` cpp
#include <iostream>
#include <thread>

void foo(int n)
{
    std::cout << "foo() " << n << "\n";
}

int main()
{
    std::thread threads[3];

    for (int i = 0; i < 3; i++)
    {
        threads[i] = std::thread(foo, i);
    }

    for (int i = 0; i < 3; i++)
    {
        threads[i].join();
    }

    std::cout << "main() exit.\n";

    return 0;
}
```
运行结果
``` cpp
$ ./a.out 
foo() foo() foo() 0
1
2
main() exit.
```
使用vector建立多个线程
``` cpp
#include <iostream>
#include <thread>
#include <vector>

void foo(int n)
{
    std::cout << "foo() " << n << std::endl;
}

int main()
{
    std::vector<std::thread> threads;

    for (int i = 0; i < 3; i++)
    {
        threads.push_back(std::thread(foo, i));
    }

    for (int i = 0; i < 3; i++)
    {
        threads[i].join();
    }

    std::cout << "main() exit.\n";

    return 0;
}
```
运行结果
``` cpp
$ ./a.out 
foo() 1
foo() 0
foo() 2
main() exit.
```

# 参数传递使用传引用

定义一个myfunc，其参数传递方式为传引用
``` cpp
void myfunc(int &n)
{
    std::cout << "myfunc n=" << n << "\n";
    n += 10;
}
```
希望建立另外一个线程去执行 myfunc，之后需要取得这个 myfunc 的运算结果，但建立线程时如果写 std::thread t1(myfunc, n);编译会出现错误，为什么会这样呢？

**因为在 std::thread 的参数传递方式为传值，要传参考的话需要透过 std::ref 来辅助达成，**所以就会写成这样，myfunc 与 myfunc2 的参数传递方式不同，可以看看这两者之间的差异
``` cpp
#include <iostream>
#include <thread>

void myfunc(int &n)
{
    std::cout << "myfunc n=" << n << "\n";
    n += 10;
}

void myfunc2(int n)
{
    std::cout << "myfunc n=" << n << "\n";
    n += 10;
}

int main()
{
    int n1 = 5;
    std::thread t1(myfunc, std::ref(n1));
    t1.join();
    std::cout << "main n1=" << n1 << "\n";

    int n2 = 5;
    std::thread t2(myfunc2, n2);
    t2.join();
    std::cout << "main n2=" << n2 << "\n";

    return 0;
}
```
运行结果
``` cpp
$ g++ -pthread demo.cpp
$ ./a.out 
myfunc n=5
main n1=15
myfunc n=5
main n2=5
```

