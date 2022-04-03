# 一、DNS协议介绍

域名系统（DNS）是一种用于TCP/IP应用程序的`分布式数据库`，该数据库提供`主机名字 - IP地址`之间的映射关系

* 分布式含义：单个站点不能拥有所有的信息，每个站点保留它自己的信息数据库，并运行一个服务器程序以提供 Internet 上的其他系统查询

对DNS的访问是通过一个`地址解析器（resolver）`来完成的，在Unix主机中，该解析器主要通过两个库函数来访问的，分别是`gethostbyname`和`gethostbyaddr`

* gethostbyname：通过主机名字来获取IP地址
* gethostbyaddr：通过IP地址来获取主机名字

DNS的解析器是应用程序的一部分，而TCP/IP协议是操作系统的内核的一部分，所以TCP/IP对应DNS的存在是不知晓的

# 二、DNS协议基础

## 1. DNS树状结构层次

DNS的命名空间类似一颗树，树中的每个结点有一个最长63字符长的标识，树根是没有任何标识的特殊结点，命名不区分大小写

![image-20210808135237835](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/image-20210808135237835.png)

顶级域名分为三部分：

1. arpa是一个4字符长的用于`IP地址`到`主机名称`转换的特殊域名
2. 7个3字符长的普通域名（组织域名）
3. 所有2字符长的均是国家域名

普通域及其解释

| 域名 | 描述                           |
| ---- | ------------------------------ |
| com  | 商业组织（commercial）         |
| edu  | 教育机构（education）          |
| gov  | 其他美国政府部门（government） |
| int  | 国际组织（international）      |
| mil  | 美国军事网点（military）       |
| net  | 网络（network）                |
| org  | 其他组织（organization）       |

## 2. DNS的报文格式

DNS定义了一个用于查询和响应的报文格式，如下图所示：

![image-20210808141125559](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/image-20210808141125559.png)

这个报文由12个字节的首部和4个长度可变的字段组成

---

标识字段由`客户程序设置`并由`服务器返回结果`，客户程序通过该字段来确定`响应与查询是否匹配`

标识字段具体组成及解释如下：

![image-20210808141502905](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/image-20210808141502905.png)

标识字段共 16 位（2字节），从左向右依次介绍如下

1. `QR`：占1位，该位为`0`表示是`查询报文`，该位为`1`表示是`响应报文`
2. `opcode`：占4位，`全是0`表示`标准查询`，`全是1`表示`反向查询`，`全是2`表示`服务器状态请求`
3. `AA`：占1位，如果是`1`表示`授权回答`，表示该应答是否来自该域的权威服务器
4. `TC`：占1位，如果是`1`表示`可截断`的，即使用UDP时，它表示当应答总长度超过512字节时，只返回前512字节数据
5. `RD`：占1位，如果是`1`表示`期望递归`，可在查询中设置，并在响应中返回，该位设置为1表示使用递归查询，该位设置为0表示使用迭代查询
6. `RA`：占1位，如果是`1`表示`可用递归`，如果名字服务器支持递归查询，可在响应中将该比特位置为1
7. `zero`：占3位，全部必须`设置为0`，作为保留位
8. `rcode`：占4位，表示返回码字段，值为`0`表示查询的`名字没有差错`，值为`3`表示查询的结果有`名字差错`（域名不存在）

---

对于查询报文，查询问题字段中的条目至少是1条，其他三个可变长度字段均为0

对于应答报文，回答字段中的条目至少是1条，剩下两个可变长度字段均为0或非0

---

DNS查询报文中`查询问题`字段部分图解：

![image-20210808144851199](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/image-20210808144851199.png)



`查询名`：待转换的域名，由一个或多个标识符组成的序列。每个标识符以首字节的计数值来说明随后的标识符的字节长度，每个名字最后字节以0结束，举例如图所示：

![image-20210808145355682](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/image-20210808145355682.png)

`查询类型`：每个问题有一个查询类型，每个响应也有一个查询类型。约有20个不同的类型值，最常用的查询类型为A类型，其数值为1，表示期望获得查询名字的IP地址，即域名到IP之间的转换

`查询类`：通常是1，指互联网地址

---

DNS响应报文中最后三个字段（回答字段、授权字段、附加信息字段）均采用一种称为资源记录RR（Resource Record）的相同格式，如图所示：

![image-20210808151129722](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/image-20210808151129722.png)

`域名`：记录资源数据对应的名字，其格式与`查询名`字段格式相同

`类型`：与前面的`查询类型`字段值一致

`类`：通常为1，表示Internet数据

`生存时间`：客户程序保留该资源记录的秒数，资源记录通常的生存时间为2天

`资源数据长度`：表示资源记录数据的长度，对于类型为A的记录的资源数据是4字节的IP地址

