# 1. TCP 编程流程图

![image-20210115111308789](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210115111308789.png)

# 2. 数据收发阶段使用的API

```c
#include <sys/types.h>
#include <sys/socket.h>

int send(int s, const void *msg, size_t len, int flags);
ssize_t recv(int sockfd, void *buf, size_t len, int flags);
```

## 2.1 send接口

```c
int send(int sockfd, const void *msg, size_t len, int flags);
```

* sockfd：**客户端**调用则填入客户端创建出来的套接字描述符；**服务端**调用则填入服务端创建出来的**用于数据收发**的套接字描述符
* msg：待发送的数据
* len：待发送的数据的长度
* flags：填入0表示`阻塞发送`

返回值：小于0表示失败

## 2.2 recv接口

```c
ssize_t recv(int sockfd, void *buf, size_t len, int flags);
```

* sockfd：**客户端**调用则填入客户端创建出来的套接字描述符；**服务端**调用则填入服务端创建出来的**用于数据收发**的套接字描述符
* buf：接收数据的缓冲区
* len：缓冲区的最大接受数据长度
* flags：填入0表示`阻塞接收`

返回值：

| 返回值ssize_t | 含义                 |
| ------------- | -------------------- |
| <0            | 接收失败             |
| =0            | `对端关闭连接`       |
| >0            | 接收了多少个字节数据 |



# 3. 两个队列

* TCP编程中有两个队列：`未完成连接队列`和`已完成连接队列`
* 未完成连接队列：保存目前`正在处于`**三次握手**之中的连接
* 已完成连接队列：保存的是`已经完成`三次握手的连接



# 4. TCP 编程双端流程

![image-20210115125222533](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210115125222533.png)



# 5. 单线程：TCP协议下 单客户端与单服务端 socket通信



## 5.1 客户端代码

```cpp
#include <stdio.h>                                                                  
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;

int main()
{
    // 1. 创建套接字
    int sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(sockfd < 0)
    {
        perror("socket");
        return -1;
    }

    // 2. 发起连接
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.153.128");
    addr.sin_port = htons(19999);
    int ret = connect(sockfd,(struct sockaddr*)&addr,sizeof(addr));
    if(ret < 0)
    {
        perror("connect");
        return -1;
    }

    while(1)
    {
        // 3. 发送数据
        string s;
        cout<<"You want to say:";
        fflush(stdout);
        getline(cin,s);
        ret = send(sockfd, s.c_str(),s.size(),0);
        if(ret < 0)
        {
            perror("send");
            continue;
        }

        // 4. 接受数据
        char buf[1024] = {0};
        ret = recv(sockfd, buf,strlen(buf)-1,0);
        if(ret < 0)
        {
            perror("recv");
            continue;
        }
        else if(ret == 0)
        {
            printf(" server shutdown!\n");
            break;
        }
        cout << "server say:" << buf << endl;
    }


    // 5. 关闭套接字
    close(sockfd);

    return 0;
}
```

## 5.2 服务端代码

```cpp
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>                                                                        
#include <iostream>
using namespace std;

int main()
{
    // 1. 创建套接字
    //创建一个使用流式套接字+IPv4的套接字
    int listen_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listen_sockfd < 0)
    {
        perror("socket");
        return -1;
    }

    // 2. 绑定地址信息  
    char ip_str[] = "192.168.153.128";
    uint32_t ip = inet_addr(ip_str);
    uint16_t port = htons(19999);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ip;
    addr.sin_port = port;
    int ret = bind(listen_sockfd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret < 0)
    {
        perror("bind");
        return -1;
    }

    // 3. 监听：告诉内核已经准备好可以接受连接啦
    ret = listen(listen_sockfd, 1);
    if(ret < 0)
    {
        perror("listen");
        return -1;
    }

    // 4.接受连接 
    int new_sockfd = accept(listen_sockfd, NULL, NULL);
    if(new_sockfd < 0)
    {
        perror("accept");
        return -1;
    }

    while(1)
    {
        // 5. 接收数据
        char buf[1024] = { 0 };
        ret = recv(new_sockfd, buf, strlen(buf) - 1, 0);
        if(ret < 0)
        {
            perror("recv");
            continue;
        }
        else if(ret == 0)
        {
            // 若客户端已经关闭连接，则需要关闭用于数据收发的套接字
            printf(" client shutdown !\n");
            close(new_sockfd);
            break;
        }
        cout << "client say:" << buf << endl;

        // 6. 发送数据
        string s;
        cout << "you want to say:";
        getline(cin,s);
        ret = send(new_sockfd,s.c_str(),s.size(),0);
        if(ret < 0)
        {
            perror("send");
            continue;
        }
    }

    // 7. 关闭监听套接字
    close(listen_sockfd);

    return 0;
}
```



## 5.3 两个终端交互演示

