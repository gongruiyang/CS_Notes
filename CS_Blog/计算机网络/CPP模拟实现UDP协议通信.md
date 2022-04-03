

# 1. UDP的编程流程图

![image-20210114131824097](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210114131824097.png)



# 2. 数据收发阶段使用的API

```c
#include <sys/types.h>
#include <sys/socket.h>
ssize_t sendto(int sockfd, void *buf, size_t len, int flags, struct sockaddr *dest_addr, socklen_t addrlen);
ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);
```

## 2.1 sendto接口

```c
ssize_t sendto(int sockfd, void *buf, size_t len, int flags, struct sockaddr *dest_addr, socklen_t addrlen);
```

* sockfd：套接字描述符
* buf：待发送数据
* len：待发送数据长度
* flags：标志位，填入0表示阻塞发送

* dest_addr：消息接收方的地址信息结构体
* addrlen：消息接收方的地址信息结构体大小

## 2.2 recvfrom接口

```c
ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);
```

* sockfd：套接字描述符
* buf：数据接受存放的缓冲区
* len：最大接受数据的能力（strlen(buf)-1）
* flag：标志位，填入0表示阻塞接收
* src_addr：输出型参数，消息发送方的地址信息结构，recvfrom函数会填写该结构体字段信息
* addrlen：输出型参数，消息发送方的地址信息长度，recvfrom函数会填写该信息长度的地址

# 3. CPP语言 模拟实现 UDP协议 下 客户端 与 服务端 的 Socke通信

## 3.1 服务端代码

```c
#include <stdio.h>                                                                                      
#include <iostream>
#include <string.h>
using namespace std;

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//创建
int get_socket()
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sockfd < 0)
    {
        perror("socket");
        return -1;
    }
    return sockfd;
}

bool bind_addr(int sockfd, char* ip_str, uint16_t port)
{
    uint32_t ip = inet_addr(ip_str);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = port;
    addr.sin_addr.s_addr = ip;
    int ret = bind(sockfd,(sockaddr*)&addr,sizeof(addr));
    if(ret < 0)
    {
        perror("bind");
        return false;
    }
    return true;
}
void interact_client(int sockfd)
{
    // 1.等待客户端发送数据
    char buf[1024] = { 0 };//用于存放接收的信息的缓冲区
    struct sockaddr_in src_addr;  //出参,地址信息结构体
    socklen_t addr_len = sizeof(src_addr);//地址信息长度
    ssize_t recv_size = recvfrom(sockfd, buf, strlen(buf) - 1, 0, (struct sockaddr*)&src_addr, &addr_len);
    if(recv_size < 0)
    {
        perror("recvfrom");
        return ;
    }
    cout << "cli say:" << buf << endl;

    // 2.给客户端发送数据
    cout << "I wan to say:";
    std::string s;
    cin >> s;
    //执行到这里，就代表已经获得了客户端的 地址信息结构体
    ssize_t send_size = sendto(sockfd, s.c_str(), s.size(), 0, (struct sockaddr*)&src_addr, addr_len);
    if(send_size < 0)
    {
        perror("sendto");
        return ;
    }
}

int main()
{
    int sockfd = get_socket();
    uint16_t port = 19999;
    char *ip_str = "192.168.153.128";

    if(!bind_addr(sockfd, ip_str, port))  
    {
        return -1;
    }

    while(1)
    {
        interact_client(sockfd);
    }

    return 0;
}                                                                                                         
```

## 3.2 客户端代码

```c
#include <stdio.h>                                                                                       
#include <iostream>
#include <string.h>
#include <string>
using namespace std;

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int get_socket()
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sockfd < 0)
    {
        perror("socket");
        return -1;
    }
    return sockfd;
}

void interact_svr(int sockfd, struct sockaddr_in* svr_addr )
{
    // 1.给服务端发送数据
    cout << "I want to say:";
    string s;
    cin >> s;
    ssize_t send_size = sendto(sockfd, s.c_str(), s.size(), 0, (struct sockaddr*)svr_addr, sizeof(struct sockaddr_in));
    if(send_size < 0)
    {
        perror("sendto");
        return ;
    }
    
    // 2.从服务端接收数据
    char buf[1024] = { 0 };//用于存放接收的信息的缓冲区
    //已知对端信息所以可以不接收
    ssize_t recv_size = recvfrom(sockfd, buf, strlen(buf) - 1, 0, NULL, NULL);
    if(recv_size < 0)
    {
        perror("recvfrom");
        return ;
    }
    cout << "svr say:" << buf << endl;
}

int main()
{
    int sockfd = get_socket();
    uint16_t port = 19999;
    char *ip_str = "192.168.153.128";

    struct sockaddr_in svr_addr;
    svr_addr.sin_family = AF_INET;
    svr_addr.sin_port = port;
    svr_addr.sin_addr.s_addr = inet_addr(ip_str);

    while(1)
    {
        interact_svr(sockfd,&svr_addr);
    }
	
    return 0;
}                                                  
```



