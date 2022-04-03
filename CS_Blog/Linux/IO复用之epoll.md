# 一、epoll 接口及结构体介绍

> epoll api 执行与 poll 类似的任务：监视多个文件描述符以查看是否可以对其中任何一个进行I/O
>
> epoll API既可以用作边缘触发接口，也可以用作级别触发接口，并且可以很好地扩展到大量关注的文件描述符
>
> epoll 是当今公认的在Linux操作系统下，性能最高的多路转接IO模型

epoll系列接口：

```cpp
#include <sys/epoll.h>
int epoll_create(int size);
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
```

## epoll_create

创建 epoll 操作句柄，通过操作句柄操控底层的结构体`eventpoll`，这个操作句柄本质上也是一个文件描述符

```cpp
int epoll_create(int size);
```

* `size`：版本大于Linux 2.6.8时传入的 size 参数被忽略，随便传入大于0的参数即可，底层维护的数据结构会自动扩容
* 返回值：返回大于0表示 epoll 操作句柄；返回 -1 表示执行失败

此文件描述符用于对epoll接口的所有后续调用，当不再需要时，应该使用 close 关闭 epoll_create 返回的文件描述符。当所有引用epoll实例的文件描述符都已关闭时，内核将销毁该实例并释放相关资源以供重用。

## epoll_ctl 

向 epoll 维护的红黑树中 添加/修改/删除 节点（节点中存储的是事件结构体）

```cpp
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
```

* `epfd`：epoll操作句柄，即 epoll_create 函数返回值
* `op`：告知 epoll_ctl 需要执行 添加/修改/删除 哪一个操作，每个操作对应一个宏定义
* `fd`：需要操作的文件描述符
* `event`：事件结构体
* 返回值：返回 0 表示操作成功；返回 -1 表示操作失败

请求 epoll 操作句柄 epfd 对目标文件描述符 fd 执行操作op

|     OP宏      | 释义 |
| :-----------: | :--: |
| EPOLL_CTL_ADD | 添加 |
| EPOLL_CTL_DEL | 删除 |
| EPOLL_CTL_MOD | 修改 |

在内核源码中的宏定义：`eventpoll.h`

```cpp
/* Valid opcodes to issue to sys_epoll_ctl() */
#define EPOLL_CTL_ADD 1
#define EPOLL_CTL_DEL 2
#define EPOLL_CTL_MOD 3
```

## epoll_wait

等待 epoll 文件描述符上的I/O事件

```cpp
int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
```

* `epfd`：epoll 操作句柄，即 epoll_create 返回值
* `events`：事件结构数组，将已就绪的事件结构体拷贝至该数组中
* `maxevents`：告诉 epoll 拷贝多少个已就绪的事件结构体
* `timeout`：用于设置超时时间，单位是毫秒，填入-1表示无限期阻塞
* 返回值：就绪文件描述符个数；返回 -1 表示执行出错

## epoll_event 结构体

源码定义：

```cpp
typedef union epoll_data {
    void        *ptr;
    int          fd;
    uint32_t     u32;
    uint64_t     u64;
} epoll_data_t;

struct epoll_event {
    uint32_t     events;      /* Epoll events */
    epoll_data_t data;        /* User data variable */
};
```

* `events`：所关心的文件描述符对应的事件，都是一些宏定义
* `data`：是一个`union`类型，对于 epoll 底层来说有用的只有 fd
  * 只使用 fd ：相当于传递一个文件描述符进去
  * 只使用 ptr：相当于可以传递一个结构体进去，这个结构体包括文件描述符，还可以自己增加一些用于其他作用的带外数据



# 二、底层 eventpoll  结构体

```cpp
struct eventpoll {
    spinlock_t lock;				/* 保护此结构访问							*/
    struct mutex mtx;				/* 用于确保epoll正在使用的文件描述符不会被删除 */    
    wait_queue_head_t wq;			/* sys_epoll_wait()使用的等待队列	 		*/
    wait_queue_head_t poll_wait;	/* file->poll()使用的等待队列     	 		*/
    struct list_head rdllist;		/* 就绪文件描述符双向列表			  		 */
    struct rb_root rbr;				/* 用于存储被监视的fd结构的红黑树根   		   */
    struct epitem *ovflist;			/* 单链表它链接将就绪事件传输到用户空间时发生的所有“struct epitem” */
    struct user_struct *user; 		/* 创建eventpoll描述符的用户 */
};
```

该结构体中比较重要的两个数据结构分别是`红黑树`和`双向链表`

