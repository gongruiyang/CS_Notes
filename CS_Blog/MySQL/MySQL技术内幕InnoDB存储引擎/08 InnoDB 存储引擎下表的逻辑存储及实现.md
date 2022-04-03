# 一、索引组织表

在 InnoDB 存储引擎中，表中的数据都是根据`主键顺序`组织存放的，这种组织数据的方式的表称为`索引组织表`

表主键的定义方式有以下三种：

1. 显式定义一个非空属性为主键
2. 若没有使用 1 定义主键，则首先判断表中是否有`非空的唯一索引`，若有则该列为主键
3. 若 1 和 2 都没有满足，则 InnoDB 存储引擎自动创建一个 6 字节的指针，即 `_rowid`来作为主键

见示例：

```sql
MySQL [gongruiyang]> create table z(
    -> a int not null,
    -> b int null,
    -> c int not null,
    -> d int not null,
    -> unique key (b),
    -> unique key (d),
    -> unique key (c));
Query OK, 0 rows affected (0.05 sec)

MySQL [gongruiyang]> insert into z select 1,2,3,4;
Query OK, 1 row affected (0.01 sec)
Records: 1  Duplicates: 0  Warnings: 0

MySQL [gongruiyang]> insert into z select 5,6,7,8;
Query OK, 1 row affected (0.01 sec)
Records: 1  Duplicates: 0  Warnings: 0

MySQL [gongruiyang]> insert into z select 9,10,11,12;
Query OK, 1 row affected (0.01 sec)
Records: 1  Duplicates: 0  Warnings: 0

MySQL [gongruiyang]> select a,b,c,d,_rowid from z;
+---+------+----+----+--------+
| a | b    | c  | d  | _rowid |
+---+------+----+----+--------+
| 1 |    2 |  3 |  4 |      4 |
| 5 |    6 |  7 |  8 |      8 |
| 9 |   10 | 11 | 12 |     12 |
+---+------+----+----+--------+
3 rows in set (0.01 sec)
```

> 由 `_rowid`的值我们可知：z 表的主键是 d
>
> 由于 b 是可为 null 的，所以不符合设置为主键的条件
>
> 属性 d 和属性 c 都是符合条件的，但是 d 比 c 先定义，故选择 d 作为主键

`_rowid `只能用来查看`单列主键`，对于`联合主键`就无能为力了



# 二、InnoDB 逻辑存储结构

在 InnoDB 存储引擎下，所有的数据都被逻辑的存放在表空间（tablespace）中，表空间又由各种段组成，段由区组成，区由页组成，页由行记录结构组成

![image-20211223112559472](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202112231126639.png)

## 1. 表空间

* 若没开启 `innodb_file_per_table`，则所有表的数据都放在`共享表空间 ibdata1 `中
* 若开启 `innodb_file_per_table`，则每张表的`数据、索引、insert buffer bitmap page `放在`独立的表空间`内，其他数据（undo log page、insert buffer index page、double write buffer等）还是放在`共享表空间 ibdata1` 中

使用 `py_innodb_page_info.py`工具查看共享表空间中有哪些数据页：

```shell
gongruiyang@gongruiyang:/david-mysql-tools/py_innodb_page_type$ python py_innodb_page_info.py ibdata1
Total number of page: 4864:
Insert Buffer Bitmap: 6
System Page: 133
Transaction system Page: 3
Freshly Allocated Page: 4440
Undo Log Page: 234
File Segment inode: 11
B-tree Node: 30
File Space Header: 7
```

> 我们可以看到在`共享表空间 ibdata1`中，一共有 4864 个页：
>
> undo log 页有 234 个
>
> B-tree Node （数据）页有 30 个

## 2. 段

`叶子节点段`即是`数据段`，`非叶子节点段`即是`索引段`，回滚段后续文章再续述，每个段由若干个区（Extend）组成，段的管理由 InnoDB 引擎完成。

## 3. 区

区是由连续的页组成的空间，每个区的大小恒为 1MB，一个区包含 64 个正常页，如果区内存的是压缩页，那么对应页大小为 2K、4K、8K 的页数量为 512 个、256个、128个，所以不论页的大小如何变化，区的大小恒不变

独立表空间的区大小变化：新建的表的表空间大小为 96 K（只有一个数据页） ->  刚开始的32个数据页是随着表中记录的增加而逐个增加的 -> 后续都每次表空间按照连续 64 个数据页的大小开始增长

