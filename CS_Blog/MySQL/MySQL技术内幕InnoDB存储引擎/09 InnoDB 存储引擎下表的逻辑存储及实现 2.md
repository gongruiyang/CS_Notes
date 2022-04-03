# 一、InnoDB 数据页存储结构

innodb 存储引擎数据页结构如图所示：

![image-20211225133938277](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202112251340734.png)

InnoDB 数据页由 7 个部分组成：

* File Header ：文件头
* Page Header：页头
* Infimun 和 Supremum Records
* User Records：用户记录，即行记录
* Free Space：空闲空间
* Page Directory：页目录
* File Trailer：文件结尾信息

## 1. File Header

`File Header`用来记录页的一些头信息，共占用 38 个字节，具体记录了哪些信息如下表所示：

| 名称                             | 大小 | 说明                                                         |
| -------------------------------- | ---- | ------------------------------------------------------------ |
| FIL_PAGE_SPACE_OR_CHKSUM         | 4    | MySQL 版本 4. 0 之前该值为 0，版本 4.0 之后该值代表页的 checksum 值 |
| FIL_PAGE_OFFSET                  | 4    | 表空间中页的偏移量，表示当前页在所有页中的偏移量，例如（10,1）表示查找表空间 ID 为 10 的第二个页 |
| FIL_PAGE_PREV                    | 4    | 当前页的上一个页，B+ 树的叶子节点之间链接形成双向链表        |
| FIL_PAGE_NEXT                    | 4    | 当前页的下一个页，B+ 树的叶子节点之间链接形成双向链表        |
| FIL_PAGE_LSN                     | 8    | 代表当前页最后被修改的日志序列位置                           |
| FIL_PAGE_TYPE                    | 2    | InnoDB 存储引擎页的类型                                      |
| FIL_PAGE_FILE_FLUSH_LSN          | 8    | 该值仅仅在 系统表空间 的一个页中定义，对于独立表空间该值为0，含义是文件至少被更新到了该 LSN 值 |
| FIL_PAGE_ARCH_LOG_NO_OR_SPACE_ID | 4    | MySQL 版本 4.1 之后，该值表示页属于哪个表空间                |

* `FIL_PAGE_TYPE`的取值和含义如下表所示：

```c
/** File page types (values of FIL_PAGE_TYPE) @{ */
#define FIL_PAGE_INDEX		17855	/*!< B-tree node */
#define FIL_PAGE_RTREE		17854	/*!< B-tree node */
#define FIL_PAGE_UNDO_LOG	2	/*!< Undo log page */
#define FIL_PAGE_INODE		3	/*!< Index node */
#define FIL_PAGE_IBUF_FREE_LIST	4	/*!< Insert buffer free list */
/* File page types introduced in MySQL/InnoDB 5.1.7 */
#define FIL_PAGE_TYPE_ALLOCATED	0	/*!< Freshly allocated page */
#define FIL_PAGE_IBUF_BITMAP	5	/*!< Insert buffer bitmap */
#define FIL_PAGE_TYPE_SYS	6	/*!< System page */
#define FIL_PAGE_TYPE_TRX_SYS	7	/*!< Transaction system data */
#define FIL_PAGE_TYPE_FSP_HDR	8	/*!< File space header */
#define FIL_PAGE_TYPE_XDES	9	/*!< Extent descriptor page */
#define FIL_PAGE_TYPE_BLOB	10	/*!< Uncompressed BLOB page */
#define FIL_PAGE_TYPE_ZBLOB	11	/*!< First compressed BLOB page */
#define FIL_PAGE_TYPE_ZBLOB2	12	/*!< Subsequent compressed BLOB page */
#define FIL_PAGE_TYPE_UNKNOWN	13	/*!< In old tablespaces, garbage in FIL_PAGE_TYPE is replaced with this value when flushing pages. */
#define FIL_PAGE_COMPRESSED	14	/*!< Compressed page */
#define FIL_PAGE_ENCRYPTED	15	/*!< Encrypted page */
#define FIL_PAGE_COMPRESSED_AND_ENCRYPTED 16/*!< Compressed and Encrypted page */
#define FIL_PAGE_ENCRYPTED_RTREE 17	/*!< Encrypted R-tree page */
```

| 常量名称                | 十六进制 | 含义                      |
| ----------------------- | -------- | ------------------------- |
| FIL_PAGE_INDEX          | 0x45BF   | B+ 数叶子节点（数据节点） |
| FIL_PAGE_UNDO_LOG       | 0x0002   | undo log 页               |
| FIL_PAGE_INODE          | 0x0003   | 索引节点                  |
| FIL_PAGE_IBUF_FREE_LIST | 0x0004   | insert buffer 空闲列表页  |
| FIL_PAGE_TYPE_ALLOCATED | 0x0000   | 最新分配还未使用的页      |
| FIL_PAGE_IBUF_BITMAP    | 0x0005   | insert buffer 位图页      |
| FIL_PAGE_TYPE_SYS       | 0x0006   | 系统页                    |
| FIL_PAGE_TYPE_TRX_SYS   | 0x0007   | 事务系统数据页            |
| FIL_PAGE_TYPE_FSP_HDR   | 0x0008   | File Space Header 页      |
| FIL_PAGE_TYPE_XDES      | 0x0009   | 扩展描述页                |
| FIL_PAGE_TYPE_BLOB      | 0x000A   | BLOB 页                   |

