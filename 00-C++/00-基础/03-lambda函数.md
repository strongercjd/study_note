# 1、lambda函数

lambda函数示例

``` cpp
#include <iostream>

int main()
{
    int a = 1;
    int b = 2;
    // 定义一个lambda函数
    auto sum = [](int x, int y) -> int
    {
        return x + y;
    };
    std::cout << sum(a, b) << std::endl; // 3
    return 0;
}
```

示例lambda函数sum，输入两个int的参数，返回int型的两个入参的和。

通常情况下，lambda函数的语法定义如下：

``` cpp
[capture](parameters) mutable -> return-type{statement}
```

- [capture]，捕捉列表。**可能为空**，捕捉列表总是在lambda函数的开始，[]是lambda函数的引出符。编译器根据该引出符确定接下来的代码是不是lambda函数。捕捉列表能够捕捉上下文中的变量以供lambda函数使用。
- (parameters)，参数列表，**可选部分**，和普通函数的参数列表一致，如果不需要参数可以连()一起省略。
- mutable，一个修饰符。默认情况下，lambda函数总是一个const函数，mutable可以取消其常量性。在使用mutable时，参数列表不可省略，即使参数为空。
- ->return-type，返回类型。**可选部分**，用追踪返回类型声明函数的返回值。不需要返回值时，可以连同符号->一起省略。另外，在返回类型明确的情况下，也可以省略，让编译器推导出返回类型。
- statement，函数体，**可能为空**，与普通函数一样，但是可以使用捕捉列表中的变量。

lambda函数的各种形式

``` cpp
int main()
{
    int a = 1;
    int b = 2;

    [] {};                             // 最简单
    [=] {return a + b; };              // 省略了参数列表和返回值类型
    auto func1 = [&](int c) {b = a + c; };    // 省略了返回值类型
    auto func2 = [=, &b](int c)->int { return b += a + c; };  // 完整的lambda函数

    return 0;
}
```

通过上面的代码可以发现，起始lambda函数和普通函数之间最特殊的地方就是，lambda函数可以捕捉上下文的数据进行使用。我们用捕捉的方式来完成最开始的例子：

``` cpp
int main()
{
    int a = 1;
    int b = 2;
    // 定义一个lambda函数
    auto sum = [a,b]{
        return a + b;
    };

    std::cout << sum() << std::endl;   // 3

    return 0;
}
```

和最开始的lambda函数相比，此处的lambda函数在原型上发生了变化，不再需要传递参数，这个变化看起来似乎没有很特殊。不过在后面的介绍中，会详细讲解捕捉列表的特点。

另外，我们可以看到捕捉列表中会使用一些特殊符号，这是有一定含义的。我们来看一下捕捉列表的几种形式：

- [var]表示以值传递的方式来捕捉变量var。
- [=]表示以值传递的方式捕捉所有父作用域的变量，包括this指针。
- [&var]表示引用传递捕捉变量var。
- [&]表示引用传递捕捉所有父作用域的变量，包括this指针。
- [this]表示值传递方式捕捉当前的this指针。

以上的方式可以组合使用，但是不允许对同一个变量以同一方式重复捕捉。

在块作用域(可以理解为在{}以内的代码)以外的lambda函数捕捉列表必须为空，这种lambda和普通函数除了语法上不同以外，和普通函数差别不大。在块作用域内的lambda函数只能捕捉父作用域中的自动变量，不能捕捉非此作用域的变量或者非自动变量(如静态变量等)。

# 2、基础使用

有时候，我们需要对一个方法中的变量进行状态监控，且在这个方法中会多次调用。往往我们会封装一个函数去做这件事情，但是实际上其他的地方又用不到这个函数，如果用lambda函数来做这件事，可以使代码更加易读：

``` cpp
#include <iostream>

int main(void)
{
    int a = 1;
    int b = 2;
    
    auto func = [&]
    { std::cout << a << "," << b << std::endl; };

    int count = 0;
    while (count < 10)
    {
        int c = a + b;
        func();
        a += 1;
        b += 2;
        func();
        ++count;
    }
}
```

