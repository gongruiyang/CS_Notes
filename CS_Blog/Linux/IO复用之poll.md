# 一、poll 系统调用解释

> poll 没有 select 跨平台性能优越
>
> poll 没有 epoll 性能高
>
> 故 poll 我们只做简单了解

## 1. poll  函数原型解释

> poll系统调用功能：poll() 执行与 select 类似的任务，它等待一组文件描述符中的一个就绪的 I/O。

```cpp
#include <poll.h>
int poll(struct pollfd *fds, nfds_t nfds, int timeout);
```

* `fds`：需要监控的`事件结构数组`，即`struct pollfd arr[N]`
* `nfds`：事件结构数组中有效元素的个数
* `timeout`：阻塞等待时间长度，0 表示立即返回，小于 0 表示无限制阻塞，大于 0 表示阻塞 timeout 毫秒
* 返回值：大于 0 表示就绪的文件描述符的个数；等于 0 表示超时；小于 0 表示监控出错

如果任何文件描述符都没有发生请求的事件（并且没有错误），则 poll() 将阻塞，直到其中一个事件发生。



## 2. struct pollfd 结构体

内核中源码定义：

```cpp
struct pollfd {
    int   fd;         /* file descriptor  */
    short events;     /* requested events */
    short revents;    /* returned events  */
};
```

其中`events`是`入参`需要设置的，而`revents`是`出参`，是由`poll`函数内部设置好返回出来的

* `fd`：需要监控的文件描述符，字段 fd 包含打开文件的文件描述符。 如果此字段为负，则忽略相应的 events 字段并且 revents 字段返回零。 （这提供了一种忽略单个 poll 调用的文件描述符的简单方法：简单地否定 fd 字段。）
* `events`：字段 events 是一个`输入参数`，一个掩码位指定应用程序对文件描述符 fd 感兴趣的事件。 如果此字段指定为零，则 fd 的所有事件都将被忽略，并且 revents 返回零。
* `revents`：字段 revents 是一个`输出参数`，由内核填充实际发生的事件。 revents 中返回的位可以包括事件中指定的任何位，或者值 POLLERR、POLLHUP 或 POLLNVAL 之一。 （这三个位在 events 字段中没有意义，只要相应的条件为真，就会在 revents 字段中设置。）

## 3. events/revents 含义

可以在 events/revents 中 设置/返回 的位在 <poll.h> 中定义如下：

```cpp
/* These are specified by iBCS2 */
#define POLLIN		0x0001 // 0000 0001
#define POLLPRI		0x0002 // 0000 0010
#define POLLOUT		0x0004 // 0000 0100
#define POLLERR		0x0008 // 0000 1000 
#define POLLHUP		0x0010 // 0001 0000
#define POLLNVAL	0x0020 // 0010 0000
```

* `POLLIN`：表示有数据要读取
* `POLLPRI`：表示有紧急数据要读取（例如：TCP 套接字上的带外数据）
* `POLLOUT`：表示现在写不会阻塞
* `POLLERR`：错误条件（仅输出）
* `POLLHUP`：流套接字对等关闭连接或关闭写入一半的连接， 必须定义 _GNU_SOURCE 功能测试宏（在包含任何头文件之前）才能获得此定义。
* `POLLNVAL`：无效请求：fd 未打开（仅输出）。

若想同时设置多个属性，只需`将宏按位或运算`即可，例如：`POLLIN | POLLOUT`

## 4. 图解 事件结构数组

![image-20210725144255368](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210725144302.png)

表示事件结构数组中有2个有效元素，poll 只需要监控前两个即可



# 二、poll 系统调用代码演示 之 监控标准输入

> poll.cpp

```cpp
#include <iostream>                                
#include <poll.h>
#include <string>
#include <unistd.h>

#define FDNUMS 10
#define POLLERROR (-1)
using namespace std;

int main()
{
    // 构造poll入参
    struct pollfd arrFD[FDNUMS];
    arrFD[0].fd = 0;
    arrFD[0].events = POLLIN;
    int nfds = 1;
    int timeout = 10;

loop:
    int ret = poll(arrFD, nfds, timeout);
    if(ret == 0)	// 超时重新监控
        goto loop;
    else if(ret < 0) // 监控出错
    {
        perror("poll");
        return POLLERROR;
    }
	
    // stdin文件描述符已就绪
    for(int i = 0; i < FDNUMS; i++)
    {
        // 从标准输入中读取用户输入的数据，再输出
        if(arrFD[i].revents == POLLIN)
        {
            char str[1024] = { 0 };
            read(arrFD[i].fd, str, sizeof(str) - 1);
            cout << "stdin : " << str;
        }
    }

    return 0;
}
```

```shell
[gongruiyang@localhost poll]$ g++ poll.cpp -o poll
[gongruiyang@localhost poll]$ ./poll 
hello world!
stdin : hello world!
```

