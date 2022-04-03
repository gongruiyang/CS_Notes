# 1. HTTP的概念

* http协议是`应用层`协议，在`传输层`使用的协议是`TCP协议`

> * 超文本传输协议（英文：**H**yper**T**ext **T**ransfer **P**rotocol，缩写：HTTP）是一种用于分布式、协作式和超媒体信息系统的应用层协议。HTTP是万维网的数据通信的基础。
> * HTTP的发展是由蒂姆·伯纳斯-李于1989年在欧洲核子研究组织（CERN）所发起。HTTP的标准制定由万维网协会（World Wide Web Consortium，W3C）和互联网工程任务组（Internet Engineering Task Force，IETF）进行协调，最终发布了一系列的RFC，其中最著名的是1999年6月公布的 RFC 2616，定义了HTTP协议中现今广泛使用的一个版本——`HTTP 1.1`
> * 2014年12月，互联网工程任务组（IETF）的Hypertext Transfer Protocol Bis（httpbis）工作小组将HTTP/2标准提议递交至IESG进行讨论，于2015年2月17日被批准。 `HTTP/2标准`于2015年5月以RFC 7540正式发表，取代HTTP 1.1成为HTTP的实现标准。

# 2. HTTP请求的解释

![image-20210119102750682](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210119102750682.png)

* http：使用的协议名
* user:pass：用户名和密码
* www.example.jp：域名->IP地址
* 80：服务器端口号
* /dir/index.html：带层次的文件路径
  * / ：http服务器的`逻辑根目录`（人为规定的某一个文件夹是根目录，并不是绝对的）
* uid=1：`key=value`形式的查询字符串，不同的字符串之间是使用`&符号`进行链接的

# 3. urlencode与urldecode

> 我们在搜索框中输入"C++"，并按下回车键后，会看到如下的现象:其中要查询的“C++”变成了C%2B%2B

![image-20210119103947534](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210119103947534.png)

* urlencode：用于编码URL字符串，数字和字母保持不变，空格变为'+'，其他（如：中文字符）先转换为十六进制表示，然后在每个字节前面加一个标识符%，例如：“啊”字 Ascii的十六进制是0xB0A1——>%B0%A1
* urldecode：用于解码已编码的URL 字符串，解码方式与URLEncode相反

# 4. http数据包格式

## 4.1 http请求

![image-20210119112148780](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210119112148780.png)

> Host：请求的主机名称
>
> Connection：keep-alive表示长连接
>
> User-Agent：浏览器的版本信息和操作系统的信息
>
> Content-Type：正文类型
>
> Accept-Encoding：能够接收的编码格式
>
> Accept-Language：能够接收的语言

* **第一行**：GET：方法 ；HTTP/1.1：版本号
* **Header**：`请求的属性`，属性是由冒号`:`分割的键值对，一组属性占一行，每组属性之间使用`\n`分割，遇到`空白行`表示Header部分结束
* **Body**：`空白行`后面的内容都是Body，Body允许是`空字符串`。如果Body存在，则在`Header`中会有一个`Content-Length属性`来标识Body的长度

## 4.2 http响应

![image-20210119112248504](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210119112248504.png)

> Server:BWS/1.1 表示目标Web服务器程序的名字是BWS（Baidu Web Server）
>
> Content - Type：text/html;charset=utf-8表示目标文档是MIME类型，其中text是主文档类型，HTML是子文档类型，charset用于指定文档的字符编码

* 第一行：HTTP/1.1是HTTP的版本号；200是状态码；OK是状态码解释（状态信息）
* **Header**：`响应的属性`，属性是由冒号`:`分割的键值对，一组属性占一行，每组属性之间使用`\n`分割，遇到`空白行`表示Header部分结束
* **Body**：`空白行`后面的内容都是Body，Body允许是`空字符串`。如果Body存在，则在`Header`中会有一个`Content-Length属性`来标识Body的长度

# 5. http状态码和状态信息及其含义

![image-20210119114534615](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210119114534615.png)

# 6. http请求方法