独立表空间区变化图解：

![image-20211223161523761](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202112231615860.png)

## 4. 页

页（Page）是 InnoDB 磁盘管理的最小单位，默认每个页大小为 16KB ，可通过参数`innodb_page_size`来修改页大小

```sql
MySQL [gongruiyang]> show variables like 'innodb_page_size'\G;
*************************** 1. row ***************************
Variable_name: innodb_page_size
        Value: 16384
1 row in set, 1 warning (0.00 sec)
```

常见的页类型有：

* 数据页（B-tree Node）
* undo 页（undo log page）
* 系统页（system page）
* 插入缓冲位图页（insert buffer bitmap page）
* 插入缓冲空闲列表页（insert buffer free list page）
* 事务数据页（Transaction system page）
* 未压缩的二进制大对象页（uncompressed BLOB page）
* 压缩的二进制大对象页（compressed BLOB page）

## 5. 行记录

InnoDB 存储引擎是面向行（row - oriented）的，即数据是按照行进行存储的，每个页中存储的行记录的格式都是定义好的，最多允许存储（16KB / 2 - 200）行记录，即 7992 条行记录

# 三、InnoDB 行记录的格式

InnoDB 存储引擎提供了两种行记录的存储格式：`Compact` 、 `Redundant`、`Compressed`、`Dynamic`

查看表的行记录存储格式方法：通过 `show table status like`来看 `Row_format`字段即可

```sql
MySQL [gongruiyang]> show table status like 't%'\G;
*************************** 1. row ***************************
           Name: t
         Engine: InnoDB
        Version: 10
     Row_format: Dynamic
           Rows: 4
 Avg_row_length: 4096
    Data_length: 16384
Max_data_length: 0
   Index_length: 0
      Data_free: 0
 Auto_increment: NULL
    Create_time: 2021-12-23 15:56:47
    Update_time: 2021-12-23 15:59:17
     Check_time: NULL
      Collation: latin1_swedish_ci
       Checksum: NULL
 Create_options: 
        Comment: 
*************************** 2. row ***************************
           Name: test
         Engine: InnoDB
        Version: 10
     Row_format: Dynamic
           Rows: 31
 Avg_row_length: 17441
    Data_length: 540672
Max_data_length: 0
   Index_length: 0
      Data_free: 0
 Auto_increment: 65
    Create_time: 2021-12-23 14:59:34
    Update_time: 2021-12-23 15:42:36
     Check_time: NULL
      Collation: latin1_swedish_ci
       Checksum: NULL
 Create_options: 
        Comment: 
2 rows in set (0.00 sec)
```

## 1. Compact 行记录格式

Compact 行记录的格式如下图：

![image-20211224110159734](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202112241101805.png)

* 变长字段长度列表：该字段的长度是变化的，若列的长度小于 255 字节，用 1 字节表示，若列大于 255 字节，用 2 字节表示
* NULL标志位：共 8 位，每一位表示当前列是否为 NULL，是 NULL 则当前位为 1
* 头信息：占5字节，每一位的含义如下表

| 名称         | 大小（单位：位） | 含义                                                         |
| ------------ | ---------------- | ------------------------------------------------------------ |
| （）         | 1                | 未知                                                         |
| （）         | 1                | 未知                                                         |
| deleted_flag | 1                | 标记此行是否删除                                             |
| min_rec_flag | 1                | 如果此行被预先定义为最小的记录，则设置为1                    |
| n_owned      | 4                | 此行拥有的记录数                                             |
| heap_no      | 13               | 索引堆中该记录的排序记录                                     |
| record_type  | 3                | 记录类型，000表示普通，001表示B+树节点指针，010表示Infimum，011表示Supremum，1xx表示保留 |
| next_record  | 16               | 页中下一条记录的相对位置                                     |

每行数据中除了用户定义的列外还有两个隐藏列：事务ID列（6字节）和回滚指针列（7字节），如果用户没有定义主键，可能还会多出来一个 rowid 列（6字节）