```shell
[gongruiyang@localhost client]$ g++ cli.cpp -o client
[gongruiyang@localhost client]$ ./client 
You want to say:hello Server!
server say:hello Client!
You want to say:Bye~
server say:Bye~
You want to say:^C
```

```shell
[gongruiyang@localhost client]$ g++ cli.cpp -o client
[gongruiyang@localhost client]$ ./client 
You want to say:hello Server!
server say:hello Client!
You want to say:Bye~
server say:Bye~
You want to say:^C
```



## 5.4 尝试多客户端连接发现问题

* 再启动一个客户端，尝试连接正在运行的服务端，发现不能与服务端正常通信
* 原因分析：是因为我们accept到一个连接请求后，一直在while中循环接收与发送数据，没有继续调用accept，导致不能接收新的连接请求

* 所以当前的这个TCP，是无法处理多个连接的
* 解决方法：在服务端使用`多线程`或`多线程`来处理

# 6. 多进程：TCP协议下 多客户端与单服务端 socket通信

## 6.1 客户端代码

```cpp
#include <stdio.h>                                                                  
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;

int main()
{
    // 1. 创建套接字
    int sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(sockfd < 0)
    {
        perror("socket");
        return -1;
    }

    // 2. 发起连接
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.153.128");
    addr.sin_port = htons(19999);
    int ret = connect(sockfd,(struct sockaddr*)&addr,sizeof(addr));
    if(ret < 0)
    {
        perror("connect");
        return -1;
    }

    while(1)
    {
        // 3. 发送数据
        string s;
        cout<<"You want to say:";
        fflush(stdout);
        getline(cin,s);
        ret = send(sockfd, s.c_str(),s.size(),0);
        if(ret < 0)
        {
            perror("send");
            continue;
        }

        // 4. 接受数据
        char buf[1024] = {0};
        ret = recv(sockfd, buf,strlen(buf)-1,0);
        if(ret < 0)
        {
            perror("recv");
            continue;
        }
        else if(ret == 0)
        {
            printf(" server shutdown!\n");
            break;
        }
        cout << "server say:" << buf << endl;
    }


    // 5. 关闭套接字
    close(sockfd);

    return 0;
}
```



## 6.2 服务端代码

```cpp
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <iostream>
#include <string>
using namespace std;

#include <signal.h>
#include <wait.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void signal_handler(int signum)
{
    wait(NULL);
}

int main()
{
    // 1. 创建套接字
    //创建一个使用流式套接字+IPv4的套接字
    int listen_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listen_sockfd < 0)
    {
        perror("socket");
        return -1;
    }

    // 2. 绑定地址信息  
    char ip_str[] = "192.168.153.128";
    uint32_t ip = inet_addr(ip_str);
    uint16_t port = htons(19999);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ip;
    addr.sin_port = port;
    int ret = bind(listen_sockfd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret < 0)
    {
        perror("bind");
        return -1;
    }

    // 3. 监听：告诉内核已经准备好可以接受连接啦
    ret = listen(listen_sockfd, 1);
    if(ret < 0)
    {
        perror("listen");
        return -1;
    }

    // 为了防止子进程退出后产生僵尸进程，需要处理子进程发出的SIGCHLD信号
    signal(SIGCHLD,signal_handler);

    while(1)
    {
        int new_sockfd = accept(listen_sockfd, NULL, NULL);
        if(new_sockfd < 0)
        {
            perror("accept");
            continue;
        }

        // 4.接受连接 
        //接收到了新连接，需要创建子进程去处理
        pid_t pid = fork();
        if(pid < 0)
        {
            perror("fork");
            continue;
        }
        else if (pid == 0)
        {
            close(listen_sockfd);
            while(1)
            {
                // 5. 接收数据
                char buf[1024] = { 0 };
                ret = recv(new_sockfd, buf, strlen(buf) - 1, 0);
                if(ret < 0)
                {
                    perror("recv");
                    continue;
                }
                else if(ret == 0)
                {
                    // 若客户端已经关闭连接，则需要关闭用于数据收发的套接字
                    printf(" client shutdown !\n");
                    close(new_sockfd);
                    break;
                }
                cout << "client say:" << buf << endl;

                // 6. 发送数据
                string s;
                cout << "you want to say:";
                getline(cin,s);
                ret = send(new_sockfd,s.c_str(),s.size(),0);
                if(ret < 0)
                {
                    perror("send");
                    continue;
                }
            }
        }
        close(new_sockfd);
    }


    // 7. 关闭监听套接字
    close(listen_sockfd);

    return 0;
}
```



## 6.3 三个终端交互演示

> 服务端

```shell
[gongruiyang@localhost server]$ ./pro_svr 
client say:I'm Client1
you want to say:Hello Client1
client say:I'm Client2
you want to say:Hello Client2
^C
```

> 客户端1

```shell
[gongruiyang@localhost client]$ ./client 
You want to say:I'm Client1
server say:Hello Client1
You want to say:1
 server shutdown!
```