![image-20210119114739665](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210119114739665.png)

* HEAD、GET、OPTIONS、TRACE方法是安全方法：只是从服务器获取资源或信息，而不对服务器资源进行任何修改
* POST、PUT、DELETE、PATCH方法是不安全方法：会影响服务器上的资源

* HEAD、GET、OPTIONS、TRACE、PUT、DELETE方法是等幂的：多次重复的请求和只发送一次具有完全相同的效果



# 7. 模拟实现http协议服务器

```cpp
#include <stdio.h>                                                                                         
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

// 网络编程所需头文件
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// 提醒启动http服务端的格式
void Tips()
{
    printf("Format: ./http_server [ip] [port]\n");
}

int main(int argc, char* argv[])
{
    // 限定启动的格式
    if(argc != 3)
    {
        Tips();
        return 1;
    }

    // 创建服务端套接字，使用TCP协议
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd < 0)
    {
        perror("socket");
        return 1;
    }

    // 使用使用输入的端口号和IP地址初始化地址信息结构体
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));

    // 服务端绑定地址信息结构体
    int ret = bind(sock_fd,(struct sockaddr*)&addr, sizeof(addr));
    if(ret < 0)
    {
        perror("bind");
        return 1;
    }

    // 等待客户端连接
    ret = listen(sock_fd, 0);
    if(ret < 0)
    {
        perror("listen");
        return -1;
    }

    while(1)
    {
        // 用于存放客户端的地址信息结构
        struct sockaddr_in client_addr;
        socklen_t len;
        // 获取客户端的连接，并填写客户端的地址信息结构体
        int client_fd = accept(sock_fd,(struct sockaddr*)&client_addr,&len);
        if(client_fd < 0)
        {
            perror("accept");
            continue;
        }

        char input_buf[1024 * 10] = {0};  // 用足够大的缓冲区将数据读完
        ssize_t read_size = read(client_fd, input_buf, sizeof(input_buf) - 1);
        if(read_size < 0)
        {
            return 1;
        }
        printf("[Request]:%s",input_buf);

        char buf[1024] = {0}; 
        const char* hello = "<h1>hello world!</h1>";
        sprintf(buf, "HTTP/1.0 200 OK\nContent-Length:%lu\n\n%s", strlen(hello), hello);
        write(client_fd, buf, strlen(buf));
    }
    return 0;
}
```

* 打开网页输入`ip:port`进入后会有以下现象



![image-20210119142625790](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210119142625790.png)

```shell
[gongruiyang@localhost httpProgram]$ ./server 192.168.153.128 9090
[Request]:GET / HTTP/1.1
Host: 192.168.153.128:9090
Connection: keep-alive
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4315.4 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
Accept-Encoding: gzip, deflate
Accept-Language: zh-CN,zh;q=0.9

[Request]:GET /favicon.ico HTTP/1.1
Host: 192.168.153.128:9090
Connection: keep-alive
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4315.4 Safari/537.36
Accept: image/avif,image/webp,image/apng,image/svg+xml,image/*,*/*;q=0.8
Referer: http://192.168.153.128:9090/
Accept-Encoding: gzip, deflate
Accept-Language: zh-CN,zh;q=0.9

^C
```



# 8. 浏览器地址栏键入URL所需经历的流程

当你在浏览器的地址栏中`输入某个URL`直至`显示出来网页内容`需要经历以下的流程：

1. `浏览器`向`DNS服务器`发出请求，请求**解析**该URL中的**域名**对应的**IP地址**
2. 解析出IP地址后，通过**IP地址**和**默认http端口号80**与服务器**建立TCP连接**
3. `浏览器`发出**读取文件的HTTP请求**，该**请求报文**作为**TCP三次握手**的**第三个报文的部分数据**发送给`服务器`
4. `服务器`对`浏览器`做出响应，并把对应的**HTML文本**发送给`浏览器`
5. **释放TCP连接**
6. `浏览器`**解析HTML文件**并**显示内容**

![image-20210119150816236](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210119150816236.png)