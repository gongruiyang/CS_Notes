# 1. C++日期和时间简介

* C ++标准库没有提供正确的日期类型。C ++从C继承了用于日期和时间操作的结构和函数。要访问与日期和时间相关的函数和结构，您需要在 C ++ 程序中包括 ctime 头文件。
* 有四种与时间相关的类型：clock_t，time_t，size_t和tm结构体。
* 类型clock_t，size_t和time_t能够将系统时间和日期表示为某种整数
* tm结构体中可以保存日期和时间信息

# 2. 保存日期和时间的结构体tm

```cpp
struct tm {
    int tm_sec;   // seconds of minutes from 0 to 61
    int tm_min;   // minutes of hour from 0 to 59
    int tm_hour;  // hours of day from 0 to 24
    int tm_mday;  // day of month from 1 to 31
    int tm_mon;   // month of year from 0 to 11
    int tm_year;  // year since 1900
    int tm_wday;  // days since sunday
    int tm_yday;  // days since January 1st
    int tm_isdst; // hours of daylight savings time
}
```

# 3. 时间相关API

## 3.1 时间操纵函数

```cpp
clock_t clock (void);
time_t time (time_t* timer);
time_t mktime (struct tm * timeptr);
double difftime (time_t end, time_t beginning);
```

* clock：返回程序消耗的处理器时间。
* time：返回 自1970年1月1日 至 系统当前日历时间 所经过的秒数
* mktime：将 tm 结构体转换为 time_t 类型
* difftime：返回 beginning 和 end 之间相差的秒数

## 3.2 时间转换函数

```cpp
char* asctime (const struct tm * timeptr);
char* ctime (const time_t * timer);
struct tm * gmtime (const time_t * timer);
struct tm * localtime (const time_t * timer);
size_t strftime (char* ptr, size_t maxsize, const char* format,
                 const struct tm* timeptr );
```

* asctime：将 tm 结构体类型 转换成 字符串类型 并返回，格式为：day month date hours:minutes:seconds year
* ctime：将 time_t类型 转换成 字符串类型 并返回
* gmtime：将 time_t类型 转换成 UTC时间 并返回
* localtime：将 time_t类型 转换成 本地时间 并返回
* strftime：以特定格式格式化日期和时间

`程序1`

```cpp
// 获取当前系统时间
time_t now = time(0);

// 将当前系统时间从time_t类型转化为字符串类型
char* dt = ctime(&now);

cout << "The local date and time is: " << dt << endl;

// 将当前系统时间从time_t类型转化为tm结构体类型
tm *gmtm = gmtime(&now);
dt = asctime(gmtm);
cout << "The UTC date and time is:" << dt << endl;
```

> 输出：
>
> ```
> The local date and time is: Thu Feb 25 17:18:58 2021
> 
> The UTC date and time is:Thu Feb 25 09:18:58 2021
> ```

`程序2`

```cpp
// 获取当前系统时间
time_t now = time(0);
cout << "Number of sec since January 1,1970 is:: " << now << endl;

// 将time_t类型的时间转换成tm结构体
tm *ltm = localtime(&now);

// 从tm结构体中获取日期时间相关信息
cout << "Year:" << 1900 + ltm->tm_year << endl;
cout << "Month: " << 1 + ltm->tm_mon << endl;
cout << "Day: " << ltm->tm_mday << endl;
cout << "Time: " << 5 + ltm->tm_hour << ":";
cout << 30 + ltm->tm_min << ":";
cout << ltm->tm_sec << endl;
```

> 输出：
>
> ```
> Number of sec since January 1,1970 is:: 1614245549
> Year:2021
> Month: 2
> Day: 25
> Time: 22:62:29
> ```