`资源数据`：记录的响应主体内容

# 三、高速缓存

为了减少Internet上DNS的通信量，所有的`名字服务器`均使用高速缓存。

在标准的Unix实现中，高速缓存是由`名字服务器维护`的而`不是名字解析器维护`的。

名字解析器位于应用程序中，并不是总处于工作状态，而高速缓存放在只要系统处于工作状态就能起作用的程序中显得很重要，所以放置在名字服务器中。这样任何一个使用相同的名字服务器的应用程序或任何主机均可获得共享的高速缓存

通常情况下，在Linux中的`/etc/resolv.conf`文件中配置了名字服务器的IP地址：

```shell
[gongruiyang@localhost ~]$ cat /etc/resolv.conf 
search localdomain
nameserver 192.168.14.2		# 名字服务器
```

现代Web浏览器设计为默认将DNS记录缓存一段时间，越靠近web浏览器进行DNS缓存，越能快速获得`域名-IP 映射`

操作系统中也有一份DNS缓存，位于`\etc\hosts`中

一次完整的 DNS 查询过程需要访问多台 DNS 服务器才能获得最终的结果，这必定会带来一定的时间延迟。

为了改善时间延迟，在进行 DNS 查询前有多级缓存来提供查询

![image-20210808181809798](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/image-20210808181809798.png)

缓存记录的存活并不是永久的，在响应报文中answers字段中的TTL字段表示存活时间（见后文Wireshark抓包）

# 四、DNS工具：host

在Linux中，使用`host命令`可以将待查询的域名传递给名字服务器，并显示返回的结果

```bash
[root@localhost gongruiyang]# host www.baidu.com
www.baidu.com is an alias for www.a.shifen.com.
www.a.shifen.com has address 182.61.200.6
www.a.shifen.com has address 182.61.200.7
```

由此我们可以看出：域名`www.baidu.com`这个域名先被转换成`www.a.shifen.com.`这个域名，然后再进行DNS解析出来两个地址，说明：`www.baidu.com`域名对应两个IP地址`182.61.200.6`和`182.61.200.7`

# 五、DNS PTR记录

指针查询方式（反向DNS查找）：给定一个IP地址，返回该地址对应的域名

```bash
[root@localhost gongruiyang]# host 140.252.13.34
34.13.252.140.in-addr.arpa domain name pointer pipen14.tuc.noao.edu.
[root@localhost gongruiyang]# host pipen14.tuc.noao.edu.
pipen14.tuc.noao.edu has address 140.252.13.34
```

该种方式常用于日志记录，由于系统日志只记录IP地址，反向DNS查找可以将这些IP地址转换为域名，以便日志更加易读

![image-20210808161516626](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/image-20210808161516626.png)

# 六、递归查询与迭代查询

> 先对以下两个名词做出区分
>
> * 递归DNS查询：是指向需要解析该查询的 DNS 解析器发出的`请求`
>
> * 递归DNS解析器：DNS 解析器是 DNS 查找的第一站，是一种`计算机`，其负责与发出初始请求的客户端打交道。解析器启动查询序列，最终使 URL 转换为必要的 IP 地址。

DNS解析分为`递归查询`和`迭代查询`两种方式，其中，`客户端`与`本地DNS服务器`之间一般采用`递归`查询，而`DNS服务器之间`一般采用`迭代`查询

* 递归查询：即如果本地DNS服务器在本服务器上没有查询到该域名，则会代替客户端的身份去向其他DNS服务器发送查询请求报文，客户端只需等待最终的结果即可
* 迭代查询：其他DNS服务器如果在本机上面未查到该域名，就会对本地DNS服务器说，下一步应该向哪一个DNS服务器发送请求报文



![image-20210808172504197](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/image-20210808172504197.png)



# 七、Wireshark抓DNS包验证报文结构

* 步骤一：设置Wireshark过滤条件：`icmp || DNS` ，如下图

![image-20210808173810681](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/image-20210808173810681.png)

* 步骤二：cmd中输入`ping www.baidu.com`，如下图

![image-20210808173942468](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/image-20210808173942468.png)

* 步骤三：查看DNS `查询报文 & 响应报文`

![image-20210808174112945](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/image-20210808174112945.png)

`查询报文`解析如图所示：

![image-20210808175612162](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/image-20210808175612162.png)

`响应报文`解析如图所示：

![image-20210808181029727](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/image-20210808181029727.png)

三个answers记录

![image-20210808182050287](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/image-20210808182050287.png)





---





> 参考资料：
>
> [掘金-计算机网络 - 图解 DNS & HTTPDNS 原理](https://juejin.cn/post/6884183177926033416#heading-6)
>
> 《TCP/IP 详解卷 I ：协议》
