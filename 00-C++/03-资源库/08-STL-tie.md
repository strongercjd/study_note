# 简介
std::tie多数是和std::tuple组合使用的。

一般std::tie有2个作用：

- 创建一个std::tuple;
- 解包标准库中的一些集合类，简化访问方法。

# 生成tuple

生成tuple的常规操作
``` cpp
auto student2 = std::make_tuple(11, 98, "Tom" );
```
使用tie的操作
``` cpp
string name3 = "david3";
int id3 = 3;
double d3 = 99.3;
tuple<int, double, string>  student3 = std::tie(id3, d3, name3);
```


# 解包
直接使用tuple中的示例代码
``` cpp
int myint;
char mychar;

std::tuple<int, float, char> mytuple;

mytuple = std::make_tuple(10, 2.6, 'a');          // packing values into tuple

std::tie(myint, std::ignore, mychar) = mytuple;   // unpacking tuple into variables

std::cout << "myint contains: " << myint << '\n';
std::cout << "mychar contains: " << mychar << '\n';
```