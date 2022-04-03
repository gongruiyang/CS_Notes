# select系统调用

## 函数原型解释

```cpp
#include <sys/select.h>
int select(int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout);
```

* nfds：需要监控的文件描述符中最大的文件描述符值 + 1
* readfds：可读事件对应的文件描述符集合
* writefds：可写事件对应的文件描述符集合
* exceptfds：异常事件对应的文件描述符集合
* timeout：超时时间
* 返回值：就绪（可读、可写和异常）文件描述符的总数

### 事件集合结构体 fd_set

fd_set内核源码：

```cpp
typedef struct {
	unsigned long fds_bits [1024 / (8 * sizeof(unsigned long))];
} __kernel_fd_set;
typedef __kernel_fd_set		fd_set;
```

由内核源码可以看出，在64位Linux操作系统下fd_set实际上是一个`unsigned long fds_bits [16]`位图，该位图一共有`1024个位`，位上如果是`1`表示该号的文件描述符是需要监控的，位上如果是`0`表示该号的文件描述符是不需要监控的，每一位标记一个文件描述符

![image-20210714165137239](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210714165144.png)

对于位图上面的每一位，系统需要逐位判断该位是否为1，可以根据第一个参数`nfds`来设定遍历上限，这样可以减少遍历次数

### 操作 fd_set 的接口

```cpp
void FD_CLR(int fd, fd_set *set);
int  FD_ISSET(int fd, fd_set *set);
void FD_SET(int fd, fd_set *set);
void FD_ZERO(fd_set *set);
```

* `FD_CLR`：将 set 位图上的第 fd 位置为 0
* `FD_ISSET`：判断 set 位图上的第 fd 位是否为 1
* `FD_SET`：将 set 位图上的第 fd 位置为 1
* `FD_ZERO`：将 set 位图每一位都置为 0

### fd_set 接口实现源码

```cpp
#define	__FDMASK(d)	(1UL << ((d) % (8 * sizeof(unsigned long))))
#define	__FDELT(d)	((d) / (8 * sizeof(unsigned long)))

#define	__FD_SET(d, set)	((set)->fds_bits[__FDELT(d)] |= __FDMASK(d))
#define	__FD_CLR(d, set)	((set)->fds_bits[__FDELT(d)] &= ~__FDMASK(d))
#define	__FD_ISSET(d, set)	(((set)->fds_bits[__FDELT(d)] & __FDMASK(d)) != 0)
#define	__FD_ZERO(set)	\
  ((void) memset ((void *) (set), 0, sizeof (__kernel_fd_set)))
```

前三个接口由源码可知是通过将1进行移d个位后与原位图进行按位操作来实现的

最后一个接口由源码可知是通过使用memset方式将整个位图的二进制位置0的

### 超时时间结构体 timeval

```cpp
struct timeval {
	long tv_sec;	/* seconds 秒数 */
	long tv_usec;	/* microseconds 微秒数 */
};
```

若该参数传递NULL，表示阻塞监控，直到某个文件描述符就绪

若该参数传递0，表示非阻塞监控，select立即返回，即`tv_sec=0 && tv_usec=0`

若该参数传递其他，表示带有超时时间的监控，当超时时间达到还没有某个文件描述符就绪，select返回，如果在超时时间中有文件描述符就绪，

### select使用须知

当select运行完毕后，会将传入的三个事件集合参数中未就绪的文件描述符置为0，如果你再想监控原来的文件描述符就需要手动给再次加上，这样会导致非常的麻烦，我们可以传递一个拷贝文件描述符集合的临时变量进去，原位图不让select修改即可，下图中的问题便解决了

![image-20210714174237358](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210714174237.png)



## 代码测试 select

### select监控 0 号文件描述符

```cpp
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>

int main()
{
    fd_set readfds;
    FD_ZERO(&readfds);		// 清空位图
    FD_SET(0, &readfds);	// 将 0 号置为 1
    int ret = select(1, &readfds, NULL, NULL, NULL);
    if(ret < 0)
    {
        perror("select");
        return 1;
    }
    else printf("select ret : %d\n", ret);

    char buf[1024] = {0};
    read(0, buf, sizeof(buf));
    printf("buf:%s\n", buf);

    return 0;
}
```

输出：

```shell
gcc test.cpp && ./a.out
hello world!
select ret : 1
buf:hello world!
```

### select监视新连接

```cpp
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main()
{
  // 1. 创建监听套接字
  int listen_sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(listen_sockfd < 0)
  {
    perror("socket");
    return 0;
  }
  // 2. 绑定地址信息：IP 端口 协议
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(19999);
  addr.sin_addr.s_addr = inet_addr("0.0.0.0");
  int ret = bind(listen_sockfd, (struct sockaddr*)&addr, sizeof(addr));
  if(ret < 0)                                                            
  {
    perror("bind");
    return 0;
  }
  // 3. 监听
  listen(listen_sockfd, 2);
  // 4. 创建一个select监视位图，监控"监听套接字文件描述符"
  fd_set readfds;
  FD_ZERO(&readfds);
  FD_SET(listen_sockfd, &readfds);
  while(1)
  {
    // 设置select监听位图属性：非阻塞监控
    struct timeval tv;
    tv.tv_sec  = 2;
    tv.tv_usec = 0;
    fd_set tmp = readfds;
    int ret    = select(listen_sockfd + 1, &tmp, NULL, NULL, &tv);
    if(ret < 0)
    {
      perror("select");
      return 0;
    }
    else if(ret == 1 &&  FD_ISSET(listen_sockfd, &tmp) == 1)
    {
      // 来了一个新连接
      int new_sock = accept(listen_sockfd, NULL, NULL);
      if(new_sock < 0)
      {
        perror("accept");
        continue;
      }
      printf("accept a connection : sockfd[%d]\n", new_sock);
    }
    else printf("No connections\n");
  } // end while 

  return 0;
}
```

