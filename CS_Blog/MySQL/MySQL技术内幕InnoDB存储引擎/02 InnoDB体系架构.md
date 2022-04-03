# 一、思维导图

## 1. 后台线程

![image-20211111200708633](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202111112007721.png)

## 2. InnoDB 内存划分

![image-20211111195933783](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202111111959982.png)

## 3. 内存管理

![image-20211111203305855](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202111112033963.png)

# 二、笔记

## 1. 图解InnoDB体系架构

* InnoDB体系架构由三部分组成：后台线程 + 内存池 + 文件集合
* 后台线程：Master Thread、IO Thread、Purge Thread、Page Cleaner Thread
* 内存池：若干个内存块
* 文件集合：组成数据库

![image-20211108214210044](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202111082142215.png)

## 2. 后台线程

### 2.1 Master Thread

作用：负责将缓冲池中的数据异步刷新到磁盘中，保证数据的一致性，包括：脏页的刷新、合并插入缓冲、undo页的回收等

### 2.2 IO Thread

作用：用来对数据库文件做 IO 操作的线程

InnoDB 存储引擎中大量使用了 AIO 来处理写 IO 请求，其中的 IO Thread 的工作主要是负责这些 IO 请求的回调

> AIO：异步非阻塞式IO，无需额外的线程轮询所有 IO 操作状态是否改变，若状态改变，系统会通知对应的线程来处理

IO Thread一共有四种：insert buffer thread、log thread、read thread、write thread

* 查看 Thread 

```mysql
MySQL [(none)]> show variables like 'innodb_version'\G
*************************** 1. row ***************************
Variable_name: innodb_version
        Value: 5.7.31
1 row in set, 1 warning (0.00 sec)
MySQL [(none)]> show engine innodb status\G;
*************************** 1. row ***************************
  Type: InnoDB
  Name: 
Status: 
=====================================
2021-11-08 22:17:22 0x5b6c INNODB MONITOR OUTPUT
=====================================
Per second averages calculated from the last 52 seconds
--------
FILE I/O
--------
I/O thread 0 state: wait Windows aio (insert buffer thread)
I/O thread 1 state: wait Windows aio (log thread)
I/O thread 2 state: wait Windows aio (read thread)
I/O thread 3 state: wait Windows aio (read thread)
I/O thread 4 state: wait Windows aio (read thread)
I/O thread 5 state: wait Windows aio (read thread)
I/O thread 6 state: wait Windows aio (write thread)
I/O thread 7 state: wait Windows aio (write thread)
I/O thread 8 state: wait Windows aio (write thread)
I/O thread 9 state: wait Windows aio (write thread)
Pending normal aio reads: [0, 0, 0, 0] , aio writes: [0, 0, 0, 0] ,
 ibuf aio reads:, log i/o's:, sync i/o's:
Pending flushes (fsync) log: 0; buffer pool: 0
349 OS file reads, 56 OS file writes, 7 OS fsyncs
0.00 reads/s, 0 avg bytes/read, 0.00 writes/s, 0.00 fsyncs/s
```

由此可知，在 5.7.X 版本下，一共有 10 个 IO Thread，分别是一个 insert buffer thread 和 一个 log thread 和 四个 read thread 和 四个 write thread

```mysql
MySQL [(none)]> show variables like 'innodb_read_io_threads'\G;
*************************** 1. row ***************************
Variable_name: innodb_read_io_threads
        Value: 4
1 row in set, 1 warning (0.00 sec)

MySQL [(none)]> show variables like 'innodb_write_io_threads'\G;
*************************** 1. row ***************************
Variable_name: innodb_write_io_threads
        Value: 4
1 row in set, 1 warning (0.00 sec)
```

### 2.3 Purge Thread

作用：在事务提交之后，其所使用的 undolog 可能不再需要，此时就需要 Purge Thread 来回收已经使用并分配的 undo 页

在 InnoDB 1.1 版本之前，回收操作仅在 Master Thread 中完成，之后，为了减轻 Master Thread 的工作，进而提高 CPU 的使用率以及提升存储引擎性能，将回收操作独立到单独的线程中进行，便出现了 purge thread

* 查看 purge Thread 线程数量