> 举例1：如果一个表使用 `row_format = compact`方式存储， 一共有四列（varchar(10) , varchar(10), char(10), varchar(10)），插入一条数据为（'a', NULL,  ‘bb’, 'ccc'）
>
> 则使用 Compact 行记录的格式存储在独立表空间中的数据为：
>
> `NULL标志位`：0x02，二进制为 10，表示第二列为NULL值
>
> `col1` ：61('a')
>
> `col3`：62('b') 62('b') 20 20 20 20 20 20 20 20 
>
> `col4`：63('c') 63('c') 63('c')
>
> 由此，我们可以观察到以下规律：
>
> 1. 我们可以看出第三列为固定长度的char，如果该列中存储的数据没有达到固定长度，则剩余的字节使用 `0x20` 填充
> 2. 无论是 char 还是 varchar 在 Compact 格式下存储 NULL 都不占用空间



## 2. Redundant 行记录格式

Redundant 行记录格式如下图：

![image-20211224123813594](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202112241238687.png)

* 字段长度偏移列表：同 Compact 的变长字段长度列表
* 头信息：占 6 个字节，具体位信息如下表

| 名称            | 大小（单位：位） | 描述                                      |
| --------------- | ---------------- | ----------------------------------------- |
| （）            | 1                | 未知                                      |
| （）            | 1                | 未知                                      |
| deleted_flag    | 1                | 标记此行是否已经被删除                    |
| min_rec_flag    | 1                | 如果此行被预先定义为最小的记录，则设置为1 |
| n_owned         | 4                | 该记录拥有的记录数                        |
| heap_no         | 13               | 索引堆中该条记录的索引号                  |
| n_fields        | 10               | 记录中列的数量                            |
| 1byte_offs_flag | 1                | 偏移列表为 1 字节还是 2 字节              |
| next_record     | 16               | 页中下一条记录的相对位置                  |

MySQL数据库一行支持最多的列个数由`n_fields`控制，即 `2^10 -1 = 1023 个`

我们发现：redundant 格式与 compact 格式不同的是没有 `NULL标志位`，可以看看下面例子中 redundant 格式是如何处理 NULL 列的

> 举例1：如果一个表使用 `row_format = redundant `方式存储， 一共有四列（varchar(10) , varchar(10), char(10), varchar(10)），插入一条数据为（'d', NULL,  NULL, 'fff'）
>
> 则使用 Redundant行记录的格式存储在独立表空间中的数据为：
>
> `字段长度偏移列表`：21 9e 94 14 13 0c 06
>
> `col1` ：64('d')
>
> `col3`：00 00 00 00 00 00 00 00 00 00
>
> `col4`：66('f') 66('f') 66('f') 
>
> 由数据我们发现：
>
> 1. 数据类型为 Char 的 NULL 列，会使用定义长度的个数个 0x00 来填充
> 2. 数据类型为 varchar 的 NULL 列，则不存储



## 3. Compressed 行记录格式和 Dynamic 行记录格式

这两种行记录格式采用了`完全行溢出`的方式，B+ 数的叶子节点上面存储的是数据页，完全行溢出的方式是在数据页中存储 20 字节的指针，时间的数据都存放在 `Off Page`中，如图所示：

![image-20211224144035646](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202112241440739.png)



# 四、不同数据类型的存储方式探究

## 1. varchar

MySQL 数据库的 varchar 类型可以存放 `65532` 个 `latin字符集编码` 的字符，可以存放`21845`个`utf8字符集编码`的字符，可以存放`32767`个`gbk字符集编码`的字符，这里的的存储上限是指`所有的 varchar 类型的列之和`

通过以下样例我们来看看 varchar 是如何存储的：

1. 产生一个 `65532`大小的数据记录

```sql
MySQL [gongruiyang]> create table tt(
    -> a varchar(65532))engine=innodb charset=latin1;
Query OK, 0 rows affected (0.06 sec)

MySQL [gongruiyang]> insert into tt select repeat('a', 65532);
Query OK, 1 row affected (0.03 sec)
Records: 1  Duplicates: 0  Warnings: 0
```

```shell
gongruiyang@gongruiyang:/mnt/e/宫瑞阳/计算机/GitHub开源项目/david-mysql-tools/py_innodb_page_type$ python py_innodb_page_info.py -v tt.ibd
page offset 00000000, page type <File Space Header>
page offset 00000001, page type <Insert Buffer Bitmap>
page offset 00000002, page type <File Segment inode>
page offset 00000003, page type <B-tree Node>, page level <0000>
page offset 00000004, page type <Uncompressed BLOB Page>
page offset 00000005, page type <Uncompressed BLOB Page>
page offset 00000006, page type <Uncompressed BLOB Page>
page offset 00000007, page type <Uncompressed BLOB Page>
page offset 00000008, page type <Uncompressed BLOB Page>
Total number of page: 9:
Insert Buffer Bitmap: 1
Uncompressed BLOB Page: 5
File Space Header: 1
B-tree Node: 1
File Segment inode: 1
```

