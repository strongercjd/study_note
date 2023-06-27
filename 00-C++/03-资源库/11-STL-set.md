set集合容器实现了红黑树（Red-Black Tree）的平衡二叉检索树的的数据结构，

在插入元素时，它会自动调整二叉树的排列，把该元素放到适当的位置，以确保每个子树根节点的键值大于左子树所有节点的键值，而小于右子树所有节点的键值；

另外，还得确保根节点的左子树的高度与有字数的高度相等，

这样，二叉树的高度最小，从而检索速度最快。

> 它不会重复插入相同键值的元素，而采取忽略处理。
> 
> 平衡二叉检索树的检索使用中序遍历算法，
> 
> 检索效率高于vector、deque、和list的容器。
> 
> 平衡二叉检索树在插入元素时，就会自动将元素按键值从小到大的顺序排列。

使用示例
``` cpp
set<int>::iterator it;             // 定义前向迭代器  
set<int>::reverse_iterator rit;    // 定义反向迭代器
it = s.erase(it++);                // 删除
it = s.find(6);                    // 查找的元素  
s.count(a)                         // 是否在集合中
```

元素不是结构体，那么可以编写比较函数。下面的程序比较规则为按键值从大到小的顺序插入到集合中。
``` cpp
#include <iostream>
#include <set>
using namespace std;
struct mycomp
{ // 自定义比较函数，重载“（）”操作符
    bool operator()(const int &a, const int &b)
    {
        if (a != b)
            return a > b;
        else
            return a > b;
    }
};
int main()
{
    set<int, mycomp> s; // 采用比较函数mycomp
    s.insert(5);        // 第一次插入5，可以插入
    s.insert(1);
    s.insert(6);
    s.insert(3);
    s.insert(5); // 第二次插入5，重复元素，不会插入
    set<int, mycomp>::iterator it;
    for (it = s.begin(); it != s.end(); it++)
        cout << *it << " ";
    cout << endl;
    return 0;
}
```
运行结果
``` cpp
$ ./a.out 
6 5 3 1 
```

如果元素是结构体，那么可以直接把比较函数写在结构体内。
``` cpp
#include <iostream>
#include <set>
#include <string>
using namespace std;
struct Info
{
    string name;
    double score;
    bool operator<(const Info &a) const // 重载“<”操作符，自定义排序规则
    {
        // 按score由大到小排序。如果要由小到大排序，使用“>”即可。
        return a.score < score;
    }
};
int main()
{
    set<Info> s;
    Info info;

    // 插入三个元素
    info.name = "Jack";
    info.score = 80;
    s.insert(info);

    info.name = "Tom";
    info.score = 99;
    s.insert(info);

    info.name = "Steaven";
    info.score = 60;
    s.insert(info);

    set<Info>::iterator it;
    for (it = s.begin(); it != s.end(); it++)
        cout << (*it).name << " : " << (*it).score << endl;
    return 0;
}
```
运行结果
``` cpp
$ ./a.out 
Tom : 99
Jack : 80
Steaven : 60
```