`红黑树`：作用是管理正在监视的`未就绪`的文件描述符

`双向链表`：作用是管理`已经就绪`的文件描述符

如果`红黑树`中的文件描述符从`未就绪状态`转变为`就绪状态`，则会将该结点`复制`至`双向链表`中

![image-20210725180139568](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210725180139.png)





# 三、epoll 系统调用代码演示

## demo1：监视标准输入文件描述符

> epoll.cpp

```cpp
#include <iostream>
#include <unistd.h>
#include <sys/epoll.h>
#include <string.h>

using namespace std;

int main()
{
    // 创建 epoll 结构
    int epfd = epoll_create(1);
    if(epfd < 0)
    {
        perror("epoll_create");
        return -1;
    }

	// 将标准输入文件描述符放入监控中
    int op = EPOLL_CTL_ADD;
    int stdinfd = 0;
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = 0;
    int ret = epoll_ctl(epfd, op, stdinfd, &event);
    if(ret < 0)
    {
        perror("epoll_ctl");
        return -1;
    }

    // 阻塞等待输入事件发生
    struct epoll_event events;
    int maxEvents = 1;
    int timeout = 10;
loop:
    ret = epoll_wait(epfd, &events, maxEvents, timeout);           
    if(ret == 0)
        goto loop;
    else if(ret == -1)
    {
        perror("epoll_wait");
        return -1;
    }

    // 输入事件发生
    if(events.events == EPOLLIN)
    {
        char buf[1024];
        memset(buf, '\0', sizeof(buf));
        ret = read(stdinfd, buf, sizeof(buf) - 1);
        if(ret < 0)
        {
            perror("read");
            return -1;
        }
        cout << "stdin : " << buf ;
    }

    return 0;
}
```

执行过程：

```shell
[gongruiyang@localhost epoll]$ g++ epoll.cpp -o epoll
[gongruiyang@localhost epoll]$ ./epoll 
hello world!
stdin : hello world!
```

# 四、epoll 的两种触发方式

## LT 与 ET



## 代码演示demo1：LT下TCP通信

> TcpServer.hpp

```cpp
#pragma once
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
#include <string.h>

using namespace std;

#define TCPSUCCESS 0
#define TCPERROR (-1)
#define SOCKETINIT (-2)
#define PEERSHUTDOWN (-3)                                                                          

class TcpSvr{
    public:
    TcpSvr()
    {
        socket_fd = SOCKETINIT;;
    }

    ~TcpSvr(){}

    int CreateSocket()
    {
        socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(socket_fd < 0)
        {
            perror("socket");
            return TCPERROR;
        }
        return TCPSUCCESS;
    }

    int Bind(const string& ip, const uint16_t port)
    {
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
        int ret = bind(socket_fd, (struct sockaddr*)&addr, sizeof(addr));
        if(ret < 0)
        {
            perror("bind");
            return TCPERROR;
        }
        return TCPSUCCESS;
    }

    int Listen(int backlog = 5)
    {
        int ret = listen(socket_fd, backlog);
        if(ret < 0)
        {
            perror("listen");
            return TCPERROR;
        }
        return TCPSUCCESS;
    }

    int Accept(TcpSvr* ts, struct sockaddr_in* peer_addr, socklen_t* peer_addrlen)
    {
        int new_socket = accept(socket_fd, (struct sockaddr*)peer_addr, peer_addrlen);
        if(new_socket < 0)
        {
            perror("accept");
            return TCPERROR;
        }
        ts->setSocket(new_socket);
        return TCPSUCCESS;
    }

    int Send(const string& inputData)
    {
        ssize_t ret = send(socket_fd, inputData.c_str(), inputData.size(), 0);
        if(ret < 0)
        {
            perror("send");
            return TCPERROR;
        }
        return TCPSUCCESS;
    }

    int Recv(string& outputData)
    {
        char buf[1024];
        ssize_t ret = recv(socket_fd, buf, sizeof(buf) - 1, 0);
        if(ret < 0)
        {
            perror("recv");
            return TCPERROR;
        }
        else if(ret == 0)
            return PEERSHUTDOWN;
        outputData.assign(buf, strlen(buf));
        return TCPSUCCESS;
    }

    int getSockFd()
    {
        return socket_fd;
    }

    void setSocket(int sockfd)
    {
        socket_fd = sockfd;
    }
    private:
    int socket_fd;
};
```

> EpollServer.hpp

