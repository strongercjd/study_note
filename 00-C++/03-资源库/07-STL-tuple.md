# 简介
C++11引入了一个新的较实用的模板类型，std::tuple，也即是元组。元组是一个固定大小的不同类型（异质，heterogeneous）值的集合，也即它可以同时存放不同类型的数据。

C++已有的std::pair类型类似于一个二元组，可看作是std::tuple的一个特例，std::tuple也可看作是std::pair的泛化。std::pair的长度限制为2，而std::tuple的元素个数为0~任意个。

# 使用
- 有时候希望将一些数据组合成单一对象，但又不想麻烦地定义一个新数据结构来表示这些数据时，std::tuple是非常有用的。我们可以将std::tuple看作一个“快速而随意”的数据结构。我们可以把它当作一个通用的结构体使用，但又不需要创建和获取结构体的特征，使得程序更加简洁直观。
- 创建一个std::tuple对象时，可以使用tuple的默认构造函数，它会对每个成员进行值初始化；也可以为每个成员提供一个初始值，此时的构造函数是explicit的，因此必须使用直接初始化方法。
- 一个std::tuple类型的成员数目是没有限制的，因此，元组的成员都是未命名的。要访问一个元组的成员，就要使用一个名为get的标准库函数模板。为了使用get，我们必须指定一个显式模板实参，它指出我们想要访问第几个成员。我们传递给get一个tuple对象，它返回指定成员的引用。get尖括号中的值必须是一个整型常量表达式。与往常一样，我们从0开始计数，意味着get<0>是第一个成员。
- 为了使用tuple_size或tuple_element，我们需要知道一个元组对象的类型。与往常一样，确定一个对象的类型的最简单方法就是使用decltype。
- std::tuple的一个常见用途是从一个函数返回多个值。
- std::tuple中元素是被紧密地存储的(位于连续的内存区域)，而不是链式结构。

std::tuple实现了多元组，这是一个编译期就确定大小的容器，可以容纳不同类型的元素。多元组类型在当前标准库中被定义为可以用任意数量参数初始化的类模板。每一模板参数确定多元组中一元素的类型。所以，多元组是一个多类型、大小固定的值的集合。

# 创建和初始化
示例代码
``` cpp
std::tuple<int, double, std::string> first;    // 创建一个空的元组，需要指定元组元素的数据类型，调用各个成员的默认构造函数进行初始化。
std::tuple<int, double, std::string> second(first);  // 拷贝构造
std::tuple<int, char> third(10, 'a');        // 创建并初始化，使用小括号初始化
std::tuple<int, std::string, double> fourth{42, "Test", -3.14};  // 创建并初始化，使用新的大括号初始化列表方式初始化
std::tuple<int, char> fifth(std::make_tuple(20, 'b'));     // 移动构造，使用模板库的make_tuple
first = std::make_tuple(1, 3.14, "tuple");       // 移动赋值
int i_third = 3;
std::tuple<int&> sixth(std::ref(i_third));   // 创建一个元组，元组的元素可以被引用
```

# 元素的访问与修改

示例代码如下

``` cpp
int n = 1;
auto t = std::make_tuple(10, "Test", 3.14, std::ref(n), n);
// get尖括号中的值必须是一个整型常量表达式。从0开始计数，意味着get<0>是第一个成员。
std::cout << "The value of t is "  << "("
<< std::get<0>(t) << ", " << std::get<1>(t) << ", "
<< std::get<2>(t) << ", " << std::get<3>(t) << ", "
<< std::get<4>(t) << ")\n";
// 由于get返回指定元素的引用，所以可用来修改指定位置的元素的值。此处因为第4个元素是引用类型，所以被引用的值也会改变
std::get<3>(t) = 9;
std::cout << n << std::endl;
```

# 元素个数
示例代码如下
``` cpp
std::tuple<char, int, long, std::string> first('A', 2, 3, "4");
int i_count = std::tuple_size<decltype(first)>::value;  // 使用std::tuple_size计算元组个数
std::cout << "the number of elements of a tuple:" << i_count << "\n";
```

# 元组解包
std::tie() 元组包含一个或者多个元素，使用std::tie解包： 首先需要定义对应元素的变量，再使用tie。
``` cpp
// std::tie: function template, Constructs a tuple object whose elements are references
// to the arguments in args, in the same order
// std::ignore: object, This object ignores any value assigned to it. It is designed to be used as an
// argument for tie to indicate that a specific element in a tuple should be ignored.

int myint;
char mychar;

std::tuple<int, float, char> mytuple;

mytuple = std::make_tuple(10, 2.6, 'a');          // packing values into tuple

std::tie(myint, std::ignore, mychar) = mytuple;   // unpacking tuple into variables

std::cout << "myint contains: " << myint << '\n';
std::cout << "mychar contains: " << mychar << '\n';
```

# 元素类型获取
获取元组中某个元素的数据类型，需要用到另外一个类型： std::tuple_element。

语法： std::tuple_element<index, tuple>。

示例代码如下
``` cpp
std::tuple<int, std::string> third(9, std::string("ABC"));

// 得到元组第1个元素的类型，用元组第一个元素的类型声明一个变量
std::tuple_element<1, decltype(third)>::type val_1;

// 获取元组的第一个元素的值
val_1 = std::get<1>(third);
std::cout << "val_1 = " << val_1.c_str() << "\n";
```

# 元组拼接

使用 std::tuple_cat 执行拼接

示例代码如下
``` cpp
std::tuple<char, int, double> first('A', 1, 2.2f);
// 组合到一起, 使用auto， 自动推导
auto second = std::tuple_cat(first, std::make_tuple('B', std::string("-=+")));
// 组合到一起，可以知道每一个元素的数据类型时什么 与 auto推导效果一样
std::tuple<char, int, double, char, std::string> third = std::tuple_cat(first, std::make_tuple('B', std::string("-=+")));
// 输出合并后的元组内容
int index = 0;
std::cout << index++ << " = " << std::get<0>(second) << "\n";  // 0 = A
std::cout << index++ << " = " << std::get<1>(second) << "\n";  // 1 = 1 
std::cout << index++ << " = " << std::get<2>(second) << "\n";  //  2 = 2.2
std::cout << index++ << " = " << std::get<3>(second) << "\n";  // 3 = B
std::cout << index++ << " = " << std::get<4>(second).c_str() << "\n"; // 4 = -=+
```

# 元组的遍历
元组没用提供operator []重载，遍历起来较为麻烦，需要为其单独提供遍历模板函数

``` cpp
template<class T>
void print_single(T const& v)
{
  if constexpr (std::is_same_v<T, std::decay_t<std::string>>)
    std::cout << std::quoted(v);
  else if constexpr (std::is_same_v<std::decay_t<T>, char>)
    std::cout << "'" << v << "'";
  else
    std::cout << v;
}

// helper function to print a tuple of any size
template<class Tuple, std::size_t N>
struct TuplePrinter {
  static void print(const Tuple& t)
  {
    TuplePrinter<Tuple, N-1>::print(t);
    std::cout << ", ";
    print_single(std::get<N-1>(t));
  }
};

template<class Tuple>
struct TuplePrinter<Tuple, 1>{
  static void print(const Tuple& t)
  {
    print_single(std::get<0>(t));
  }
};

template<class... Args>
void print(const std::tuple<Args...>& t)
{
  std::cout << "(";
  TuplePrinter<decltype(t), sizeof...(Args)>::print(t);
  std::cout << ")\n";
}
// end helper function
```