> 通过观察发现，只有一个数据页，但是多出来了 5 个 `Uncompressed BLOB Page（未压缩的二进制大对象页）`

2. 通过对页内数据分析发现数据页中只存放了这 65532 个字符的前 768 字节的前缀数据，剩下的（65532 - 768）个字节的数据存放在了 Uncompressed BLOB Page 中

![image-20211224162140347](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202112241621428.png)

那是不是说，只要 varchar 中存储的数据大于 768 个字节，多余的字节就一定会存储的未压缩的二进制大对象页中？如果是这样的话，就会导致 B+ 树失去了意义（因为只有一个数据页，剩下数据就往二进制大对象页中插入就行），成为了一个链表。

如果一个数据页中恰好能放下两条记录，那么就不会把数据放到未压缩的二进制大对象页中去了，那么这个恰好放下两条记录的 varchar阈值 是多少呢？经过反复测试发现这个阈值为 8098 字节。

* 实验一：设置 varchar 为8098

```sql
MySQL [gongruiyang]> create table tt(a varchar(8098))engine=innodb charset=latin1;
Query OK, 0 rows affected (0.05 sec)

MySQL [gongruiyang]> insert into tt select repeat('a', 8098);
Query OK, 1 row affected (0.02 sec)
Records: 1  Duplicates: 0  Warnings: 0

MySQL [gongruiyang]> insert into tt select repeat('a', 8098);
Query OK, 1 row affected (0.01 sec)
Records: 1  Duplicates: 0  Warnings: 0
```

```shell
gongruiyang@gongruiyang:/mnt/e/宫瑞阳/计算机/GitHub开源项目/david-mysql-tools/py_innodb_page_type$ python py_innodb_page_info.py -v tt.ibd
page offset 00000000, page type <File Space Header>
page offset 00000001, page type <Insert Buffer Bitmap>
page offset 00000002, page type <File Segment inode>
page offset 00000003, page type <B-tree Node>, page level <0000>
page offset 00000000, page type <Freshly Allocated Page>
page offset 00000000, page type <Freshly Allocated Page>
Total number of page: 6:
Freshly Allocated Page: 2
Insert Buffer Bitmap: 1
File Space Header: 1
B-tree Node: 1
File Segment inode: 1
```

插入第三条数据：

```sql
MySQL [gongruiyang]> insert into tt select repeat('a', 8098);
Query OK, 1 row affected (0.01 sec)
Records: 1  Duplicates: 0  Warnings: 0
```

```shell
gongruiyang@gongruiyang:/mnt/e/宫瑞阳/计算机/GitHub开源项目/david-mysql-tools/py_innodb_page_type$ python py_innodb_page_info.py -v tt.ibd
page offset 00000000, page type <File Space Header>
page offset 00000001, page type <Insert Buffer Bitmap>
page offset 00000002, page type <File Segment inode>
page offset 00000003, page type <B-tree Node>, page level <0001>
page offset 00000004, page type <B-tree Node>, page level <0000>
page offset 00000005, page type <B-tree Node>, page level <0000>
Total number of page: 6:
Insert Buffer Bitmap: 1
File Space Header: 1
B-tree Node: 3
File Segment inode: 1
```

> 此时可以发现，插入两条数据时只有一个数据页，当插入第三条数据时出现了 B+ 树的分裂操作，产生了两个数据页

* 实验二：设置 varchar 为8099

```sql
MySQL [gongruiyang]> insert into t select repeat('a', 8099);
Query OK, 1 row affected (0.01 sec)
Records: 1  Duplicates: 0  Warnings: 0
```

```shell
gongruiyang@gongruiyang:/mnt/e/宫瑞阳/计算机/GitHub开源项目/david-mysql-tools/py_innodb_page_type$ python py_innodb_page_info.py -v t.ibd
page offset 00000000, page type <File Space Header>
page offset 00000001, page type <Insert Buffer Bitmap>
page offset 00000002, page type <File Segment inode>
page offset 00000003, page type <B-tree Node>, page level <0000>
page offset 00000004, page type <Uncompressed BLOB Page>
page offset 00000000, page type <Freshly Allocated Page>
Total number of page: 6:
Insert Buffer Bitmap: 1
Freshly Allocated Page: 1
File Segment inode: 1
B-tree Node: 1
File Space Header: 1
Uncompressed BLOB Page: 1
```