# 3、捕获方式

lambda捕捉列表的捕捉方式不同，结果也会不同。
- 引用捕获
- 赋值捕获

``` cpp
#include <iostream>

int main()
{
    int i = 1;
    auto func1 = [=] {i++; std::cout << i << std::endl; };//赋值捕获

    return 0;
}
```
编译报错如下
``` cpp
$ g++  demo.cpp 
demo.cpp: In lambda function:
demo.cpp:6:23: error: increment of read-only variable ‘i’
    6 |     auto func1 = [=] {i++; std::cout << i << std::endl; };//赋值捕获
```
说明赋值捕获时，i是只读的，不能被写入，下文有提到，可以使用mutable解决这个问题


``` cpp
#include <iostream>

int main()
{
    int i = 1;
    auto func1 = [=]
    { std::cout << "赋值捕获:" << i << std::endl; }; // 赋值捕获
    auto func2 = [&]
    { std::cout << "引用捕获:" << i++ << std::endl; }; // 引用捕获

    func1(); // 1
    std::cout << "赋值捕获后" << i << std::endl;
    func2(); // 1
    std::cout << "引用捕获后" << i << std::endl;

    return 0;
}
```
运行结果
``` cpp
$ ./a.out 
赋值捕获:1
赋值捕获后1
引用捕获:1
引用捕获后2
```

# 4、很函数指针对比

在最开始，lambda函数的类型看起来和函数指针的很像，都是把函数赋值给了一个变量。

后来了解到lambda函数是用仿函数实现的，又认为它是一种自定义的类。

而事实上，lambda类型并不是简单的函数指针类型或者自定义类型。lambda函数是一个闭包(closure，特有的、匿名的且非联合体的class类型)的类，没有lambda表达式都会产生一个闭包类型的临时对象(右值)。

因此，严格来说lambda函数并非函数指针，但是C++11允许lambda表达式向函数指针的转换，前提是没有捕捉任何变量且函数指针所指向的函数必须跟lambda函数有相同的调用方式：

``` cpp
typedef int(*pfunc)(int x, int y);

int main()
{
    auto func = [](int x, int y)->int {
        return x + y;
    };
    pfunc p1 = nullptr;
    p1 = func;

    std::cout << p1(1, 2) << std::endl;

    return 0;
}
```

函数指针和Lambda函数
``` cpp
#include <iostream>
#include <functional>

void printff(int input)
{
    std::cout << "printff " << input << '\n';
    return;
}

void function_tmp(void ptr(int))
{
    ptr(1);
}
void function_lambda(std::function<void(int)> ptr)
{
    ptr(2);
}

int main()
{
    int data = 10;
    function_tmp(printff);

    function_tmp([](int puter)
                 { std::cout << "puter " << puter << '\n'; });

    /* 错误的，当lambda函数有捕获时，必须使用function_lambda */
    // function_tmp([=](int puter)
    //              { std::cout << "data " << data << "  puter " << puter << '\n'; });

    function_lambda([=](int puter)
                    { std::cout << "data " << data << "  puter " << puter << '\n'; });
    return 0;
}
```
运行结果
``` cpp
$ ./a.out 
printff 1
puter 1
data 10  puter 2
```

# 4、常量性和mutable关键字

``` cpp
int main()
{
    int val = 0;
    auto const_val_lambda = [=] { val = 3; };    // 编译失败，不能在const的lambda函数中修改

    auto mutable_val_lambda = [=]() mutable { val = 3; };

    auto const_ref_lambda = [&] { val = 3; };

    auto const_param_lambda = [](int v) { v = 3; };
    const_param_lambda(val);

    return 0;
}
```

可以看到在const的lambda函数中无法修改捕捉到的参数。前面说到过，lambda函数是通过仿函数来实现的，捕捉参数相当于是仿函数类中的成员变量，而lambda函数相当于是成员函数，const成员函数自然不能修改普通成员变量。

