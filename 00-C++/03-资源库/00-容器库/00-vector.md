# 1、vector介绍

向量 vector 是一种对象实体, 能够容纳许多其他类型相同的元素, 因此又被称为容器。 与string相同, vector 同属于STL(Standard Template Library, 标准模板库)中的一种自定义的数据类型, 可以广义上认为是数组的增强版 ,与数组相比，vector就是一个可以不用再初始化就必须制定大小的边长数组，当然了，它还有许多高级功能。

在使用它时, 需要包含头文件vector。

``` cpp
#include<vector>
```

# 2、vector声明和初始化

vector 型变量的声明以及初始化的形式也有许多, 常用的有以下几种形式:

``` cpp
/* 初始化vector对象的方法 */
vector<T> v1 ;                          //v1是一个空的vector,它潜在的元素是T类型的，执行默认初始化
vector<T> v2(v1) ;                      //v2中包含v1中所有元素的副本
vector<T> V2 = V1;                      //与上等价
vector<T> v3(n, val) ;                  //声明一个初始大小为10且初始值都为1的向量
vector<T> v4(n) ;                       //v4包含了n个重复地执行了值初始化的对象
vector<T> v5{a,b,c,...}                 //v5包含了初始值个数的元素，每个元素给赋予对应的初值
vector<T> v5 = {a,b,c,...}              //与上等价
vector<T> v6(v5.begin(), v5.begin()+3) ;   //将v5向量中从第0个到第2个(共3个)作为向量v6的初始值
```

如果vector的元素类型是int，默认初始化为0；如果vector元素类型为string，则默认初始化为空字符串。除此之外, 还可以直接使用数组来初始化向量,例如 :

``` cpp
vector<int> v1;               
vector<father> v2;
vector<string> v3;            
vector<vector<int> >;          //注意空格。这里相当于二维数组int a[n][n];
vector<int> v5 = { 1,2,3,4,5 };//列表初始化,注意使用的是花括号
vector<string> v6 = { "hi","my","name","is","lee" };
vector<int> v7(5, -1);         //初始化为-1,-1,-1,-1,-1。第一个参数是数目，第二个参数是要初始化的值
vector<string> v8(3, "hi");
vector<int> v9(10);            //默认初始化为0
vector<int> v10(4);            //默认初始化为空字符串
 
/* 直接使用数组来初始化向量 */
int n[] = {1, 2, 3, 4, 5} ;
vector<int> v11(n, n+5) ;      //将数组n的前5个元素作为向量a的初值
vector<int> v11(&n[1], &n[4]) ;//将n[1] - n[4]范围内的元素作为向量v11的初值
```

# 3、vector元素读写

元素的输入和访问可以像操作普通的数组那样, 用cin>>进行输入, cout<<a[n] 这样进行输出，示例:

``` cpp
#include<iostream>
#include<vector>
using namespace std ;
int main()
{
    vector<int> a(10, 0) ;      //大小为10初值为0的向量a 
    /*  对其中部分元素进行输入  */
    cin >>a[2] ;
    cin >>a[5] ;
    cin >>a[6] ; 
    /* 全部输出 */
    for(int i=0; i<a.size(); i++)
        cout<<a[i]<<" " ;
 
    return 0 ;
}
```

注意：只能对已存在的元素进行赋值或者修改操作，如果是要**加入新元素，务必使用push_back**。push_back的作用有两个：告诉编译器为新元素开辟空间、将新元素存入新空间里。

比如下面的代码是错误的，但是编译器不会报错，就像是数组越界。

``` cpp
vector<int> vec;     //vec为空
vec[0] = 1;          //错误！不能给空向量直接赋值，应该用push_back()
```

下面这样写是正确的

``` cpp
vector<int> vec(3);    //vec声明时定义了长度为3，默认vec中有三个元素0
vec[0] = 1;            //正确！相当于将vec中第0个元素的值0变成1   
```

向量元素的位置也是一种数据类型, 在向量中迭代器的类型为: vector<int>::iterator。 迭代器不但表示元素位置, 还可以再容器中前后移动。我们也可以选择使用迭代器(iterator)来访问元素：

``` cpp
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main()
{
    vector<string> v6 = {"hi", "my", "name", "is", "lee"};
    for (vector<string>::iterator iter = v6.begin(); iter != v6.end(); iter++)
    {
        cout << *iter << endl;
    }    
    return 0;
}
```

上面是正向迭代，如果我们想从后往前迭代该如何操作？使用反向迭代器(reverse_iterator)，如下：

``` cpp
for (vector<string>::reverse_iterator iter = v6.rbegin(); iter != v6.rend(); iter++)
{
    cout << *iter << endl;   //访问iter所指向的元素
}
```

# 4、vetor基本操作

基本操作

``` cpp
 v.size()                 //返回向量v中的元素个数
 v.empty()                //若v中不包含任何元素，返回真；否则返回假
 v.(t)           //向v的尾端添加一个值为t的元素
 v.front()                //访问v的第一个元素
 v.back()                 //访问v的最后一个元素
 v[n]                     //返回v中第n个位置上元素的应用
 v1 = v2 ;                //将v2中的元素拷贝替换v1中的元素
 v1 = {a,b,c,...}         //用列表中元素拷贝替换v1中的元素
 ==、!=、>、>=、<、<=      //惯有含义，以字典顺序进行比较 ;
```

添加或删除元素的操作

