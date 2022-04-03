# 1. socket常见API

```c
#include <sys/types.h>         
#include <sys/socket.h>
int socket(int domain, int type, int protocol);
int bind(int socket, const struct sockaddr *address, socklen_t address_len);
int listen(int socket, int backlog);
int accept(int socket, struct sockaddr* address, socklen_t* address_len);
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

（1）socket接口

> 功能：常见套接字

```c
int socket(int domain, int type, int protocol);
```

* domain：地址域，指定网络上使用什么协议

| 填入domain的宏 | 含义                   |
| -------------- | ---------------------- |
| AF_INET        | IPv4协议               |
| AF_INET6       | IPv6协议               |
| AF_UNIX        | 本机上面的进程通信协议 |

* type：套接字类型

| 填入type的宏 | 含义                       |
| ------------ | -------------------------- |
| SOCK_DGRAM   | 用户数据报套接字（数据报） |
| SOCK_STREAM  | 流式套接字（字节流）       |

* protocol：协议

如果type中填入的是SOCK_DGRAM

| 填入protocol的宏 | 含义                                  |
| ---------------- | ------------------------------------- |
| 0                | 表示采用SOCK_DGRAM默认的协议：UDP协议 |
| IPPROTO_UDP      | 表示指定使用UDP协议                   |

如果type中填入的是SOCK_STREAM

| 填入protocol的宏 | 含义                                   |
| ---------------- | -------------------------------------- |
| 0                | 表示采用SOCK_STREAM默认的协议：TCP协议 |
| IPPROTO_TCP      | 表示指定使用TCP协议                    |

* 返回值：`套接字操作句柄`

> 套接字操作句柄本质上是一个`文件描述符`

```c
// 创建TCP套接字
int TCP_soc_id = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)
// 创建UDP套接字
int UDP_soc_id = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)
```

（2）bind接口

> 功能：绑定地址信息

```c
int bind(int socket, const struct sockaddr *address, socklen_t address_len);
```

* socket：套接字描述符（socket接口返回值）
* address：当使用IPv4版本的ip协议时，需要传入sockaddr_in结构体并强转位为sockaddr结构体
* address_len：绑定的地址信息长度

> bind函数不需要针对特定的协议，准备不同类型的参数，只需要你准备一个通用的地址结构sockaddr即可

（3）listen接口（TCP编程使用）

> 监听是否有客户端要来连接本客户端

```c
int listen(int socket, int backlog);
```

* socket：侦听套接字描述符（socket接口返回值）
* backlog：`已完成连接队列`的大小

（4）accept接口（TCP编程使用）

> 从`已完成连接队列`中**获取新连接**，若已完成连接队列为`空`，调用改接口，会导致`阻塞`该执行流

```c
int accept(int socket, struct sockaddr* address, socklen_t* address_len);
```

* socket：侦听套接字描述符（socket接口返回值）
* address：对端的地址信息（客户端地址信息）
* address_len：对端的地址信息长度（客户端地址信息长度）

返回值：正常情况下返回的是**为新连接创建出来的套接字描述符**

（5）connect接口（TCP编程使用）

> 客户端发起连接的接口

```c
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

* sockfd：客户端创建出来的套接字描述符
* addr：要连接的服务端的地址信息
* addrlen：要连接的服务端的地址信息长度

# 2. IPv4协议的地址信息结构：sockaddr_in结构体

`sockaddr_in结构体源码`

```c
#define __SOCKADDR_COMMON_SIZE  (sizeof (unsigned short int))	//解释__SOCKADDR_COMMON_SIZE
typedef uint16_t in_port_t;				//解释in_port_t
typedef unsigned short int sa_family_t;	//解释sa_family_t
#define __SOCKADDR_COMMON(sa_prefix) \	//解释__SOCKADDR_COMMON
  sa_family_t sa_prefix##family   
/* Structure describing an Internet socket address.  */
struct sockaddr_in
{
    __SOCKADDR_COMMON (sin_);	
    in_port_t sin_port;     	/* Port number.  */
    struct in_addr sin_addr;    /* Internet address.  */

    /* Pad to size of `struct sockaddr'.  */
    unsigned char sin_zero[sizeof (struct sockaddr) -
                           __SOCKADDR_COMMON_SIZE -
                           sizeof (in_port_t) -                           
                           sizeof (struct in_addr)];
};