> 才插入一条记录，就发现已经产生了 Uncompressed BLOB Page ，说明已经将部分数据插入到了 Uncompressed BLOB Page 中

总结：所以 varchar 的存储方式如图所示

![image-20211224165214105](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202112241652199.png)

## 2. char

我们使用 `CHAR(N)`来定义一个存储 N 个字节定长字段，那么它真的只能存 N 个嘛？看以下实验：

定义一个 GBK 字符集编码的 char(2) 属性，并插入三条数据：ab、我们、a

```sql
MySQL [gongruiyang]> create table t(a char(2))engine=innodb charset=GBK;
Query OK, 0 rows affected (0.05 sec)

MySQL [gongruiyang]> insert into t select 'ab';
Query OK, 1 row affected (0.01 sec)
Records: 1  Duplicates: 0  Warnings: 0

MySQL [gongruiyang]> insert into t select '我们';
Query OK, 1 row affected (0.01 sec)
Records: 1  Duplicates: 0  Warnings: 0

MySQL [gongruiyang]> insert into t select 'a';
Query OK, 1 row affected (0.01 sec)
Records: 1  Duplicates: 0  Warnings: 0

MySQL [gongruiyang]> select a, CHAR_LENGTH(a), LENGTH(a) from t\G;
*************************** 1. row ***************************
             a: ab
CHAR_LENGTH(a): 2
     LENGTH(a): 2
*************************** 2. row ***************************
             a: 我们
CHAR_LENGTH(a): 2
     LENGTH(a): 4
*************************** 3. row ***************************
             a: a
CHAR_LENGTH(a): 1
     LENGTH(a): 1
3 rows in set (0.00 sec)
```

> 我们发现：
>
> ‘ab’ 和 '我们' 的长度都是 2，‘a’ 的长度是 1
>
> 实际上在内部存储上：'ab' 占 2 个字节， ''我们'占 4 个字节，'a'占 1 个字节

在 Compact/Redundant  行记录的格式中：如果 char(N) 中的并没有存够 N 个字符，则会使用 0x20 填充

总结：在多字节的字符编码中，char 类型不再代表固定长度的字符串，与 varchar 并没有什么区别，底层存储时都是按照变长对待的。

# 五、探究独立表空间大小变化过程

我们接下来来观察观察`独立表空间`的空间变化：

1. 我们创建一个新的表：

```sql
MySQL [gongruiyang]> create table test(
    -> a int not null auto_increment,
    -> b varchar(7000),
    -> primary key(a)
    -> )engine=innodb;
Query OK, 0 rows affected (0.05 sec)
```

2. 看看 test 表的独立表空间大小：

```shell
gongruiyang@gongruiyang:/mnt/c/ProgramData/MySQL/MySQL Server 5.7/Data/gongruiyang$ ls -lh
total 144K
-rwxrwxrwx 1 gongruiyang gongruiyang  65 Dec 22 14:53 db.opt
-rwxrwxrwx 1 gongruiyang gongruiyang 13K Dec 23 14:59 test.frm
-rwxrwxrwx 1 gongruiyang gongruiyang 96K Dec 23 14:59 test.ibd
```

> 发现刚刚建立的表的独立表空间大小为 96K 
>
> 疑惑：不是说好的一个区的大小恒为 1M 吗？这个`test.ibd`至少也应该是 1M 呀？

3. 看看 test.ibd 中页的分配情况：

```shell
gongruiyang@gongruiyang:/mnt/e/宫瑞阳/计算机/GitHub开源项目/david-mysql-tools/py_innodb_page_type$ python py_innodb_page_info.py -v test.ibd
page offset 00000000, page type <File Space Header>
page offset 00000001, page type <Insert Buffer Bitmap>
page offset 00000002, page type <File Segment inode>
page offset 00000003, page type <B-tree Node>, page level <0000>
page offset 00000000, page type <Freshly Allocated Page>
page offset 00000000, page type <Freshly Allocated Page>
Total number of page: 6:
Freshly Allocated Page: 2
Insert Buffer Bitmap: 1
File Space Header: 1
B-tree Node: 1
File Segment inode: 1
```