# 5、lambda与STL

lambda对于C++最大的改变音轨是在STL库中，它使得STL的算法使用更加容易。首先我们看一下for_each算法：

``` cpp
int s = 0;
inline void sum(int i)
{
    s += i;
}

int main()
{
    std::vector<int> vec = {0,1,2,3,4};
    std::for_each(vec.begin(), vec.end(), sum);
    std::cout << s << std::endl;

    int s1 = 0;
    std::for_each(vec.begin(), vec.end(), [&](int i) {
        s1 += i;
    });
    std::cout << s1 << std::endl;

    return 0;
}
```

可以看到，原来使用for_each算法需要定义一个函数来作为参数进行计算。用了lambda之后，代码结构更加清晰和简单。不过，针对这个例子使用C++11中的for循环更加方便，但是如果循环中的算法更加复杂，可能用lambda+for_each会比较好。

# 6.举例
cartographer中的代码
``` cpp
/**
 * @brief 在node_handle中订阅topic,并与传入的回调函数进行注册
 *
 * @tparam MessageType 模板参数,消息的数据类型
 * @param[in] handler 函数指针, 接受传入的函数的地址
 * @param[in] trajectory_id 轨迹id
 * @param[in] topic 订阅的topic名字
 * @param[in] node_handle ros的node_handle
 * @param[in] node node类的指针
 * @return ::ros::Subscriber 订阅者
 */
template <typename MessageType>
::ros::Subscriber SubscribeWithHandler(
    void (Node::*handler)(int, const std::string&,
                          const typename MessageType::ConstPtr&),
    const int trajectory_id, const std::string& topic,
    ::ros::NodeHandle* const node_handle, Node* const node) {
  return node_handle->subscribe<MessageType>(
      topic, kInfiniteSubscriberQueueSize,  // kInfiniteSubscriberQueueSize = 0
      // 使用boost::function构造回调函数,被subscribe注册
      boost::function<void(const typename MessageType::ConstPtr&)>(
          // c++11: lambda表达式
          [node, handler, trajectory_id,
           topic](const typename MessageType::ConstPtr& msg) {
            (node->*handler)(trajectory_id, topic, msg);
          }));
}
```
调用方式举例
``` cpp
SubscribeWithHandler<sensor_msgs::LaserScan>(
             &Node::HandleLaserScanMessage, trajectory_id, topic, &node_handle_,
             this)
```

# 8.编译

lambda函数实际被编译成编译成**可执行的类**

下面是一个典型的生产者和消费者模式
``` cpp
#include <iostream>
#include <thread>
#include <queue>
#include <chrono>
#include <mutex>
#include <condition_variable>
/* conditional variable queue  条件变量队列
条件变量是一种线程间的同步机制，用于满足特定条件之前使线程等待
它通常与互斥锁(mutex)一起使用，在多线程编成中实现线程间同步与协调*/
class condvarQueue
{
    std::queue<int> produced_nums;
    std::mutex m;
    std::condition_variable cond_var;
    bool done = false; // 是否结束
    bool notified = false;

public:
    void push(int i)
    {
        std::unique_lock<std::mutex> lock(m);
        produced_nums.push(i);
        notified = true;
        cond_var.notify_one();
    }
    // 消费者
    template <typename Consumer>
    void consume(Consumer consumer)
    {
        std::unique_lock<std::mutex> lock(m);
        while (!done)
        {
            while (!notified) // 使用notified条件循环避免假的唤醒
            {
                cond_var.wait(lock);
            }
            while (!produced_nums.empty())
            {
                consumer(produced_nums.front());
                produced_nums.pop();
            }
            notified = false;
        }
    }

    void close()
    {
        {
            std::lock_guard<std::mutex> lock(m);
            done = true;
            notified = true;
        }
        cond_var.notify_one();
    }
};

int main()
{
    condvarQueue queue;
    // 生产者线程
    std::thread producer([&]()
                         {
        for (int i = 0; i < 5; ++i) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "producing " << i << '\n';
            queue.push(i);
        }
        queue.close(); });

    // 消费者线程
    std::thread consumer([&]()
                         { queue.consume([](int input)
                                         { std::cout << "consuming " << input << '\n'; }); });

    producer.join();
    consumer.join();
}
```
那么我们就来研究生产者的lambda函数
``` cpp
std::thread producer([&]()
                        {
    for (int i = 0; i < 5; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "producing " << i << '\n';
        queue.push(i);
    }
    queue.close(); });
```
lambda函数被编译器编译成如下可执行的类
``` cpp
class producer_lambda
{
public:
    producer_lambda(condvarQueue &queue) : _queue(queue){};
    ~producer_lambda(){};

    void operator()()
    {
        for (int i = 0; i < 5; ++i)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "producing " << i << '\n';
            _queue.push(i);
        }
        _queue.close();
    }

private:
    condvarQueue &_queue;
};
```
因为condvarQueue &_queue;是引用，所以[&]

