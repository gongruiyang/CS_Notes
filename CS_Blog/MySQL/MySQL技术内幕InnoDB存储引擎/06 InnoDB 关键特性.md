# 1、Insert Buffer

## 1.1 聚集索引 与 非聚集索引

> 概念

聚集索引（Clustered Index）：数据记录在`物理磁盘上的物理顺序`与`索引列的值的逻辑顺序`一致，又称为一级索引

非聚集索引（Unclustered Index）：数据记录在`物理磁盘上的物理顺序`与`索引列的值的逻辑顺序`不一致，又称为二级索引或辅助索引

> 分类

聚集索引有primary key ，该类索引的 B+ 树的叶子节点是**数据结点**

非聚集索引分普通索引、唯一索引、全文索引，该类索引的 B+ 树的叶子节点是**索引结点**，需要进行二次查询才能查到数据

其中 primary key 的选择有以下三种情况：

1. 如果表中定义有主键，该主键索引就是聚集索引
2. 如果未定义主键，MySQL取第一个唯一索引（unique）而且只含非空列（NOT NULL）作为主键，InnoDB使用它作为聚集索引
3. 如果找不到第二种情况的列，InnoDB就自己产生一个row id列，它有六个字节，而且是隐藏的，使其作为聚集索引。

> 数据插入特性

在InnoDB存储引擎中，主键是行唯一的标识符。故行记录插入的顺序是按照主键递增的顺序进行插入的，因此，插入聚集索引一般就是按照顺序插入的，不需要磁盘的随机读取，故插入性能较高

当聚集索引与非聚集索引同时存在时，在进行插入操作时，数据页的存放还是按照主键（聚集索引）进行顺序存放的，但是对于非聚集索引叶子节点的插入不再是顺序的，这时，就需要离散地访问非聚集索引页，由于随机读取的存在导致了插入性能的下降

故：**聚集索引插入具有顺序性，非聚集索引插入具有离散性**

## 1.2 非聚集索引的插入过程

对于非聚集索引的插入或更新操作：

1. 判断待插入的非聚集索引页是否存在于缓冲池中
2. 若在，则直接插入到辅助索引叶子节点上
3. 若不再，则先放入到一个` Insert Buffer 对象`中去，然后再以一定的频率和情况进行` insert buffer `和 `辅助索引叶子节点`的合并（merge）操作

合并操作的好处：将多个对于同一个索引页的插入操作合并到一个操作中，大大提高了对于非聚集索引插入的性能

## 1.3  使用 Insert Buffer 的条件

使用 Insert Buffer 需要满足以下两个条件：

1. 索引必须是辅助索引（非聚簇索引或二级索引）
2. 索引不能是唯一的

之所以辅助索引不能是唯一的，是因为在插入缓冲时，数据库并不去查找索引页来判断插入记录的唯一性，如果去查找判断的话，就产生了离散读取的情况，从而导致了 insert buffer 的意义（因为唯一索引需要去查找判断插入记录的唯一性）

总结：**insert buffer 就是为了避免 非聚集索引的插入 的离散性，进而提升插入效率**

## 1.4 查看数据库 insert buffer 的信息

```mysql
MySQL [(none)]> show engine innodb status\G;
-------------------------------------
INSERT BUFFER AND ADAPTIVE HASH INDEX
-------------------------------------
Ibuf: size 1, free list len 0, seg size 2, 0 merges
merged operations:
 insert 0, delete mark 0, delete 0
discarded operations:
 insert 0, delete mark 0, delete 0
```

* seg size 2：表示当前 insert buffer 大小为 2 * 16KB
* size 1：表示已经合并记录页的数量
* free list len 0：表示空闲列表的长度

## 1. 5 delete buffer 和 purge buffer

InnoDB 存储引擎可以对DML操作（insert、delete、update）都进行缓冲，分别使用 insert buffer 、delete buffer、purge buffer，后面两种 buffer 统称为 change buffer