> 发现只分配一个一个数据页 `page offset 00000003, page type <B-tree Node>, page level <0000>`，这里的<000>表示是叶子节点

4. 插入两条记录看看页变化情况：

```sql
MySQL [gongruiyang]> insert test select NULL,REPEAT('a', 7000);
Query OK, 1 row affected (0.02 sec)
Records: 1  Duplicates: 0  Warnings: 0

MySQL [gongruiyang]> insert into test select NULL,REPEAT('a', 7000);
Query OK, 1 row affected (0.01 sec)
Records: 1  Duplicates: 0  Warnings: 0
```

```shell
gongruiyang@gongruiyang:/mnt/e/宫瑞阳/计算机/GitHub开源项目/david-mysql-tools/py_innodb_page_type$ python py_innodb_page_info.py -v test.ibd
page offset 00000000, page type <File Space Header>
page offset 00000001, page type <Insert Buffer Bitmap>
page offset 00000002, page type <File Segment inode>
page offset 00000003, page type <B-tree Node>, page level <0000>
page offset 00000000, page type <Freshly Allocated Page>
page offset 00000000, page type <Freshly Allocated Page>
Total number of page: 6:
Freshly Allocated Page: 2
Insert Buffer Bitmap: 1
File Space Header: 1
B-tree Node: 1
File Segment inode: 1
```

> 据观察，插入两条数据后，页的分配并无任何变化，依然是只有一个数据页

5. 再插入一条记录观察页变化情况

```sql
MySQL [gongruiyang]> insert into test select NULL,REPEAT('a', 7000);
Query OK, 1 row affected (0.01 sec)
Records: 1  Duplicates: 0  Warnings: 0
MySQL [gongruiyang]> select a from test\G;
*************************** 1. row ***************************
a: 1
*************************** 2. row ***************************
a: 2
*************************** 3. row ***************************
a: 3
3 rows in set (0.00 sec)
```

```shell
gongruiyang@gongruiyang:/mnt/e/宫瑞阳/计算机/GitHub开源项目/david-mysql-tools/py_innodb_page_type$ python py_innodb_page_info.py -v test.ibd
page offset 00000000, page type <File Space Header>
page offset 00000001, page type <Insert Buffer Bitmap>
page offset 00000002, page type <File Segment inode>
page offset 00000003, page type <B-tree Node>, page level <0001>
page offset 00000004, page type <B-tree Node>, page level <0000>
page offset 00000005, page type <B-tree Node>, page level <0000>
Total number of page: 6:
Insert Buffer Bitmap: 1
File Space Header: 1
B-tree Node: 3
File Segment inode: 1
```

> <000>表示叶子节点，<001>表示非叶子节点，再插入一条新的数据后产生了 B+ 数的分裂操作
>
> 此时 B+ 树的结构如下：共有两个叶子节点<000>，一个非叶子节点<001>![image-20211223152220370](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202112231522539.png)由此可以看出，每两条记录就占据一个数据页

6. 我们一直增加到 63 条记录看看变化情况：

```sql
MySQL [gongruiyang]> delimiter $$
MySQL [gongruiyang]> create procedure load_test(count int)
    -> BEGIN
    -> declare s int default 1;
    -> declare c varchar(7000) default repeat('a', 7000);
    -> while s <= count do
    -> insert into test select NULL, c;
    -> set s = s + 1;
    -> end while;
    -> END;
    -> $$
Query OK, 0 rows affected (0.01 sec)

MySQL [gongruiyang]> delimiter ;
MySQL [gongruiyang]> call load_test(60);
Query OK, 1 row affected (0.55 sec)
MySQL [gongruiyang]> select count(*) from test\G;
*************************** 1. row ***************************
count(*): 63
1 row in set (0.00 sec)
```

使用存储过程再插入 60 条记录，此时有 63 条数据，我们再看看独立表空间变化情况：

