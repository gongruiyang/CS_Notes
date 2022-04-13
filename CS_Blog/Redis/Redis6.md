# 一、NoSQL数据库简介

## 1. 技术的分类

1. 解决`功能`问题的技术：Java、Jsp、RDBMS、Tomcat、HTML、Linux、JDBC、SVN
2. 解决`扩展性`问题的技术：Struts、Spring、SpringMVC、Hibernate、Mybatis
3. 解决`性能`问题的技术：NoSQL、Java线程、Hadoop、Nginx、MQ、ElasticSearch

## 2. NoSQL数据库的作用

### 场景一：解决CPU与内存性能问题

> 当服务端与客户端进行连接创建会话时，若服务端采用Nginx来负载均衡时，session对象应该如何存储？



方案一：session对象存储在客户端的cookie中，缺点是不安全

方案二：session对象存储在文件服务器或数据库中，缺点是存在对数据库的大量IO，耗费资源

方案三：session对象在每个服务器中备份一份，缺点是造成大量的session对象冗余

最佳方案：使用NoSQL数据库存储session对象，服务器从NoSQL数据库中读取session对象，由于NoSQL数据库是建立在cache中的，读取速度快

### 场景二：解决IO问题

> 数据库随着日积月累存储着大量的数据，对于频繁使用的数据操作会产生大量IO，如何管理这些数据呢？



方案：水平切分、垂直切分、读写分离，缺点：通过破坏一定的业务逻辑来换取性能

最佳方案：将这些数据存储在NoSQL数据库中，NoSQL数据库作为缓存数据库，将专门的数据存储在其中，大大减少IO压力，直接通过内存来读取，还能减少访问速度

## 3. NoSQL数据库简介

NoSQL（not only SQL）意为""不仅仅是SQL"，泛指`非关系型数据库`，NoSQL以`key - value`模式存储

NoSQL与关系型数据库区别：

1. 不遵循SQL标准
2. 不支持ACID
3. 性能高于SQL

NoSQL适用场景：

1. 对数据高并发的读写
2. 海量数据的读写
3. 对数据高可扩展性的场景

NoSQL不适用场景：

1. 需要事务的支持
2. 基于SQL结构化查询存储

NoSQL类型数据库产品：

1. Memcache：最早出现的NoSQL数据库，不支持数据持久化，支持KV模型，只支持单一的字符串数据类型，作为缓存数据库辅助持久化的数据库
2. Redis：覆盖了Memcache的绝大部分功能，支持KV模型，还支持多种数据结构的存储
3. MongoDB：文档型数据库，数据存储在内存中，如果内存不足，将不常用的数据保存到硬盘，虽然是存储的是KV模式，但是对于value提供了丰富的查询功能，支持二进制数据及大型对象



# 二、Redis6概述与安装

## 1. Redis简介



## 2. Redis6的安装

> 1. 安装GCC

查看gcc版本

```shell
gcc --version
```

安装gcc

```shell
yum install gcc
```

> 2. 安装Redis

去官网下载Redis：[Redis](https://redis.io/)

解压Redis压缩文件

```shell
tar -zxvf redis-6.2.1.tar.gz
```

进入解压文件目录进行编译和安装

```shell
cd redis-6.2.1
make
make install
```

最终会被安装到默认目录`/usr/local/bin`，目录结构如下：

```

```





## 3. Redis两种启动方式

### 3.1 前台启动

```shell
cd /usr/local/bin
redis-server
```

前台启动导致当前terminal被redis占用，无法进行其他操作，关闭terminal就相当于关闭redis

### 3.2 后台启动

> 1. 备份conf文件

```shell
cp /opt/redis-XXX/redis.conf ./myRedis/
```

> 2. 修改conf文件中的字段：daemonize yes -> daemonize no

```shell
vim redis.conf
```

> 3. Redis启动

```shell
redis-server /myRedis/redis.conf
```

> 4. 查看Redis进程

```she
ps -ef | grep redis
```



## 4. Redis两种关闭方式

### 4.1 shutdown



### 4.2 kill

使用 `kill -9 ` 命令

```shell
kill -9 PID
```



## 5. Redis原理



# 03、常用的五大数据类型

# 04、Redis配置文件详解

# 05、Redis6的发布和订阅

# 06、Redis6新数据类型

# 07、Jedis操作Redis6

# 08、Redis6与springBoot整和

# 09、Redis6的事务操作

# 10、Redis6持久化之RDB

# 11、Redis6持久化之AOF

# 12、Redis6的主从复制

# 13、Redis6集群

# 14、Redis6应用问题解决

# 15、Redis6新功能

