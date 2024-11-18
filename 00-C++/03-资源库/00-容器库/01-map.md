# 1、简介

map是STL的一个关联容器，以键值对存储的数据，其类型可以自己定义，每个关键字在map中只能出现一次，关键字不能修改，值可以修改；

map同set、multiset、multimap（与map的差别仅在于multimap允许一个键对应多个值）内部数据结构都是红黑树，而java中的hashmap是以hash table实现的。所以map内部有序（自动排序，单词时按照字母序排序），查找时间复杂度为O(logn)。

# 2、用法

**头文件**

``` cpp
#include <map>
```

**定义**

``` cpp
map<string,int> my_map;
/*也可以使用*
typedef map<string,int> My_Map;
My_Map my_map;
```

**基本方法**

| 函数名          | 功能                            |
| --------------- | ------------------------------- |
| my_map.insert() | 插入                            |
| my_map.find()   | 擦查找一个元素                  |
| my_map.clear()  | 清空                            |
| my_map.erase()  | 删除一个元素                    |
| my_map.size()   | map的长度大小                   |
| my_map.begin()  | 返回指向map头部的迭代器         |
| my_map.end()    | 返回指向map末尾的迭代器         |
| my_map.rbegin() | 返回一个指向map尾部的逆向迭代器 |
| my_map.rend()   | 返回一个指向map头部的逆向迭代器 |
| my_map.empty()  | map为空时返回true               |

# 3、插入数据

**第一种：用insert函数插入pair数据：**

``` cpp
map<int,string> my_map;
my_map.insert(pair<int,string>(1,"first"));
my_map.insert(pair<int,string>(2,"second")); 
```

**第二种：用insert函数插入value_type数据：**

``` cpp
map<int,string> my_map;
my_map.insert(map<int,string>::value_type(1,"first"));
my_map.insert(map<int,string>::value_type(2,"second"));
 
map<int,string>::iterator it;           //迭代器遍历
for(it=my_map.begin();it!=my_map.end();it++)
    cout<<it->first<<it->second<<endl;
```

**第三种：用数组的方式直接赋值：**

``` cpp
map<int,string> my_map;
my_map[1]="first";
my_map[2]="second";
 
map<int,string>::iterator it;
for(it=my_map.begin();it!=my_map.end();it++)
    cout<<it->first<<it->second<<endl;
```

# 4、查找元素

判定这个关键字是否在map中出现

第一种：用count函数来判断关键字是否出现，其缺点是无法定位元素出现的位置。由于map一对一的映射关系，count函数的返回值要么是0，要么是1。

``` cpp
map<string,int> my_map;
my_map["first"]=1;
cout<<my_map.count("first")<<endl;    //输出1；
```

第二种：用find函数来定位元素出现的位置，它返回一个迭代器，当数据出现时，返回的是数据所在位置的迭代器；若map中没有要查找的数据，返回的迭代器等于end函数返回的迭代器。

``` cpp
#include <map>  
#include <string>  
#include <iostream>  
 
using namespace std;  
  
int main()  
{  
    map<int, string> my_map;  
    my_map.insert(pair<int, string>(1, "student_one"));  
    my_map.insert(pair<int, string>(2, "student_two"));  
    my_map.insert(pair<int, string>(3, "student_three"));  
    map<int, string>::iterator it;  
    it = my_map.find(1);  
    if(it != my_map.end())  
       cout<<"Find, the value is "<<it->second<<endl;      
    else  
       cout<<"Do not Find"<<endl;  
    return 0;  
}
//通过map对象的方法获取的iterator数据类型是一个std::pair对象，包括两个数据iterator->first和iterator->second，分别代表关键字和value值。
```

# 5、删除元素

示例代码如下

``` cpp
#include <map>  
#include <string>  
#include <iostream>  
  
using namespace std;  
  
int main()  
{  
    map<int, string> my_map;  
    my_map.insert(pair<int, string>(1, "one"));  
    my_map.insert(pair<int, string>(2, "two"));  
    my_map.insert(pair<int, string>(3, "three"));  
    //如果你要演示输出效果，请选择以下的一种，你看到的效果会比较好
    //如果要删除1,用迭代器删除
    map<int, string>::iterator it;  
    it = my_map.find(1);  
    my_map.erase(it);                   //如果要删除1，用关键字删除
    int n = my_map.erase(1);            //如果删除了会返回1，否则返回0
    //用迭代器，成片的删除
    //一下代码把整个map清空
    my_map.erase( my_map.begin(), my_map.end() );  
    //成片删除要注意的是，也是STL的特性，删除区间是一个前闭后开的集合
    //自个加上遍历代码，打印输出吧
    return 0;
} 
```

# 6、排序

map中元素是自动按key升序排序（从小到大）的；按照value排序时，想直接使用sort函数是做不到的，sort函数只支持数组、vector、list、queue等的排序，无法对map排序，那么就需要把map放在vector中，再对vector进行排序。

``` cpp
#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <vector>
using namespace std;
 
bool cmp(pair<string,int> a, pair<string,int> b) {
	return a.second < b.second;
}
 
int main()
{
    map<string, int> ma;
    ma["Alice"] = 86;
    ma["Bob"] = 78;
    ma["Zip"] = 92;
    ma["Stdevn"] = 88;
    vector< pair<string,int> > vec(ma.begin(),ma.end());
    //或者：
    //vector< pair<string,int> > vec;
    //for(map<string,int>::iterator it = ma.begin(); it != ma.end(); it++)
    //    vec.push_back( pair<string,int>(it->first,it->second) );
 
    sort(vec.begin(),vec.end(),cmp);
    for (vector< pair<string,int> >::iterator it = vec.begin(); it != vec.end(); ++it)
    {
        cout << it->first << " " << it->second << endl;
    }
    return 0;
}
```