```

> 要想看懂该源码需要看懂其中嵌套的结构体的源码

## 2.1 sockaddr结构体

```c
#define __SOCKADDR_COMMON(sa_prefix) \	//解释__SOCKADDR_COMMON是一个sa_family_t
  sa_family_t sa_prefix##family   		
typedef unsigned short int sa_family_t;	//解释sa_family_t
struct sockaddr
{
    __SOCKADDR_COMMON (sa_);  //这里的__SOCKADDR_COMMON就是sa_family_t，也就是一个unsigned short int类型
    char sa_data[14];   /* Address data.  */
};        
```

* 所以一个sockaddr结构体占用16个字节（unsigned short int占2字节，char数组占14字节）
* sockaddr结构体是一个通用的地址信息结构

## 2.2 in_addr结构体

```c
/* Internet address.  */
typedef uint32_t in_addr_t;
struct in_addr
{
    in_addr_t s_addr;
};
```

* 所以一个in_addr结构体占用4字节



将所有宏定义翻译之后的sockaddr_in结构体源码

```c
struct sockaddr_in
{
    unsigned short int sin_;	/*  地址域  */		//2字节
    uint16_t sin_port;     		/*  端口号  */		//2字节
    uint32_t sin_addr;    		/*  IP地址  */		 //4字节

    /* 填充.  */
    unsigned char sin_zero[16 - 2 - 2 - 4];			//8字节
};
```

* 所以sockaddr_in结构体占用16字节

## 2.3 sockaddr_in结构体图解

![image-20210113185107152](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210113185107152.png)



# 3. socket接口中参数domain的所有宏定义源码

```c
/* Address families.  */
#define AF_UNSPEC PF_UNSPEC
#define AF_LOCAL  PF_LOCAL
#define AF_UNIX   PF_UNIX	//本机进程间通信
#define AF_FILE   PF_FILE
#define AF_INET   PF_INET	//IPv4
#define AF_AX25   PF_AX25
#define AF_IPX    PF_IPX
#define AF_APPLETALK  PF_APPLETALK
#define AF_NETROM PF_NETROM
#define AF_BRIDGE PF_BRIDGE
#define AF_ATMPVC PF_ATMPVC
#define AF_X25    PF_X25
#define AF_INET6  PF_INET6	//IPv6
#define AF_ROSE   PF_ROSE
#define AF_DECnet PF_DECnet
#define AF_NETBEUI  PF_NETBEUI
#define AF_SECURITY PF_SECURITY
#define AF_KEY    PF_KEY
#define AF_NETLINK  PF_NETLINK
#define AF_ROUTE  PF_ROUTE
#define AF_PACKET PF_PACKET
#define AF_ASH    PF_ASH
#define AF_ECONET PF_ECONET            
#define AF_ATMSVC PF_ATMSVC
#define AF_RDS    PF_RDS
#define AF_SNA    PF_SNA
#define AF_IRDA   PF_IRDA
#define AF_PPPOX  PF_PPPOX
#define AF_WANPIPE  PF_WANPIPE
#define AF_LLC    PF_LLC
#define AF_IB   PF_IB
#define AF_MPLS   PF_MPLS
#define AF_CAN    PF_CAN
#define AF_TIPC   PF_TIPC
#define AF_BLUETOOTH  PF_BLUETOOTH
#define AF_IUCV   PF_IUCV
#define AF_RXRPC  PF_RXRPC
#define AF_ISDN   PF_ISDN
#define AF_PHONET PF_PHONET
#define AF_IEEE802154 PF_IEEE802154
#define AF_CAIF   PF_CAIF
#define AF_ALG    PF_ALG
#define AF_NFC    PF_NFC
#define AF_VSOCK  PF_VSOCK
#define AF_MAX    PF_MAX

