# 一、Lambda表达式简介

* Lambda 表达式是 C++11 中语法之一
* Lambda 表达式把函数看作对象，把这个表达式当做对象使用
* Lambda 表达式可以赋值给变量，也可以当做参数传给真正的函数



# 二、Lambda表达式语法解析

## 1. Lambda exp 基本语法结构

```cpp
[ /* captures */ ] /* <tparams> */ ( /* params */ ) /* -> retType */ { /* body */ }	
```

* `captures` : 捕获变量列表，该 Lambda 表达式`上下文中的变量`都可以放在 captures 中，以供 body 中使用，如果程序上下文中的变量不会在 body 中用到，可以不传入
* `tparams` ：模板参数列表，可省略
* `params `：传入的参数列表，若没有参数，可省略`()`
* `retType`：返回值类型，以下两种情况可以不写：①无返回值 ②返回值可以被编译器推导
* `body`：函数体，用于编写具体函数逻辑的地方
* 其中的 tparams \ params \ retType 都可以省略，最简单的表达式结构： `[ ] { }`

##  2. 捕获变量的几种方式

你可以通过`按值传递`和`按引用传递`的方式捕获变量：

- `[&epsilon]` 将参数 epsilon 按`引用`传递给Lambda表达式
- `[epsilon]` 参数 epsilon 按`值`传递给Lambda表达式
- `[&]` 通过按`引用`捕获 Lambda 表达式中使用的`所有变量`
- `[=]` 通过按`值`捕获 Lambda 表达式中使用的`所有变量`
- `[&, epsilon]` 除了参数 epsilon 按`值`传递，其他变量都按`引用`传递
- `[=, &epsilon]` 除了参数 epsilon 按`引用`传递，其他变量都按`值`传递

具体解释：

> 1. 按值捕获：只可以输出内容，不可以对内容进行修改，捕捉列表中的变量为只读，是原变量的一份拷贝

```cpp
int a = 10;
auto lmd = [a] () {
    cout << a << endl; 	// 合法操作
	a += 2; 			// 非法操作
}
```

> 2. 按引用捕获：获得该变量的引用，可以对该变量进行任何操作

```cpp
int a = 10;
auto lmd = [&a] () {
    cout << a << endl; 	// 合法操作
	a += 2; 			// 合法操作
}
```

> 3. `[=]` **按值捕获**所有能捕获的变量：也可以捕获`this`指针
>
> 4. 使用 `[&]` **按引用捕获**所有能捕获的变量，也可以将其他变量**单独按值捕获**，例如：`[&, param1, param2]`

```cpp
#include <iostream>
#include <string>
using namespace std;
class Date{
    private:
        int _year;
        int _month;
        int _day;
    public:
        Date(int y, int m, int d):_year(y), _month(m), _day(d){}
        void showDate()
        {
            string s = "Today is ";
            auto show = [=](){
                cout << s << this->_year << " : " << this->_month << " : " << this->_day << endl;
            };
            show();
        }
};
int main()
{
    Date d(2020, 7, 4);
    d.showDate();
    return 0;
```

注意：如果不使用`=`来捕捉，必须将this传进Lambda表达式中，要不然会报以下错误

```cpp
 error: ‘this’ was not captured for this lambda function
```

## 3. mutable关键字

* 如果使用按值捕获，并且想修改拷贝的值，可以使用mutable关键字修饰

```cpp
int a = 10;
auto lmd = [a] () mutable {
    cout << a << endl; 	// 10
	a += 2; 			// 合法操作，并且不影响 lambda 表达式外面的变量a的值
    cout << a << endl; 	// 12
}
cout << a << endl; 	// 10
```



# 三、Lambda表达式使用示例

* 将 Lambda 表达式当做`函数对象`使用

```cpp
#include <iostream>
using namespace std;

int main()
{
    int a = 10, b = 4;
    auto lamb = [&a, &b](int&& x, int&& y) -> void {
        cout << a << " " << b << " " << x << " " << y << endl;
    };
    lamb(1, 2);

    return 0;
}
```



* 将 Lambda 表达式当做`函数参数`使用：常用于替换仿函数

```cpp
/* 对自定义结构排序 */
#include <iostream>
#include <algorithm>
using namespace std;

struct S
{
    int x;
    int y;
}s[2];

int main()
{
    s[0].x = 0, s[0].y = 2;
    s[1].x = 0, s[1].y = 3;
    sort(s,s + sizeof(s) / sizeof(s[0]), [](S& a, S& b){
        if(a.x == b.x)
            return a.y > b.y;
        return a.x > b.x;
            });
    for(auto e : s)
        cout << e.x << " " << e.y << endl;

    return 0;
}
```



# 四、探究Lambda表达式的实现原理

* 我们发现 Lambda表达式 和 函数对象 极其相似，故我们可以对比看看他俩的汇编代码如何

```cpp
class Rate
{
public:
	Rate(double rate) : _rate(rate){}
	double operator()(double money, int year){
		return money * _rate * year;
	}
private:
	double _rate;
};
int main()
{
	double rate = 0.49;

	// 函数对象
	Rate r1(rate);
	r1(10000, 2);	

	// lambda exp
	auto r2 = [=](double monty, int year)->double{
		return monty * rate * year; 
	};
	r2(10000, 2);

	system("pause");
	return 0;
}
```

> 汇编代码对比

![image-20210704203429574](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210704203429.png)

由此可见，Lambda表达式其实也是一个函数对象，也是调用了 `operator()` 来实现 Lambda表达式 的



# 五、不可省去返回值的情况

当你的返回值可能会出现多种数据类型的情况下，编译器无法自动推导出具体返回值类型，这时，便必须指明返回值类型，例如：

```cpp
void func4(std::vector<double>& v) {
    std::transform(v.begin(), v.end(), v.begin(),
        [](double d) {
            if (d < 0.0001) {
                return 0;	// int
            } else {
                return d;	// double
            }
        });
}
```

以上代码必须使用`-> T`的方式来告诉编译器，返回值究竟使用什么数据类型

```cpp
void func4(std::vector<double>& v) {
    std::transform(v.begin(), v.end(), v.begin(),
        [](double d) -> double {
            if (d < 0.0001) {
                return 0;	// 相当于 (double)0
            } else {
                return d;
            }
        });
}
```