## 3.3 交互演示

```c
[gongruiyang@localhost client]$ ./cli 
I want to say:XXXX
svr say:AAAA
I want to say:66666
svr say:7777
I want to say:^C
    
[gongruiyang@localhost server]$ ./svr 
cli say:XXXX
I wan to say:AAAA
cli say:66666
I wan to say:7777
^C
```



# 4. 寻找 UDP下 客户端与服务端通信 共性

![image-20210114154203832](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210114154203832.png)

* 由图可以看出，这两个端口中有许许多多的接口都是重复的
* 为了降低代码重复率，我们可以将这些方法封装在一起



# 5. 封装后的UDP协议下socket编程接口

> udp.cpp

```c
#pragma once

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>
#include <string>

class UdpSvr
{
private:
    int sockfd_;

public:
    UdpSvr()
    {
        sockfd_ = -1;
    }
    ~UdpSvr()
    {

    }

    int CreatSocket()
    {
        sockfd_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if(sockfd_ < 0)
        {
            perror("socket");
            return -1;
        }
        return 0;
    }

    int Bind(std::string ip, uint16_t port)
    {
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
        int ret = bind(sockfd_, (struct sockaddr*)&addr, sizeof(addr));
        if(ret < 0)
        {
            perror("bind");
            return -1;
        }

        return 0;
    }

    int Sendto(std::string data, struct sockaddr_in* dest_addr)
    {
        ssize_t send_size = sendto(sockfd_, data.c_str(), data.size(), 0 ,(struct sockaddr*)dest_addr, sizeof(struct sockaddr_in));
        if(send_size < 0)
        {
            perror("sendto");
            return -1;
        }
        return send_size;
    }

    int Recvfrom(std::string* data, struct sockaddr_in* peer_addr)
    {
        char buf[1024] = {0};
        socklen_t peer_addr_len = sizeof(struct sockaddr_in);
        ssize_t recv_size = recvfrom(sockfd_, buf, sizeof(buf) - 1, 0, (struct sockaddr*)peer_addr, &peer_addr_len);
        if(recv_size < 0)
        {
            perror("recvfrom");
            return -1;
        }

        data->assign(buf, strlen(buf));
        return recv_size;
    }

    void Close()
    {
        close(sockfd_);
    }
};
```

> client端口代码

```c
#include "../udp.hpp"

#define CHECK_RET(p) if(p < 0){return -1;}

int main(int argc, char* argv[])
{
    // ./cli [ip] [port]
    if(argc != 3)
    {
        printf("using ./cli [ip] [port]\n");
        return -1;
    }

    UdpSvr us;
    CHECK_RET(us.CreatSocket());

    std::string ip = argv[1]; 
    uint16_t port = atoi(argv[2]);

    struct sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    dest_addr.sin_addr.s_addr = inet_addr(ip.c_str());

    while(1)
    {
        std::string data;
        std::cout << "please enter msg to svr:";
        fflush(stdout);
        std::cin >> data;

        int ret = us.Sendto(data, &dest_addr);
        if(ret < 0)
        {
            continue;
        }

        data.clear();

        struct sockaddr_in peer_addr;
        ret = us.Recvfrom(&data, &peer_addr);
        if(ret < 0)
        {
            continue;
        }

        std::cout << "svr say: " << data << std::endl;
    }

    us.Close();

    return 0;
}
```

> server端口代码

```c
#include "../udp.hpp"

#define CHECK_RET(p) if(p < 0){return -1;}

int main(int argc, char* argv[])
{
    // ./svr [ip] [port]
    if(argc != 3)
    {
        printf("using ./svr [ip] [port]\n");
        return -1;
    }

    UdpSvr us;
    CHECK_RET(us.CreatSocket());

    std::string ip = argv[1]; 
    uint16_t port = atoi(argv[2]);

    CHECK_RET(us.Bind(ip, port));

    while(1)
    {
        std::string data;
        struct sockaddr_in peer_addr;
        int ret = us.Recvfrom(&data, &peer_addr);
        if(ret < 0)
        {
            continue;
        }

        std::cout << "cli say: " << data << std::endl;

        data.clear();
        std::cout << "please enter msg to client: ";
        fflush(stdout);
        std::cin >> data;

        ret = us.Sendto(data, &peer_addr);
        if(ret < 0)
        {
            continue;
        }
    }

    us.Close();

    return 0;
}
```