对一条记录进行 delete 操作可以分为两个过程：

1. 将记录标记为删除，对应 delete buffer
2. 真正将记录删除，对应 purge buffer

InnoDB 提供了参数 `innodb_change_buffering` 来开启各种 buffer 的选项。该参数可选的值为 insert \ delete \ purges \ changes \ all \ none

```mysql
MySQL [(none)]> show variables like 'innodb_change_buffering'\G;
*************************** 1. row ***************************
Variable_name: innodb_change_buffering
        Value: all
1 row in set, 1 warning (0.03 sec)
```

可以通过参数`innodb_change_buffer_max_size`来控制 change buffer 的大小

```mysql
MySQL [(none)]> show variables like 'innodb_change_buffer_max_size'\G;
*************************** 1. row ***************************
Variable_name: innodb_change_buffer_max_size
        Value: 25
1 row in set, 1 warning (0.00 sec)
```

## 1.6 insert buffer 数据结构探究

insert buffer 对应的结构体：`mysql-5.7.19\storage\innobase\include\ibuf0ibuf.ic`

```c
/** Insert buffer struct */
struct ibuf_t{
    dict_index_t* index;    /*!< insert buffer index */
    ulint   size;     /*!< current size of the ibuf index tree, in pages */
    ulint   max_size; /*!< recommended maximum size of the ibuf index tree, in pages */
    ulint   seg_size; /*!< allocated pages of the file segment containing ibuf header and tree */
    bool    empty;    /*!<  true insert buffer tree is empty. */
    ulint   free_list_len;  /*!< length of the free list */
    ulint   height;   /*!< tree height */
    ulint   n_merges; /*!< number of pages merged */
    ulint   n_merged_ops[IBUF_OP_COUNT];
    ulint   n_discarded_ops[IBUF_OP_COUNT];
};
```

* max_size：`insert buffer index tree` 的大小，即 insert buffer 的最大占有内存
* index：插入缓冲索引

数据库启动时，初始化 insert buffer 时将最大设置为缓冲池内存的 1/4 

```c
#define CHANGE_BUFFER_DEFAULT_SIZE (25)
ibuf->max_size = (buf_pool_get_curr_size() / UNIV_PAGE_SIZE) * CHANGE_BUFFER_DEFAULT_SIZE / 100;
```

由源码的注释可以看出，insert buffer 其实是一颗 B+ 树，insert buffer B+ tree 的由叶子节点的非叶子节点组成，叶子节点就是索引页(index page)

当前版本中，全局只有一颗insert buffer B+ 树，负责对所有表的辅助索引做插入缓冲操作，这课树存放在共享表空间中，默认为 ibdata1 中

非叶子节点存放的是 search key ，其构造如图所示：

![image-20211220160007648](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202112201600722.png)

* space ：每个表有唯一的 space_id ，用来标识是哪张表
* marker：用于兼容老版本
* offset：表示页所在位置的偏移量，也叫 page_no

当一个辅助索引要插入到页（space, offset）时，如果这个页不再缓冲池中，那么 InnoDB 存储引擎首先会构造出来一个 search key ，接下来查询 insert buffer B+ 树具体插入位置，寻找到位置后，将这个 search key 构造成叶子节点结构后再插入，构造规则如下：

![image-20211220162805825](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202112201628879.png)

* metadata：低两字节存储当前记录进入 insert buffer 的顺序，第三个字节表示操作类型，第四个字节是额外标志

```c
/* Offsets for the fields at the beginning of the fourth field */
#define IBUF_REC_OFFSET_COUNTER 0 /*!< Operation counter */
#define IBUF_REC_OFFSET_TYPE  2 /*!< Type of operation */
#define IBUF_REC_OFFSET_FLAGS 3 /*!< Additional flags */
```

metadata结构：

![image-20211220163152261](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202112201631322.png)



为了保证`辅助索引页中的记录`能够插入到 insert buffer B+ 树，需要有一个特殊的页来标记每个辅助索引页的可用空间，这个特殊的页为 insert buffer bitmap page

