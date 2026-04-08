# 示例代码

对于类成员函数内绑定类内其他成员函数见文档<03-类中函数指针>

`std::bind` 是 C++11 标准库 `<functional>` 中提供的函数适配器。它的核心作用是**将可调用对象（函数、函数指针、成员函数、函数对象等）与部分参数预先绑定**，生成一个新的可调用对象（通常配合 `std::function` 使用）。简单来说，它实现的是“参数的部分求值”或“参数占位”。

## 基本语法

```cpp
#include <functional>
using namespace std::placeholders; // 占位符 _1, _2, ...

auto newCallable = std::bind(callable, arg_list);
```

- `callable`：需要绑定的原始可调用对象。
- `arg_list`：参数列表，可以包含**具体值**或**占位符**（如 `_1`、`_2`...）。占位符表示该参数将由新可调用对象的实际调用参数提供。

## 主要用途

1. **固定部分参数**  
   将函数的多参数中某些参数固定为常量，减少调用时需要传入的参数数量。

2. **改变参数顺序**  
   通过占位符的重排，实现参数顺序的调整。

3. **绑定成员函数**  
   成员函数不能像普通函数一样直接调用，需要绑定一个对象实例（或指针/引用）。

## 代码示例

```cpp
#include <iostream>
#include <functional>

void greet(const std::string& name, const std::string& title) {
    std::cout << title << " " << name << std::endl;
}

int add(int a, int b) { return a + b; }

struct Multiplier {
    int operator()(int x, int y) const { return x * y; }
    int times(int x, int y) const { return x * y; }
};

int main() {
    using namespace std::placeholders;

    // 1. 固定第一个参数为 "Alice"
    auto greetAlice = std::bind(greet, "Alice", _1);
    greetAlice("Ms.");  // 输出: Ms. Alice

    // 2. 改变参数顺序：原为 (a,b) -> 新为 (b,a)
    auto reversedAdd = std::bind(add, _2, _1);
    std::cout << reversedAdd(3, 5) << std::endl; // 输出 8 (5+3)

    // 3. 绑定函数对象（仿函数）
    Multiplier mul;
    auto bindMul = std::bind(Multiplier(), _1, _2);
    std::cout << bindMul(4, 5) << std::endl; // 20

    // 4. 绑定成员函数（需要对象实例）
    auto bindTimes = std::bind(&Multiplier::times, &mul, _1, _2);
    std::cout << bindTimes(6, 7) << std::endl; // 42

    // 5. 与 std::function 配合使用
    std::function<int(int,int)> func = std::bind(add, _1, _2);
    std::cout << func(10, 20) << std::endl; // 30

    return 0;
}
```

## 占位符详解

- 占位符 `_1`, `_2`, ... 表示新可调用对象的**第几个参数**。
- 占位符放在 `std::bind` 的参数列表中，表示“这个位置的值将在实际调用时由使用者提供”。
- 例如 `std::bind(f, _2, _1)`：新对象有两个参数，调用时第一个参数会传给 `f` 的第二个参数位置，第二个参数会传给 `f` 的第一个参数位置。

## 绑定成员函数的关键点

- 必须显式取地址：`&ClassName::memberFunc`
- 参数列表中需要传入一个**对象实例**：可以是对象本身（值传递）、指针、引用（`std::ref(obj)` 或 `&obj`）。
- 示例：`std::bind(&MyClass::foo, &myObj, _1, 42)`

## 类中的方法绑定

> 这一部分在<01-类_成员函数>中有讲解

## 综合示例

``` cpp
#include <functional>
#include <iostream>
#include <memory>
#include <random>
 
void f(int n1, int n2, int n3, const int& n4, int n5)
{
    std::cout << n1 << ' ' << n2 << ' ' << n3 << ' ' << n4 << ' ' << n5 << '\n';
}
 
int g(int n1)
{
    return n1;
}
 
struct Foo
{
    void print_sum(int n1, int n2)
    {
        std::cout << n1 + n2 << '\n';
    }
 
    int data = 10;
};
 
int main()
{
    using namespace std::placeholders;  // 对于 _1, _2, _3...
 
    std::cout << "1) 参数重排序和按引用传递：";
    int n = 7;
    // （ _1 与 _2 来自 std::placeholders ，并表示将来会传递给 f1 的参数）
    auto f1 = std::bind(f, _2, 42, _1, std::cref(n), n);
    n = 10;
    f1(1, 2, 1001); // 1 为 _1 所绑定， 2 为 _2 所绑定，不使用 1001
                    // 进行到 f(2, 42, 1, n, 7) 的调用
 
    std::cout << "2) 使用 lambda 达成相同效果：";
    n = 7;
    auto lambda = [&ncref = n, n](auto a, auto b, auto /* 未使用 */)
    {
        f(b, 42, a, ncref, n);
    };
    n = 10;
    lambda(1, 2, 1001); // 等同于调用 f1(1, 2, 1001)
 
    std::cout << "3) 嵌套 bind 子表达式共享占位符：";
    auto f2 = std::bind(f, _3, std::bind(g, _3), _3, 4, 5);
    f2(10, 11, 12); // 进行到 f(12, g(12), 12, 4, 5); 的调用
 
    std::cout << "4) 以分布绑定随机数生成器：";
    std::default_random_engine e;
    std::uniform_int_distribution<> d(0, 10);
    std::function<int()> rnd = std::bind(d, e); // e 的一个副本存储于 rnd
    for (int n = 0; n < 10; ++n)
        std::cout << rnd() << ' ';
    std::cout << '\n';
 
    std::cout << "5) 绑定指向成员函数指针：";
    Foo foo;
    auto f3 = std::bind(&Foo::print_sum, &foo, 95, _1);
    f3(5);
 
    std::cout << "6) 绑定是指向成员函数指针的 mem_fn：";
    auto ptr_to_print_sum = std::mem_fn(&Foo::print_sum);
    auto f4 = std::bind(ptr_to_print_sum, &foo, 95, _1);
    f4(5);
 
    std::cout << "7) 绑定指向数据成员指针：";
    auto f5 = std::bind(&Foo::data, _1);
    std::cout << f5(foo) << '\n';
 
    std::cout << "8) 绑定是指向数据成员指针的 mem_fn：";
    auto ptr_to_data = std::mem_fn(&Foo::data);
    auto f6 = std::bind(ptr_to_data, _1);
    std::cout << f6(foo) << '\n';
 
    std::cout << "9) 使用智能指针调用被引用对象的成员：";
    std::cout << f6(std::make_shared<Foo>(foo)) << '\n'
              << f6(std::make_unique<Foo>(foo)) << '\n';
}
```