```mysql
MySQL [(none)]> show variables like 'innodb_purge_threads'\G;
*************************** 1. row ***************************
Variable_name: innodb_purge_threads
        Value: 4
```

### 2.4 Page Cleaner Thread

作用：用于脏页的刷新操作

本来脏页的刷新操作也是在 Master Thread 中完成的，为了减轻 Master Thread 的工作而独立到单独的线程中去。

* 查看 Page Cleaner Thread 线程的数量

```mysql
MySQL [(none)]> show variables like 'innodb_page_cleaners'\G;
*************************** 1. row ***************************
Variable_name: innodb_page_cleaners
        Value: 1
```

## 3. 内存

### 3.1 缓冲池

#### 3.1.1 缓冲池简介

> 为什么需要缓冲池？
>
> 原因：由于 CPU 的 IO 频率与磁盘的 IO 频率之间差距较大，需要内存的频率来填补中间的频率差距，先将磁盘中的记录读取到位于内存上的缓冲池中，以备 CPU 使用

InnoDB 存储引擎是`基于磁盘存储`的，并将其中的数据记录按照`页的方式`进行管理，因此将其视为`基于磁盘的数据库系统`

缓冲池是一块`内存区域`，缓冲池中的数据记录是按照`页的方式`进行管理的

> 在数据库中进行`读取页`的操作：首先将从磁盘中读到的页存放在缓冲池中，下一次再读取相同的页时，先判断改页是否存在于缓冲池中，若在，称改页在缓冲池中被命中，直接读取该页，否则，读取磁盘上的页。
>
> 在数据库中进行`修改页`的操作：首先修改位于缓冲池中的页，然后通过 `Checkpoint` 的机制刷新回磁盘。

由此可见，缓冲池的大小越大，数据库的整体性能应该是越好的。故缓冲池的大小对于数据库性能来说是非常重要的。

缓冲池中缓存的`数据页类型`由：索引页、数据页、undo 页、插入缓冲页、自适应哈希索引、InnoDB存储的锁信息、数据字典信息等

* 图解InnoDB内存数据对象

![image-20211111130830778](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202111111308931.png)

#### 3.1.2 缓冲池相关变量

1. 缓冲池实例（innodb_buffer_pool_instances）

从 InnoDB 1.0.X 版本开始，允许有多个缓冲池实例，每个页根据哈希值平均分配到不同的缓冲池实例中去。好处是减少数据库内部资源竞争，增加数据库的并发处理性能。

```mysql
mysql> show variables like 'innodb_buffer_pool_instances'\G;
*************************** 1. row ***************************
Variable_name: innodb_buffer_pool_instances
        Value: 1
1 row in set, 1 warning (0.01 sec)
```

查看各个缓冲池状态（位于 information_schema 架构表innodb_buffer_pool_stats来观察）

```mysql
mysql> select pool_id, pool_size, free_buffers, database_pages
    -> from information_schema.innodb_buffer_pool_stats\G;
*************************** 1. row ***************************
       pool_id: 0
     pool_size: 512
  free_buffers: 223
database_pages: 289
1 row in set (0.00 sec)
```

2. 缓冲池单个实例大小（innodb_buffer_pool_size）

MySQL 5.7默认设置的缓冲池大小为 8MB

```mysql
mysql> show variables like 'innodb_buffer_pool_size'\G;
*************************** 1. row ***************************
Variable_name: innodb_buffer_pool_size
        Value: 8388608
1 row in set, 1 warning (0.00 sec)
mysql> SELECT @@innodb_buffer_pool_size/1024/1024;
+-------------------------------------+
| @@innodb_buffer_pool_size/1024/1024 |
+-------------------------------------+
|                          8.00000000 |
+-------------------------------------+
1 row in set (0.00 sec)
```

### 3.2 LRU / Free / Flush List

#### 3.2.1 LRU List

InnoDB 存储引擎是通过 LRU 算法来对缓冲池这个内存区域进行管理的。

> 思路是将最频繁使用的页放在 LRU 列表的前端，最少使用的页放在 LRU 列表的尾端，当缓冲池不能存放新读到的页时，将首先释放LRU列表的尾端的页