每个 insert buffer bitmap page 用来追踪 2^13 个辅助索引页，也就是 2^8 个区，每个辅助索引页占 4 位

这4位分别存储以下信息：

```c
/** Offsets to the per-page bits in the insert buffer bitmap */
#define IBUF_BITMAP_FREE  0 /*!< Bits indicating the amount of free space */
#define IBUF_BITMAP_BUFFERED  2 /*!< TRUE if there are buffered changes for the page */
#define IBUF_BITMAP_IBUF  3 /*!< TRUE if page is a part of the ibuf tree, excluding the root page, 
                                or is in the free list of the ibuf */
```

![image-20211220164604561](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202112201646620.png)

* IBUF_BITMAP_FREE：表示该辅助索引页中可用空间数量，0表示无剩余可用空间，1表示剩余空间大于1/32页，2表示剩余空间大于1/16页，3表示剩余空间大于1/8页
* IBUF_BITMAP_BUFFERED：表示该辅助页是否有记录被缓存在 insert buffer B+ 树中，1表示有
* IBUF_BITMAP_IBUF：表示该页是否为 insert buffer B+ 树的索引页，1表示是

## 1.7 从 B+ 树 Merge 至辅助索引页的时机

![image-20211220165556129](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202112201655214.png)

合适时机有以下几种情况：

1. 辅助索引页被读取到缓冲池中时
2. Insert Buffer Bitmap 页追踪该辅助索引页可用空间至少为1/32时，会强制读取辅助索引页
3. Master Thread  每一段时间会进行一次 merge insert buffer 操作，具体数量根据活动状态确定

合并时页的选择：

InnoDB存储引擎会在 insert buffer B+树上随机选择一个页，读取该页的 space 及之后所需数量的页。这种算法可以很好的保证公平性，若读取到的表已经被删除了，此时可以直接丢弃数据记录

# 2、Double Write

## 2.1 Double Write 解决了什么问题

当 InnoDB 存储引擎正在写入某个页到表中，而这个页只写了一部分，突然数据库宕机了，导致数据页一部分是新数据一部分是旧数据，产生了不完整的数据页，这种情况称为部分写失败（partial page write）

能否使用重做日志进行恢复呢？答案是不一定的，重做日志中记录的是对页的物理操作，如果被操作的页已经产生了损坏，再对其进行重做是没有任何意义的。（例如：在偏移量800处写'aaa'记录，在损坏页前后偏移量800附近的数据可能是不一样的）

所以，我们最好给数据页做一个备份，当出现数据页损坏时，用备份来恢复这个 损坏页，然后再进行重做日志，这就是 doublewrite

## 2.2 Double Write 工作原理

doublewrite 由两部分内存组成，一部分位于内存中的double write buffer 大小为 2 MB，另一部分位于磁盘的共享表空间中同样也是 2 MB

![image-20211220213205582](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202112202132763.png)

对缓冲池中的脏页进行刷新时：

1. 先通过 memcopy 函数将脏页**复制**到 doublewrite buffer 中，将 buffer 写满
2. 再将  doublewrite buffer 中的 2 MB 的脏页按照每次 1 MB 的大小分两次**顺序写**入磁盘上的共享表空间的 double write memory 上
3. 再将  doublewrite buffer 中的 2 MB 的脏页以**离散写**的方式写入到脏页各自的表空间文件中去
4. 当离散写完成，脏页数据固化后，再将 double write memory 中的数据标记为可覆盖，以供下一次写入

出现宕机时，损坏的数据页直接从磁盘上的 double write memory 中的备份**恢复（recovery）**即可

## 2.3 查看数据库 doublewrite 运行情况

```mysql
MySQL [(none)]> show global status like 'innodb_dblwr%'\G;
*************************** 1. row ***************************
Variable_name: Innodb_dblwr_pages_written
        Value: 2
*************************** 2. row ***************************
Variable_name: Innodb_dblwr_writes
        Value: 1
```