## 2. Page Header

`Page Header` 用来记录数据页的状态信息，共占用 56 字节，具体信息如下表所示：

| 名称              | 大小（字节） | 说明                                                         |
| ----------------- | ------------ | ------------------------------------------------------------ |
| PAGE_N_DIR_SLOTS  | 2            | 表示页目录（Page Directory）中的槽（Slot）数量               |
| PAGE_HEAP_TOP     | 2            | 记录在页中是以堆的形式存放的，该值存放的是指向第一个记录的指针 |
| PAGE_N_HEAP       | 2            | 堆中的记录数，一共占用 2 个字节                              |
| PAGE_FREE         | 2            | 指向可重用空间的首指针                                       |
| PAGE_GARBAGE      | 2            | 行记录中 delete flag 为 1 的总字节数，即被删除的记录的大小总和 |
| PAGE_LAST_INSERT  | 2            | 最后插入记录的位置                                           |
| PAGE_DIRECTION    | 2            | 最后插入的方向                                               |
| PAGE_N_DIRECTION  | 2            | 一个方向连续插入记录的数量                                   |
| PAGE_N_RECS       | 2            | 该页中记录的数量                                             |
| PAGE_MAX_TRX_ID   | 8            | 修改当前页的最大事务ID，注意该值仅在 Secondary Index 中定义  |
| PAGE_LEVEL        | 2            | 当前页在索引树中的位置，0x00表示叶节点                       |
| PAGE_INDEX_ID     | 8            | 索引ID，表示当前页属于哪个索引                               |
| PAGE_BTR_SEG_LEAF | 10           | B+ 树数据页非叶节点所在段的 segment header                   |
| PAGE_BTR_SEG_TOP  | 10           | B+ 树数据页所在段的 segment header                           |

## 3. Infimun Records 和 Supremum Records

`Infimun 记录`是比该页中任何主键值都要小的值，`Supremum 记录`是比任何可能大的值还要大的值，这两个值在页创建时被建立，并且在任何情况下都不会被删除。这两个记录位于页中的所有航记录数据的收尾处，如图所示：

![image-20211225152015796](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202112251520879.png)

## 4. User Records

`User Records`实际存储行记录的内容，如图：

![image-20211225152951366](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202112251529446.png)

## 5. Free Space

`Free Space`指的是空闲空间，同样也是使用链表来进行组织的，在一条记录被删除后，该空间会被自动加入到空闲列表中

## 6. Page Directory

`Page Directory`顾名思义是这个页的`所有记录的一个目录`，存放着`数量可变`的`记录指针`，这些指针指向页中存放的记录，有时`记录指针`被称为`槽`，所以`Page Directorty`有时被称为`目录槽`，在InnoDB页面中并不是一个记录指针对应一个记录，而是`若干个（1~8）`记录对应一个记录指针，在完整的页面中，每六条记录将有一个槽。

记录的头信息中有一个`大小为4位的n_owned`，表示管理当前记录的槽一共管理了多少个记录。

举例：如果一个页中的记录是‘A’、‘B’、‘F’、‘D’，记录指针将是（pointer to 'A'）、（pointer to 'B'）、（pointer to 'D'）、（pointer to 'F'）

![image-20211226131936925](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202112261319222.png)

由举例可知，指针的排列顺序是按照`键值的顺序`排列的，故在查找某一条记录时，可以通过`二分查找`的方式进行查找

**所以查找一条记录的过程为：先在 B+ 树上定位到记录所在的叶子节点（数据页），然后将整个数据页载入到内存中，然后在目录槽中进行二分查找目标记录所在的槽，然后在槽所管理的若干条记录中寻找目标记录**

## 7. File Trailer

为了检测页向磁盘中写入的过程中是否产生损坏，在 InnoDB 存储引擎页中设置了 File Trailer 来做检验。

File Trailer 组成：只有一个 `FIL_PAGE_END_LSN` 部分，占用 8 字节，前 4 字节代表此页的 checksum 值，最后 4 字节和 File Header 中的 FIL_PAGE_LSN 相同

![image-20211226140515503](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202112261405663.png)

检查原理：将 8 字节的 File Trailer 与 File Header 中的 FIL_PAGE_SPACE_OR_CHKSUM 和 FIL_PAGE_LSN 组成的 8 字节作比较（并不是简单的等值比较，而是需要通过检查 checksum 的算法）。

检查 checksum 的算法：通过参数`innodb_checksum_algorithm` 来控制，可设置的算法有：crc32、none、strict_innodb、strict_crc32、strict_none，其中strict_*的算法表示严格按照设置的算法进行页的检测

```sql
MySQL [(none)]> show variables like 'innodb_checksum_algorithm'\G;
*************************** 1. row ***************************
Variable_name: innodb_checksum_algorithm
        Value: crc32
1 row in set, 1 warning (0.08 sec)
```

是否开启对页的完整性检查由参数`innodb_checksums`控制：

```sql
MySQL [(none)]> show variables like 'innodb_checksums'\G;
*************************** 1. row ***************************
Variable_name: innodb_checksums
        Value: ON
1 row in set, 1 warning (0.00 sec)
```