/* Protocol families.  */
#define PF_UNSPEC 0 /* Unspecified.  */
#define PF_LOCAL  1 /* Local to host (pipes and file-domain).  */
#define PF_UNIX   PF_LOCAL /* POSIX name for PF_LOCAL.  */
#define PF_FILE   PF_LOCAL /* Another non-standard name for PF_LOCAL.  */
#define PF_INET   2 /* IP protocol family.  */
#define PF_AX25   3 /* Amateur Radio AX.25.  */
#define PF_IPX    4 /* Novell Internet Protocol.  */
#define PF_APPLETALK  5 /* Appletalk DDP.  */
#define PF_NETROM 6 /* Amateur radio NetROM.  */
#define PF_BRIDGE 7 /* Multiprotocol bridge.  */
#define PF_ATMPVC 8 /* ATM PVCs.  */
#define PF_X25    9 /* Reserved for X.25 project.  */
#define PF_INET6  10  /* IP version 6.  */
#define PF_ROSE   11  /* Amateur Radio X.25 PLP.  */
#define PF_DECnet 12  /* Reserved for DECnet project.  */
#define PF_NETBEUI  13  /* Reserved for 802.2LLC project.  */
#define PF_SECURITY 14  /* Security callback pseudo AF.  */
#define PF_KEY    15  /* PF_KEY key management API.  */
#define PF_NETLINK  16
#define PF_ROUTE  PF_NETLINK /* Alias to emulate 4.4BSD.  */
#define PF_PACKET 17  /* Packet family.  */
#define PF_ASH    18  /* Ash.  */                                            
#define PF_ECONET 19  /* Acorn Econet.  */
#define PF_ATMSVC 20  /* ATM SVCs.  */
#define PF_RDS    21  /* RDS sockets.  */
#define PF_SNA    22  /* Linux SNA Project */
#define PF_IRDA   23  /* IRDA sockets.  */
#define PF_PPPOX  24  /* PPPoX sockets.  */
#define PF_WANPIPE  25  /* Wanpipe API sockets.  */
#define PF_LLC    26  /* Linux LLC.  */
#define PF_IB   27      /* Native InfiniBand address.  */
#define PF_MPLS   28      /* MPLS.  */
#define PF_CAN    29  /* Controller Area Network.  */
#define PF_TIPC   30  /* TIPC sockets.  */
#define PF_BLUETOOTH  31  /* Bluetooth sockets.  */
#define PF_IUCV   32  /* IUCV sockets.  */
#define PF_RXRPC  33  /* RxRPC sockets.  */
#define PF_ISDN   34  /* mISDN sockets.  */
#define PF_PHONET 35  /* Phonet sockets.  */
#define PF_IEEE802154 36  /* IEEE 802.15.4 sockets.  */
#define PF_CAIF   37  /* CAIF sockets.  */
#define PF_ALG    38  /* Algorithm sockets.  */
#define PF_NFC    39  /* NFC sockets.  */
#define PF_VSOCK  40  /* vSockets.  */
#define PF_MAX    41  /* For now..  */
```

# 4. socket接口中参数protocol的所有宏定义源码

```c

/* Standard well-defined IP protocols.  */
enum
  {
    IPPROTO_IP = 0,    /* Dummy protocol for TCP.  */
#define IPPROTO_IP    IPPROTO_IP
    IPPROTO_ICMP = 1,    /* Internet Control Message Protocol.  */
#define IPPROTO_ICMP    IPPROTO_ICMP
    IPPROTO_IGMP = 2,    /* Internet Group Management Protocol. */
#define IPPROTO_IGMP    IPPROTO_IGMP                                            
    IPPROTO_IPIP = 4,    /* IPIP tunnels (older KA9Q tunnels use 94).  */
#define IPPROTO_IPIP    IPPROTO_IPIP
    IPPROTO_TCP = 6,     /* Transmission Control Protocol.  */
#define IPPROTO_TCP   IPPROTO_TCP
    IPPROTO_EGP = 8,     /* Exterior Gateway Protocol.  */
#define IPPROTO_EGP   IPPROTO_EGP
    IPPROTO_PUP = 12,    /* PUP protocol.  */
#define IPPROTO_PUP   IPPROTO_PUP
    IPPROTO_UDP = 17,    /* User Datagram Protocol.  */
#define IPPROTO_UDP   IPPROTO_UDP
    IPPROTO_IDP = 22,    /* XNS IDP protocol.  */
#define IPPROTO_IDP   IPPROTO_IDP
    IPPROTO_TP = 29,     /* SO Transport Protocol Class 4.  */
#define IPPROTO_TP    IPPROTO_TP
    IPPROTO_DCCP = 33,     /* Datagram Congestion Control Protocol.  */
#define IPPROTO_DCCP    IPPROTO_DCCP
    IPPROTO_IPV6 = 41,     /* IPv6 header.  */
#define IPPROTO_IPV6    IPPROTO_IPV6
    IPPROTO_RSVP = 46,     /* Reservation Protocol.  */
#define IPPROTO_RSVP    IPPROTO_RSVP
    IPPROTO_GRE = 47,    /* General Routing Encapsulation.  */
#define IPPROTO_GRE   IPPROTO_GRE
    IPPROTO_ESP = 50,      /* encapsulating security payload.  */
#define IPPROTO_ESP   IPPROTO_ESP
    IPPROTO_AH = 51,       /* authentication header.  */
#define IPPROTO_AH    IPPROTO_AH
    IPPROTO_MTP = 92,    /* Multicast Transport Protocol.  */
#define IPPROTO_MTP   IPPROTO_MTP
    IPPROTO_BEETPH = 94,   /* IP option pseudo header for BEET.  */
#define IPPROTO_BEETPH    IPPROTO_BEETPH
    IPPROTO_ENCAP = 98,    /* Encapsulation Header.  */
#define IPPROTO_ENCAP   IPPROTO_ENCAP
    IPPROTO_PIM = 103,     /* Protocol Independent Multicast.  */
#define IPPROTO_PIM   IPPROTO_PIM
    IPPROTO_COMP = 108,    /* Compression Header Protocol.  */
#define IPPROTO_COMP    IPPROTO_COMP
    IPPROTO_SCTP = 132,    /* Stream Control Transmission Protocol.  */
#define IPPROTO_SCTP    IPPROTO_SCTP
    IPPROTO_UDPLITE = 136, /* UDP-Lite protocol.  */
#define IPPROTO_UDPLITE   IPPROTO_UDPLITE
    IPPROTO_MPLS = 137,    /* MPLS in IP.  */
#define IPPROTO_MPLS           IPPROTO_MPLS
    IPPROTO_RAW = 255,     /* Raw IP packets.  */
#define IPPROTO_RAW   IPPROTO_RAW
    IPPROTO_MAX
  };