> 客户端2

```shell
[gongruiyang@localhost client]$ ./client 
You want to say:I'm Client2
server say:Hello Client2
You want to say:1
 server shutdown!
```

# 7. 多线程：TCP协议下 多客户端与单服务端 socket通信

## 7.1 客户端代码

```cpp
#include <stdio.h>                                                                  
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;

int main()
{
    // 1. 创建套接字
    int sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(sockfd < 0)
    {
        perror("socket");
        return -1;
    }

    // 2. 发起连接
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.153.128");
    addr.sin_port = htons(19999);
    int ret = connect(sockfd,(struct sockaddr*)&addr,sizeof(addr));
    if(ret < 0)
    {
        perror("connect");
        return -1;
    }

    while(1)
    {
        // 3. 发送数据
        string s;
        cout<<"You want to say:";
        fflush(stdout);
        getline(cin,s);
        ret = send(sockfd, s.c_str(),s.size(),0);
        if(ret < 0)
        {
            perror("send");
            continue;
        }

        // 4. 接受数据
        char buf[1024] = {0};
        ret = recv(sockfd, buf,strlen(buf)-1,0);
        if(ret < 0)
        {
            perror("recv");
            continue;
        }
        else if(ret == 0)
        {
            printf(" server shutdown!\n");
            break;
        }
        cout << "server say:" << buf << endl;
    }


    // 5. 关闭套接字
    close(sockfd);

    return 0;
}
```



## 7.2 服务端代码

```cpp
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <iostream>
using namespace std;                                                                                             

#include <pthread.h>

struct SockVal
{
    int new_sockfd;
};

void* work_task(void* arg)
{
    pthread_detach(pthread_self());
    SockVal* sv = (SockVal*)arg;
    int new_sockfd = sv->new_sockfd;

    while(1)
    {
        // 5. 接收数据
        char buf[1024] = { 0 };
        int ret = recv(new_sockfd, buf, strlen(buf) - 1, 0);
        if(ret < 0)
        {
            perror("recv");
            continue;
        }
        else if(ret == 0)
        {
            // 若客户端已经关闭连接，则需要关闭用于数据收发的套接字
            printf(" client shutdown !\n");
            close(new_sockfd);
            break;
        }
        cout << "client say:" << buf << endl;

        // 6. 发送数据
        string s;
        cout << "you want to say:";
        getline(cin,s);
        ret = send(new_sockfd,s.c_str(),s.size(),0);
        if(ret < 0)
        {
            perror("send");
            continue;
        }
    }
    delete sv;
    return NULL;
}

int main()
{
    // 1. 创建套接字
    //创建一个使用流式套接字+IPv4的套接字
    int listen_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listen_sockfd < 0)
    {
        perror("socket");
        return -1;
    }

    // 2. 绑定地址信息  
    char ip_str[] = "192.168.153.128";
    uint32_t ip = inet_addr(ip_str);
    uint16_t port = htons(19999);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ip;
    addr.sin_port = port;
    int ret = bind(listen_sockfd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret < 0)
    {
        perror("bind");
        return -1;
    }

    // 3. 监听：告诉内核已经准备好可以接受连接啦
    ret = listen(listen_sockfd, 1);
    if(ret < 0)
    {
        perror("listen");
        return -1;
    }

    while(1)
    {
        // 4.接受连接 
        int new_sockfd = accept(listen_sockfd, NULL, NULL);
        if(new_sockfd < 0)
        {
            perror("accept");
            continue;
        }
        // 使用多线程处理 接受的连接    
        SockVal* sv = new SockVal();
        sv->new_sockfd = new_sockfd;
        pthread_t ptid;
        int ret = pthread_create(&ptid,NULL,work_task,(void*)sv);
        if(ret < 0)
        {
            perror("pthread_create");
            continue;
        }
    }

    // 7. 关闭监听套接字
    close(listen_sockfd);

    return 0;
}
```



## 7.3 三个终端交互演示

> Server

```shell
[gongruiyang@localhost server]$ ./server 
client say:Hi! I'm ClientA!
you want to say:Hi! ClientA! I'm Server!
client say:Hi! I'm ClientB!
you want to say:Hi! ClientB! I'm Server!
client say:Bye~~~
you want to say:Bye~~
 client shutdown !
client say:Bye~~
you want to say:Bye~~
 client shutdown !
^C
```

> ClientA

```shell
[gongruiyang@localhost client]$ ./client 
You want to say:Hi! I'm ClientA!
server say:Hi! ClientA! I'm Server!
You want to say:Bye~~~
server say:Bye~~
You want to say:^C
```

> ClientB

```shell
[gongruiyang@localhost client]$ ./client 
You want to say:Hi! I'm ClientB!
server say:Hi! ClientB! I'm Server!
You want to say:Bye~~
server say:Bye~~
You want to say:^C
```