* `Innodb_dblwr_pages_written`：表示 doublewrite 写入的总页数
* `Innodb_dblwr_writes`：表示 doublewrite 写入的次数

## 2.4 产生部分写失败的原因

一个数据页是 16K ，但是文件系统 IO 的最小单位是 4K ，故一个数据页需要写 4 次 IO 才能写完整

redo log 每次写入的单位是 512字节 ，也就是磁盘IO的最小单位，故不存在写入不完整的情况

# 3、Adaptive Hash Index

## 3.1 建立 AHI 的初衷

B+ 树的查找次数，取决于 B+ 树的高度，一般 B+ 树的高度为 3 ~ 4 层，故需要 3 ~ 4 次的查询才能完成一次查找

InnoDB 存储引擎会监控对于表上各个索引页的查询，自动根据`访问频率`和`访问模式`来自动为某些热点页建立哈希索引，使得查找的时间复杂度为O(1)

访问模式指的是**查询条件格式**

## 3.2 建立 AHI 的要求

建立 AHI 的要求如下：

1. 以某个访问模式查询了 100 次
2. 页通过该模式访问了 N 次，其中 N = 页中的记录数 * 1 / 16

哈希索引只能用来搜索等值的查询，例如：select * from table where id = 'XXX'，对于范围查找来说，不能使用哈希索引

以下两种模式交替进行也不会建立 AHI，必须是一种模式：

* where a = xxx
* where a =xxx and b = xxx

## 3.3 查看数据库 AHI 的使用情况

```mysql
MySQL [(none)]> show engine innodb status\G;
-------------------------------------
INSERT BUFFER AND ADAPTIVE HASH INDEX
-------------------------------------
Hash table size 2267, node heap has 0 buffer(s)
Hash table size 2267, node heap has 0 buffer(s)
Hash table size 2267, node heap has 0 buffer(s)
Hash table size 2267, node heap has 0 buffer(s)
Hash table size 2267, node heap has 0 buffer(s)
Hash table size 2267, node heap has 0 buffer(s)
Hash table size 2267, node heap has 0 buffer(s)
Hash table size 2267, node heap has 0 buffer(s)
0.00 hash searches/s, 0.00 non-hash searches/s
```

由 AHI 的信息可以看出：AHI的大小、每秒哈希查找次数、每秒非哈希查找次数

```mysql
MySQL [(none)]> show variables like 'innodb_adaptive_hash_index'\G;
*************************** 1. row ***************************
Variable_name: innodb_adaptive_hash_index
        Value: ON
1 row in set, 1 warning (0.01 sec)
```

* `innodb_adaptive_hash_index`：该参数可以标识是否启用 AHI

# 4、AIO

异步IO：Asynchronous IO （AIO），即每次 IO 操作，不需要等待 IO 结束就能继续做其他操作

同步IO：synchronous IO，即每次 IO 操作，需要等待 IO 结束才能继续做其他操作

AIO的优势就是可以进行 IO merge 操作，即把多个 IO 合并为 1 个 IO 操作

> 例如：用户需要访问三个页：(8, 6) (8, 7) (8, 8)
>
> 解析：这三个页都位于同一个空间内，并且 page_no 是连续的，故 AIO 会发送一个 IO 请求，从 (8, 6) 开始，连续读取三个页大小的内存即可

控制 AIO 是否开启参数：`innodb_use_native_aio`

```mysql
MySQL [(none)]> show variables like 'innodb_use_native_aio'\G;
*************************** 1. row ***************************
Variable_name: innodb_use_native_aio
        Value: ON
1 row in set, 1 warning (0.00 sec)
```



# 5、Flush Neighbor Page

Flush Neighbor Page：即刷新邻接页，工作原理为：当刷新一个脏页时，InnoDB 存储引擎会检测该页所在区的所有页，如果有脏页一起刷新

好处：通过AIO可以将连续的脏页都刷新了