在 InnoDB 存储引擎中，缓冲池的`页大小默认为 16KB`，虽然是使用 LRU 算法进行页管理，但是与传统的 LRU 又稍有不同，在 InnoDB 存储引擎中，将新读取到的页并不是放在 LRU 列表的最前端，而是放在 `midpoint` 的位置上，在默认配置下，这个 midpoint 的位置在` LRU 列表的 5/8 处`，该位置可由变量 `innodb_old_blocks_pct`控制。

midpoint 是将冷链和热链链接起来的连接点，是冷热数据页分界线。

一般为了保护热区数据页不被刷出内存（刷出去的概率降低），可以将 midpoint 的位置设置在百分之20的位置（即innodb_old_blocks_pct = 20）

![image-20211111135838531](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202111111358595.png)

> 为什么不将新的页放在 LRU List 的首部，而是放在 midpoint 位置呢？
>
> 原因：可能新插入进来的页并非是热点数据，仅仅是当前SQL需要访问到的数据页，若放在 LUR 列表的首部，可能会将真正的热点数据页给置换出去。
>
> 将数据页放到 midpoint 位置后，多久才将该页置换到 LRU 的热端呢？
>
> 答案：由 innodb_old_block_time 参数控制，即当该页经过 1s 未被刷出内存，就调入热链首部

**page made young 操作**：页从 old 部分加入到 new 部分

**page not made young 操作**：由于 innodb_old_block_time 还未达到，导致页仍然位于 old 部分

* 通过 information_schema 架构中表` innodb_buffer_pool_stats `来观察`各个缓冲池实例的运行状态`

```mysql
mysql> select pool_id, hit_rate, pages_made_young, pages_not_made_young
    -> from information_schema.innodb_buffer_pool_stats\G;
*************************** 1. row ***************************
             pool_id: 0
            hit_rate: 0
    pages_made_young: 0
pages_not_made_young: 0
1 row in set (0.00 sec)
```

* 通过 information_schema 架构中表` innodb_buffer_page_lru` 来观察`LRU List中的数据页`

```mysql
mysql> select table_name, space, page_number, page_type
    -> from information_schema.innodb_buffer_page_lru
    -> where space = 57\G;
*************************** 1. row ***************************
 table_name: `capital_m`.`tb_account`
      space: 57
page_number: 3
  page_type: INDEX
*************************** 2. row ***************************
 table_name: NULL
      space: 57
page_number: 1
  page_type: IBUF_BITMAP
2 rows in set (0.00 sec)
```



> LRU List 相关变量

* innodb_old_block_time ：1000ms = 1s

```mysql
mysql> show variables like 'innodb_old_blocks_time'\G;
*************************** 1. row ***************************
Variable_name: innodb_old_blocks_time
        Value: 1000
1 row in set, 1 warning (0.00 sec)
```

* midpoint（innodb_old_blocks_pct）

```mysql
mysql> show variables like 'innodb_old_blocks_pct'\G;
*************************** 1. row ***************************
Variable_name: innodb_old_blocks_pct
        Value: 37
1 row in set, 1 warning (0.00 sec)
```

* 页大小（innodb_page_size）

```mysql
mysql> show variables like 'innodb_page_size'\G;
*************************** 1. row ***************************
Variable_name: innodb_page_size
        Value: 16384
1 row in set, 1 warning (0.00 sec)

mysql>  SELECT @@innodb_page_size/1024;
+-------------------------+
| @@innodb_page_size/1024 |
+-------------------------+
|                 16.0000 |
+-------------------------+
1 row in set (0.00 sec)
```

#### 3.2.2 Free List

LRU List 是用来管理已经读取的页，但是当数据库刚刚启动时，没有任何页被读取时，LRU List 是空的，这时页都被存放在 Free List 中，当要读取数据页时，会将该页从 Free List 中删除，再增加到 LRU List 中去，以维持页的守恒。

查看 Free List 使用情况和状态：`show engine innodb status\G;`

![image-20211111144754416](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202111111447513.png)

`free buffers` 表示 Free List

`database pages `表示 LRU List

`modified db pages`表示脏页的数量