```shell
gongruiyang@gongruiyang:/mnt/c/ProgramData/MySQL/MySQL Server 5.7/Data/gongruiyang$ ls -lh
total 656K
-rwxrwxrwx 1 gongruiyang gongruiyang   65 Dec 22 14:53 db.opt
-rwxrwxrwx 1 gongruiyang gongruiyang  13K Dec 23 14:59 test.frm
-rwxrwxrwx 1 gongruiyang gongruiyang 592K Dec 23 15:34 test.ibd

gongruiyang@gongruiyang:/mnt/e/宫瑞阳/计算机/GitHub开源项目/david-mysql-tools/py_innodb_page_type$ python py_innodb_page_info.py -v test.ibd
page offset 00000000, page type <File Space Header>
page offset 00000001, page type <Insert Buffer Bitmap>
page offset 00000002, page type <File Segment inode>
page offset 00000003, page type <B-tree Node>, page level <0001>
page offset 00000004, page type <B-tree Node>, page level <0000>
page offset 00000005, page type <B-tree Node>, page level <0000>
page offset 00000006, page type <B-tree Node>, page level <0000>
page offset 00000007, page type <B-tree Node>, page level <0000>
page offset 00000008, page type <B-tree Node>, page level <0000>
page offset 00000009, page type <B-tree Node>, page level <0000>
page offset 0000000a, page type <B-tree Node>, page level <0000>
page offset 0000000b, page type <B-tree Node>, page level <0000>
page offset 0000000c, page type <B-tree Node>, page level <0000>
page offset 0000000d, page type <B-tree Node>, page level <0000>
page offset 0000000e, page type <B-tree Node>, page level <0000>
page offset 0000000f, page type <B-tree Node>, page level <0000>
page offset 00000010, page type <B-tree Node>, page level <0000>
page offset 00000011, page type <B-tree Node>, page level <0000>
page offset 00000012, page type <B-tree Node>, page level <0000>
page offset 00000013, page type <B-tree Node>, page level <0000>
page offset 00000014, page type <B-tree Node>, page level <0000>
page offset 00000015, page type <B-tree Node>, page level <0000>
page offset 00000016, page type <B-tree Node>, page level <0000>
page offset 00000017, page type <B-tree Node>, page level <0000>
page offset 00000018, page type <B-tree Node>, page level <0000>
page offset 00000019, page type <B-tree Node>, page level <0000>
page offset 0000001a, page type <B-tree Node>, page level <0000>
page offset 0000001b, page type <B-tree Node>, page level <0000>
page offset 0000001c, page type <B-tree Node>, page level <0000>
page offset 0000001d, page type <B-tree Node>, page level <0000>
page offset 0000001e, page type <B-tree Node>, page level <0000>
page offset 0000001f, page type <B-tree Node>, page level <0000>
page offset 00000020, page type <B-tree Node>, page level <0000>
page offset 00000021, page type <B-tree Node>, page level <0000>
page offset 00000022, page type <B-tree Node>, page level <0000>
page offset 00000023, page type <B-tree Node>, page level <0000>
page offset 00000000, page type <Freshly Allocated Page>
Total number of page: 37:
Freshly Allocated Page: 1
Insert Buffer Bitmap: 1
File Space Header: 1
B-tree Node: 33
File Segment inode: 1
```

> 此时 `test.ibd`大小为 592K ，依然小于 1M ，其中 B-tree Node 一共是 33 个，包含 1 个非叶节点页和 32 个叶节点页，对于数据段来说，此时已经有了 32 个数据页了

7. 我们再插入一条数据，注意观察变化：

```sql
MySQL [gongruiyang]> insert into test select NULL,REPEAT('a', 7000);
Query OK, 1 row affected (0.03 sec)
Records: 1  Duplicates: 0  Warnings: 0
```

```shell
gongruiyang@gongruiyang:/mnt/c/ProgramData/MySQL/MySQL Server 5.7/Data/gongruiyang$ ls -lh
total 2.1M
-rwxrwxrwx 1 gongruiyang gongruiyang   65 Dec 22 14:53 db.opt
-rwxrwxrwx 1 gongruiyang gongruiyang  13K Dec 23 14:59 test.frm
-rwxrwxrwx 1 gongruiyang gongruiyang 2.0M Dec 23 15:42 test.ibd

gongruiyang@gongruiyang:/mnt/e/宫瑞阳/计算机/GitHub开源项目/david-mysql-tools/py_innodb_page_type$ python py_innodb_page_info.py  test.ibd
Total number of page: 128:
Freshly Allocated Page: 91
Insert Buffer Bitmap: 1
File Space Header: 1
B-tree Node: 34
File Segment inode: 1
```

> 此时 test 表的独立表空间文件大小已经变成了 2M 的大小，所以此时应该是申请了（32 + 64）个数据页以供使用