```



# 5. sockaddr可接收的结构体

![image-20210113193826378](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210113193826378.png)



# 6. 模拟实现socket通信

``` c
 #include <stdio.h>
 #include <unistd.h>                                                           
 
 #include <sys/socket.h>
 #include <netinet/in.h>
 #include <arpa/inet.h>
 
int main()
{
    // 1. 创建套接字 : 使用 IPv4 + UDP协议
    int sockfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if(sockfd < 0)
    {
        perror("socket");
        return -1;
    }

    // 2. 绑定地址信息
    // inet_addr函数作用：
    //      (1).将点分十进制的ip地址字符串转化为unit32_t，
    //      (2).将unit32_t从主机字节序转化为网络字节序
    char *ip_str = "192.168.153.128";
    uint32_t ip = inet_addr(ip_str);
    uint16_t port = htons(19999);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ip; 	//IP
    addr.sin_port =  port;		//port

    int ret = bind(sockfd,(struct sockaddr*)&addr,sizeof(struct sockaddr_in));
    if(ret < 0)
    {
        perror("bind");
        return -1;
    }

    while(1)
    {
        sleep(1);
    }
    return 0;
}
```

## 6.1 查看socket通信创建的文件描述符

* 使用命令查看某进程的文件描述符：`ls -l /proc/[pid]/fd`

```shell
[gongruiyang@localhost server]$ ps aux | grep ./test
gongrui+   8473  0.0  0.0   4212   352 pts/1    S+   15:14   0:00 ./test
gongrui+   8476  0.0  0.0 112828   996 pts/0    R+   15:14   0:00 grep --color=auto ./test
[gongruiyang@localhost server]$ ll /proc/8473/fd
总用量 0
lrwx------. 1 gongruiyang gongruiyang 64 1月  14 15:15 0 -> /dev/pts/1
lrwx------. 1 gongruiyang gongruiyang 64 1月  14 15:15 1 -> /dev/pts/1
lrwx------. 1 gongruiyang gongruiyang 64 1月  14 15:14 2 -> /dev/pts/1
lrwx------. 1 gongruiyang gongruiyang 64 1月  14 15:15 3 -> socket:[72072]
```

## 6.2 查看socket通信侦听的端口号

* 使用netstat -anp命令查看端口号侦听情况

```shell
[gongruiyang@localhost server]$ netstat -anp | grep 19999
(Not all processes could be identified, non-owned process info
 will not be shown, you would have to be root to see it all.)
udp        0      0 192.168.153.128:19999   0.0.0.0:*                           8473/./test
```

> * `udp` ： 代表该19999端口是被u**dp协议**下的进程侦听的
> * `192.168.153.128:19999`：当前**服务端**侦听的ip和端口
> * `0.0.0.0:* `：代表能够接收客户端**任意ip地址**和**任意端口**
> * `8473/./test`：进程号 和 进程名