那么上述完整的生产者和消费者模式修改如下
``` cpp
#include <iostream>
#include <thread>
#include <queue>
#include <chrono>
#include <mutex>
#include <condition_variable>
/* conditional variable queue  条件变量队列
条件变量是一种线程间的同步机制，用于满足特定条件之前使线程等待
它通常与互斥锁(mutex)一起使用，在多线程编成中实现线程间同步与协调*/
class condvarQueue
{
    std::queue<int> produced_nums;
    std::mutex m;
    std::condition_variable cond_var;
    bool done = false; // 是否结束
    bool notified = false;

public:
    void push(int i)
    {
        std::unique_lock<std::mutex> lock(m);
        produced_nums.push(i);
        notified = true;
        cond_var.notify_one();
    }
    // 消费者
    template <typename Consumer>
    void consume(Consumer consumer)
    {
        std::unique_lock<std::mutex> lock(m);
        while (!done)
        {
            while (!notified) // 使用notified条件循环避免假的唤醒
            {
                cond_var.wait(lock);
            }
            while (!produced_nums.empty())
            {
                consumer(produced_nums.front());
                produced_nums.pop();
            }
            notified = false;
        }
    }

    void close()
    {
        {
            std::lock_guard<std::mutex> lock(m);
            done = true;
            notified = true;
        }
        cond_var.notify_one();
    }
};
class producer_lambda
{
public:
    producer_lambda(condvarQueue &queue) : _queue(queue){};
    ~producer_lambda(){};

    void operator()()
    {
        for (int i = 0; i < 5; ++i)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "producing " << i << '\n';
            _queue.push(i);
        }
        _queue.close();
    }

private:
    condvarQueue &_queue;
};
int main()
{
    condvarQueue queue;
    // 生产者线程
    producer_lambda _producer(queue);
    std::thread producer(_producer);

    // 消费者线程
    std::thread consumer([&]()
                         { queue.consume([](int input)
                                         { std::cout << "consuming " << input << '\n'; }); });

    producer.join();
    consumer.join();
    return 0;
}
```
运行结果是一样的

消费者lambda函数，这是两个lambda函数
``` cpp
std::thread consumer([&]()
                        { queue.consume([](int input)
                                        { std::cout << "consuming " << input << '\n'; }); });
```
其中第2个lambda函数是
``` cpp
[](int input)
   { std::cout << "consuming " << input << '\n'; }
```
而queue.consume是一个函数模板，传入的是一个类
``` cpp
template <typename Consumer>
void consume(Consumer consumer)
{
    std::unique_lock<std::mutex> lock(m);
    while (!done)
    {
        while (!notified) // 使用notified条件循环避免假的唤醒
        {
            cond_var.wait(lock);
        }
        while (!produced_nums.empty())
        {
            /* 传入的lambda函数是这里执行的，因为传入的是可执行的类 */
            consumer(produced_nums.front());
            produced_nums.pop();
        }
        notified = false;
    }
}
```