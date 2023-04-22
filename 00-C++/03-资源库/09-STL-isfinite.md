定义在头文件 <cmath> 中

bool isfinite( float num );  (C++11 起)
bool isfinite( double num );   (C++23 前)
bool isfinite( long double num ); (C++23 起)
constexpr bool isfinite( /* 浮点类型 */ num );(C++23 起)



**额外重载**
在头文件 <cmath> 定义
template< class Integer >   (C++11 起)
bool isfinite( Integer num );(C++23 起 constexpr)

确定给定的浮点数 num 是否拥有有限值，即它是正规、非正规或零，但不是无穷大或 NaN。标准库提供所有以无 cv 限定的浮点类型作为参数 num 的类型的重载。 (C++23 起)

为所有整数类型提供额外重载，将它们当做 double



**参数**
num	-	浮点或整数值
**返回值**
在 num 拥有有限值时返回 true，否则返回 false。

**注解**
额外重载不需要以删上述的形式提供。它们只需要能够对它们的整数类型实参 num 确保 std::isfinite(num) 和 std::isfinite(static_cast<double>(num)) 的效果相同。



**示例代码**

```c++
#include <cfloat>
#include <cmath>
#include <iostream>
 
int main()
{
    std::cout << std::boolalpha
              << "isfinite(NaN) = " << std::isfinite(NAN) << '\n'
              << "isfinite(Inf) = " << std::isfinite(INFINITY) << '\n'
              << "isfinite(0.0) = " << std::isfinite(0.0) << '\n'
              << "isfinite(exp(800)) = " << std::isfinite(std::exp(800)) << '\n'
              << "isfinite(DBL_MIN/2.0) = " << std::isfinite(DBL_MIN / 2.0) << '\n';
}
```

运行结果

```c++
isfinite(NaN) = false
isfinite(Inf) = false
isfinite(0.0) = true
isfinite(exp(800)) = false
isfinite(DBL_MIN/2.0) = true
```