``` cpp
push_back()                  //把传送为参数的对象添加到vector的尾部
pop_back()                   //删除vector尾最后一个元素
erase()                      //删除一个或多个元素
  --v.erase(v.begin()) ;             //将起始位置的元素删除
  --v.erase(v.begin(), v.begin()+3) ;//将(v.begin(), v.begin()+3)之间的3个元素删除
  --v.erase(v.begin() + 3);  //删除第4个元素
clear()                      //清除所有元素
insert()                    //插入一个或多个对象
  --v.insert(v.begin(), 1000);       //将1000插入到向量v的起始位置前
  --v.insert(v.begin() + 1,9);       //在第二个位置插入新元素
  --v.insert(v.begin(), 3, 1000) ;   //位置0开始，连续插入3个1000
  --v.insert(v.begin() + 1, 7,8);    //位置1开始，连续插入7个8
  --vector<int> a(5, 1) ;
    vector<int> b(10) ;
    b.insert(b.begin(), a.begin(), a.end());//将a.begin(), a.end()之间的全部元素插入到b.begin()前
```

交换

``` cpp
 v2.swap(v1) ;           //v1向量与v2向量进行交换
```

vector的局限：**但凡使用了迭代器的循环体，都不要向迭代器所属的容器添加元素**！任何一种可能改变vector对象容量的操作，如push_back，都会使迭代器失效。

push_back和inset的区别：

- push_back把元素插入容器末尾，insert把元素插入任何你指定的位置。
- push_back速度一般比insert快，如果能用push_back尽量先用push_back。

# 5、push_back和下标

在编程过程中，经常会遇到需要建一个大的vector，并对里面的元素逐一顺序求值。此时我们有两种选择：

- 先reserve(n)后，再逐一的push_back
- 先建立含n元素的vector，再逐一的通过下标为其赋值

那么，这两个方法哪个会更快一些呢？

``` cpp
// test1  向量通过push_back赋值
int main() {
    int i=100000;
    for(int j=0;j< i;j++){
        vector<int> sz;
        sz.reserve(10000);
        for(int k=0;k<10000;k++)
            sz.push_back(1);
    }
    return 0;
}
 
//test2  向量直接通过下标赋值
int main() {
    int i=100000;
    for(int j=0;j<i;j++){
        vector<int> sz(10000);
        for(int k=0;k<10000;k++)
            sz[k]=1;
    }
    return 0;
}

```

用g++编译后，在ubuntu上测试两个程序的运行时间，发现test1需要20.289s,而test2竟只需要5.435s。且更换数据，多次均是如此。 由此，显然可知，先建立n个元素的vector，再一一用下标取值要快的多。 与n次push_back的时间相比，遍历vector为每个元素的初始化所花时间完全是微不足道的。

故，在已知vector的数据的时候，直接在声明时给出元素个数，是十分明智且效率高的选择。在数据存取时，在同等情况下，可尽量用数组下标存取元素，减少调用push_back的次数。

# 6、读取

前面已经提到读取操作如下

```cpp
v[n]                     //返回v中第n个位置上元素的应用
```

operator[]主要是为了与C语言进行兼容。它可以像C语言数组一样操作。但at()是我们的首选，因为at()进行了边界检查，如果访问超过了vector的范围，将抛出一个例外

```cpp
vector<int> v;
v.reserve(10);
 
for(int i=0; i<7; i++)
    v.push_back(i);
 
try
{
    int iVal1 = v[7];  // 没有检查边界，不会抛出异常
    int iVal2 = v.at(7); // 进行边界检查，如果超出范围则会抛出异常
}
 
catch(const exception& e)
{
    cout << e.what();
}
```





# 7、二维向量

与数组相同, 向量也可以增加维数, 例如声明一个m*n大小的二维向量方式可以像如下形式:

``` cpp
vector< vector<int> > b(10, vector<int>(5));        //创建一个10*5的int型二维向量
```

在这里, 实际上创建的是一个向量中元素为向量的向量。同样可以根据一维向量的相关特性对二维向量进行操作, 如：

``` cpp
#include<iostream>
#include<vector>
using namespace std ;
 
int main()
{
    vector< vector<int> > b(10, vector<int>(5, 0)) ;
 
    //对部分数据进行输入
    cin>>b[1][1] ;
    cin>>b[2][2] ;
    cin>>b[3][3];
 
    //全部输出
    int m, n ;
    for(m=0; m<b.size(); m++)           //b.size()获取行向量的大小
    {
        for(n=0; n<b[m].size(); n++)    //获取向量中具体每个向量的大小
            cout<<b[m][n]<<" " ;
        cout<<"\n" ;
     }
 
    return 0;
}
```

# 8、向量和数组

- **内存中的位置**    C++中数组为内置的数据类型，存放在栈中，其内存的分配和释放完全由系统自动完成；vector，存放在堆中，由STL库中程序负责内存的分配和释放，使用方便。
- **大小能否变化**    数组的大小在初始化后就固定不变，而vector可以通过 push_back 或 pop 等操作进行变化。
- **用size获取长度**  数组在定义时就确定了长度，不能用size获取长度；vector可以用size 获取长度。
- **初始化**    数组不能将数组的内容拷贝给其他数组作为初始值，也不能用数组为其他数组赋值；而vector可以。
- **执行效率**    数组>vector向量。主要原因是vector的扩容过程要消耗大量的时间。