`Pages made young` 表示数据页从冷链到热链的累积量

 `not young`表示数据页仍然保持在冷链的累积量

`non-youngs/s`表示每秒有多少次not young操作，若该值很高，表示有很高频率的物理读写

`youngs/s`表示每秒有多少次Pages made young操作，若该值很高，说明冷链很短或者innodb_old_blocks_time的值很小

`Buffer pool hit rate`表示缓冲池命中率，若该值大于95%表示缓冲池运行良好，否则，可能是由于全表扫描引起的 LRU List 污染的情况导致

`unzip_LRU`表示被压缩的页的数量，即将 16KB 的页压缩为1KB、2KB、4KB、8KB，对于非16KB的页由 unzip_LRU List来管理，LRU中的页包含了 unzip_LRU列表中的所有页

> unzip_LRU是怎么样从缓存池中分配内存的呢？
>
> 对于大小不同的页，分别放到对应大小的 unzip_LRU List 中，通过`伙伴算法`进行内存的分配，例如需要从缓冲区申请页为4KB的大小，过程如下：
>
> 1. 检查 4KB 的 unzip_LRU 列表，看看是否有可用的空闲页，若有则直接使用，
> 2. 若没有，则检查 8KB 的 unzip_LRU 列表，若能获得一个空闲的 8KB 的页，则拆分成 2 个 4KB 的页存放至 4KB 的unzip_LRU 列表
> 3. 若不能得到，则从 LRU 列表中申请到一个 16KB 的页，将其拆分成一个 8KB 的页和两个 4KB 的页，分别存放至对应的 unzip_LRU 列表中
>
> * 查看 unzip_LRU 列表中的页
>
> ```mysql
> mysql> select *
>     -> from information_schema.innodb_buffer_page_lru
>     -> where compressed_size <> 0;
> Empty set (0.00 sec)
> ```
>
> 

#### 3.2.3 Flush List

在 LRU 列表中的页被修改后，称改页为脏页，即`缓冲池中的页上的数据`与`磁盘上对应的页上的数据`产生了不一致。

Flush List 的作用就是将这些脏页进行组织管理。脏页既存在于 LRU 列表中，也存在与 Flush 列表中。

* 查看脏页信息

```mysql
mysql> select *
    -> from information_schema.innodb_buffer_page_lru
    -> where oldest_modification > 0\G;
Empty set (0.00 sec)
```



### 3.3 重做日志缓冲区

InnoDB 存储引擎首先将重做日志信息写入重做日志缓冲区，再以一定的频率将日志信息从缓冲区中刷入重做日志文件中。

![image-20211111153740009](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202111111537083.png)

重做日志缓冲区的大小一般设置为一秒钟内产生日志量最大值即可，因为每一秒钟都会进行刷新至文件的操作。

> 触发刷新的条件有以下三种：
>
> 1. `Master Thread 每一秒钟`都会将 redo log buffer 中的数据刷新至 redo log file 中
>
> 2. `每个事务提交时`都会将 redo log buffer 中的数据刷新至 redo log file 中
>
> 3. `当 redo log buffer 剩余空间小 1/2 时`就会将 redo log buffer 中的数据刷新至 redo log file 中

* 查看 redo log buffer 大小：1MB

```mysql
mysql> show variables like 'innodb_log_buffer_size'\G;
*************************** 1. row ***************************
Variable_name: innodb_log_buffer_size
        Value: 1048576
1 row in set, 1 warning (0.00 sec)

mysql> SELECT @@innodb_log_buffer_size/1024/1024;
+------------------------------------+
| @@innodb_log_buffer_size/1024/1024 |
+------------------------------------+
|                         1.00000000 |
+------------------------------------+
1 row in set (0.00 sec)
```

### 3.4 额外的内存池

作用：这些额外的内存池用于给数据结构体动态分配内存，其作用类似于堆内存，当这些额外的内存池被结构体申请耗尽后，会从缓冲池中进行申请。

例如：缓冲池中的`帧缓冲`及对应的`缓冲控制对象`，他们的数据结构的内存申请区域都是从额外内存池中申请的。

由此可知，当把缓冲池的内存设定的较大时，额外的内存池的大小也应该相应增加。