```cpp
#pragma once                                                                     
#include <stdio.h>
#include <sys/epoll.h>
#include <unistd.h>

class EpollSvr
{
public:
    EpollSvr()
    {
        m_epoll_fd = -1;
    }
    ~EpollSvr(){}

    bool createEpoll()
    {
        m_epoll_fd = epoll_create(1);
        if(m_epoll_fd < 0)
        {
            perror("epoll_create");
            return false;
        }
        return true;
    }

    bool controlEpoll(int op, int fd, struct epoll_event* ee)
    {
        int ret = epoll_ctl(m_epoll_fd, op, fd, ee);
        if(ret < 0)
        {
            perror("epoll_ctl");
            return false;
        }
        return true;
    }

    int waitEpoll(struct epoll_event* event, int maxCount, int timeout = -1)
    {
        int ret = epoll_wait(m_epoll_fd, event, maxCount, timeout);
        if(ret < 0)
        {
            perror("epoll_wait");
            return -1;
        }
        else if(ret == 0)
        {
            printf("timeout...\n");
            return -1;
        }
        return ret;
    }

private:
    int m_epoll_fd;
};
```

> main.cpp

```cpp
#include "TcpServer.hpp"                       
#include "EpollServer.hpp"
#include <iostream>
using namespace std;

#define CONNECTION_MAX 10
#define CHECKTCPSVR(ret) if(ret<0){return -1;}
#define CHECKEPOLLSVR(ret) if(!ret){return -1;}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("Useage : ./Epoll_LT [ip] [port]\n");
        return 0;
    }

    // 注册 TCP 服务器
    TcpSvr tcpServer;
    string ip = argv[1];
    uint32_t port = atoi(argv[2]);
    CHECKTCPSVR(tcpServer.CreateSocket());
    CHECKTCPSVR(tcpServer.Bind(ip, port));
    CHECKTCPSVR(tcpServer.Listen());
    int listenFd = tcpServer.getSockFd();

    // 注册 epoll 服务器, 监听 listen socket fd
    EpollSvr epollServer;
    struct epoll_event event;
    int op = EPOLL_CTL_ADD;
    int fd = event.data.fd = listenFd;
    event.events = EPOLLIN;		// 侦听套接字使用 LT 触发
    CHECKEPOLLSVR(epollServer.createEpoll());
    CHECKEPOLLSVR(epollServer.controlEpoll(op, fd, &event));

    // 获取连接
    while(1)
    {
        struct epoll_event arrFd[CONNECTION_MAX];
        int maxCount = CONNECTION_MAX;
        int timeout = -1;
        int ret = epollServer.waitEpoll(arrFd, maxCount, timeout);
        if(ret < 0 ) break;
        else if(ret == 0) continue;

        for(int i = 0; i < ret; i++)
        {
            int fd = arrFd[i].data.fd;
            if(fd == listenFd)  // 侦听套接字已就绪
            {
                // 准备对端地址信息结构体
                TcpSvr clientServer;
                struct sockaddr_in client_addr;
                socklen_t client_addrlen = sizeof(client_addr);
                int ret = tcpServer.Accept(&clientServer, &client_addr, &client_addrlen);
                if(ret < 0) continue;

                // 输出对端地址信息
                cout << "new connetion is coming!" << endl;
                cout << "ip   : " << inet_ntoa(client_addr.sin_addr) << endl;
                cout << "port : " << client_addr.sin_port << endl;

                // 添加新连接的套接字进入epoll中监控
                struct epoll_event event;
                event.events = EPOLLIN;		// 新连接套接字使用 LT 触发
                int clientFd = event.data.fd = clientServer.getSockFd();
                int op = EPOLL_CTL_ADD;
                CHECKEPOLLSVR(epollServer.controlEpoll(op, clientFd, &event));
            }
            else  // 连接套接字已就绪  
            {
                // 设置对端 TCP服务器
                int clientFd = arrFd[i].data.fd;
                TcpSvr clientServer;
                clientServer.setSocket(clientFd);

                // 接收数据
                string peerInput;
                int ret = clientServer.Recv(peerInput);
                if(ret == TCPERROR) continue;
                else if (ret == PEERSHUTDOWN)
                {
                    int op = EPOLL_CTL_DEL;
                    CHECKEPOLLSVR(epollServer.controlEpoll(op, clientFd, NULL));
                    close(clientFd);
                }
                cout << peerInput << endl;

                // 发送数据
                string output;
                cin >> output;
                clientServer.Send(output);
            }// end if 
        }// end for 
    }// end while
    return 0;
}
```



## 代码演示demo2：ET下TCP通信