输出现象：使用其他电脑`telnet ip port`即可产生以下现象

```shell
[gongruiyang@localhost select]$ gcc seledemo.c -o main && ./main
No connections
accept a connection : sockfd[4]
No connections
No connections
No connections
accept a connection : sockfd[5]
No connections
^C
```

每来一个`telnet连接`都会为该连接分配一个文件描述符



### 封装select类和TCP类

> select_svr.hpp

```cpp
#pragma once 
#include <sys/select.h>
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <algorithm>
#include <vector>
#include "tcp_server.hpp"

using namespace std;

#define SELECT_ERR (-1)
#define TIMEOUT (-2)                                                

class SelectSvr{
    public:
    SelectSvr()
    {
        FD_ZERO(&readfds_);
        nfds_ = -1;
    }
    ~SelectSvr(){}

    // 添加文件描述符
    void addFD(int fd)
    {
        FD_SET(fd, &readfds_);
        nfds_ = max(nfds_, fd);
    }

    // 删除文件描述符
    void delFD(int fd)
    {
        FD_CLR(fd, &readfds_);
        if(fd < nfds_)
            return ;
        for(int i = nfds_; i >= 0; i--)
        {
            if(FD_ISSET(i, &readfds_) == 1)
            {
                nfds_ = i;
                break;
            }
        }
    }

    // 监控
    int selectWait(struct timeval* tv, vector<TcpSvr>& vec)
    {
        fd_set tmp = readfds_;
        int ret = select(nfds_ + 1, &tmp, NULL, NULL, tv);
        if(ret < 0)
        {
            perror("select");
            return SELECT_ERR;
        }
        else if(ret == 0)
        {
            cout << "Time Out ..." << endl;
            return TIMEOUT;
        }

        for(int i = 0; i <= nfds_; i++)
        {
            if(FD_ISSET(i, &readfds_) == 1)
            {
                TcpSvr ts;
                ts.setSocket(i);
                vec.push_back(ts);
            }
        }
        return ret;
    }
    private:
    // 读事件集合
    fd_set readfds_;
    // 轮询范围:最大文件描述符值
    int nfds_;
};

```

> tcp_server.hpp

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

> main.cpp

```cpp
#include "tcp_server.hpp"                                                                 
#include "select_svr.hpp"
#define CHECK(val) if(val==TCPERROR){return -1;}
using namespace std;
int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        cout << "usage : ./tcpsvr [ip] [port]" << endl;
        return -1;
    }

    // 创建监听服务器类实例
    TcpSvr ts;
    string ip = argv[1];
    u_int16_t port = atoi(argv[2]);
    CHECK(ts.CreateSocket());
    CHECK(ts.Bind(ip, port));
    CHECK(ts.Listen());

    // 创建监控类类实例
    SelectSvr selectSvr;

    // 监控服务类实例
    selectSvr.addFD(ts.getSockFd());

    while(1)
    {
        struct timeval tv;
        tv.tv_sec  = 2;
        tv.tv_usec = 0;
        vector<TcpSvr> vec;
        vec.clear();
        int ret = selectSvr.selectWait(&tv, vec);

        if(ret == TIMEOUT)
            continue;
        else if(ret == SELECT_ERR)
            return SELECT_ERR;

        for(size_t i = 0; i < vec.size(); i++)
        {
            if(vec[i].getSockFd() == ts.getSockFd())
            {
                // 监听套接字已经就绪，可以接受新连接了
                TcpSvr new_ts;
                struct sockaddr_in peer_addr;
                socklen_t peer_addrlen = sizeof(peer_addr);
                ts.Accept(&new_ts, &peer_addr, &peer_addrlen);
                cout << "new socket is "  << new_ts.getSockFd()             << endl;
                cout << "peer ip is"      << inet_ntoa(peer_addr.sin_addr)  << endl;
                cout << "peer port is "   << ntohs(peer_addr.sin_port)      << endl;
                selectSvr.addFD(new_ts.getSockFd());  // 将新连接的fd放入监控位图中
            }
            else
            {
                // 取出新连接的套接字
                TcpSvr newConTcoSvr = vec[i];
                // 接受数据
                string dataFromPeer;
                dataFromPeer.clear();
                int ret = newConTcoSvr.Recv(dataFromPeer);
                if(ret == TCPERROR || ret == PEERSHUTDOWN)
                {
                    // 释放文件描述符 + 从监控中移除该文件描述符
                    int newConFD = newConTcoSvr.getSockFd();
                    close(newConFD);
                    selectSvr.delFD(newConFD);
                    continue;
                }
                cout << "peer data : " << dataFromPeer << endl;

                string dataResp;
                cin >> dataResp;
                newConTcoSvr.Send(dataResp);
            }
        }
    }

    close(ts.getSockFd());
    return 0;
}
```

