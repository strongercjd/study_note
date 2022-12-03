# 1、简介

当我们要遍历容器如vector,map等复杂结构时，可以通过迭代器进行遍历，依次取出容器中的值。通过容器类的begin()和end()函数获取指向第一个元素位置的迭代器和指向最后一个元素下一个位置的迭代器。

# 2、简单使用

通过迭代器可以读取它指向的元素，*迭代器名就表示迭代器指向的元素。

迭代器可以进行++操作。反向迭代器和正向迭代器的区别在于;

- 对正向迭代器进行++操作时，迭代器会指向容器的后一个元素
- 对反向迭代器进行++操作时，迭代器会指向容易的前一个元素

迭代器使用的示例

``` cpp
#include <iostream>
#include <vector>
using namespace std;
int main()
{
    vector<int> v;  //v是存放int类型变量的可变长数组，开始时没有元素
    for (int n = 0; n<5; ++n)
        v.push_back(n);  //push_back成员函数在vector容器尾部添加一个元素
    vector<int>::iterator i;  //定义正向迭代器
    for (i = v.begin(); i != v.end(); ++i) {  //用迭代器遍历容器
        cout << *i << " ";  //*i 就是迭代器i指向的元素
        *i *= 2;  //每个元素变为原来的2倍
    }
    cout << endl;
    //用反向迭代器遍历容器
    for (vector<int>::reverse_iterator j = v.rbegin(); j != v.rend(); ++j)
        cout << *j << " ";
    return 0;
}
```

运行结果

0 1 2 3 4

8 6 4 2 0 

使用迭代器修改元素

``` cpp
void iterator_func()
{
    string s("some string");
    if (s.begin() != s.end())
    {
        auto it = s.begin();
        *it = toupper(*it);
    }
}
```

上面代码修改了字符串第一个字母为大写。

# 3、迭代器运算

迭代器支持加减运算，支持比较运算

``` cpp
*iter 返回iter所指对象得引用
iter->mem 解引用返回iter所指对象得mem成员
++iter 迭代器位置后移，指向下一个元素
--iter 迭代器位置前移，指向上一个元素
iter1 == iter2 判断iter1和iter2是否相等
iter1 != iter2 判断iter1和iter2不相等
iter = iter + n 迭代器iter向后偏移n个元素
iter = iter -n 迭代器iter 向前偏移n个元素
iter1 >= iter2 迭代器iter1指向的元素是否在iter2之后
```

