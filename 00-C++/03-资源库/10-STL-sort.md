std::sort()函数使用总结

函数声明

```c++
template< class RandomIt, class Compare >
constexpr void sort( RandomIt first, RandomIt last, Compare comp );
```

默认方法，传两个参数，将范围中的元素[first,last]按升序排序。也可以自定义排序

# 默认升序

示例代码如下

```c++
#include <iostream> 
#include <algorithm> 
  
using namespace std; 
  
void show(int a[]) 
{ 
    for(int i = 0; i < 10; ++i) 
        cout << a[i] << " "; 
} 
  
int main() 
{ 
    int a[10]= {1, 5, 8, 9, 6, 7, 3, 4, 2, 0}; 
    cout << "\n 排序前数组为 : "; 
    show(a); 
  
    sort(a, a+10); 
  
    cout << "\n\n 排序后数组为 : "; 
    show(a); 
  
    return 0; 
}
```

运行结果

```c++
排序前数组为 : 1 5 8 9 6 7 3 4 2 0
排序后数组为 : 0 1 2 3 4 5 6 7 8 9
```

# 降序排列

sort（）接受第三个参数，用于指定元素的排序顺序。我们可以传递“greater()”函数来按降序排序。

```c++
#include <bits/stdc++.h> 
using namespace std; 
  
int main() 
{ 
    int arr[] = {1, 5, 8, 9, 6, 7, 3, 4, 2, 0}; 
    int n = sizeof(arr)/sizeof(arr[0]); 
  
    sort(arr, arr+n, greater<int>()); 
  
    cout << " 排序后数组为 : \n"; 
    for (int i = 0; i < n; ++i) 
        cout << arr[i] << " "; 
  
    return 0; 
}
```

运行结果

```
排序后数组为 :
9 8 7 6 5 4 3 2 1 0
```

# 自定义排序

我们也可以编写自己的比较器函数并将其作为第三个参数传递。

```c++
#include<bits/stdc++.h> 
using namespace std; 
  
// 一个时间间隔包括开始时间 start 和结束时间 end
struct Interval 
{ 
    int start, end; 
}; 
  
// 自定义比较函数以时间间隔的升序排序
bool compareInterval(Interval i1, Interval i2) 
{ 
    return (i1.start < i2.start); 
} 
  
int main() 
{ 
    Interval arr[] =  { {6,8}, {1,9}, {2,4}, {4,7} }; 
    int n = sizeof(arr)/sizeof(arr[0]); 
  
   
    sort(arr, arr+n, compareInterval); 
  
    cout << "时间间隔数组以开始时间升序排序后结果为 : \n"; 
    for (int i=0; i<n; i++) 
       cout << "[" << arr[i].start << "," << arr[i].end 
            << "] "; 
  
    return 0; 
}
```

运行结果

```c++
时间间隔数组以开始时间升序排序后结果为 :
[1,9] [2,4] [4,7] [6,8]
```

