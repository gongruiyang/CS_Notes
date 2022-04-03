# 一、参数文件

## 1. 参数文件作用

当数据库启动时，数据库首先会去按顺序读取`若干个配置参数文件`，这些文件当中包含：各种文件路径定义、各种数据结构的大小等

通过`mysql --help | grep my.cnf`查看 MySQL 配置文件及读取顺序

```shell
[gongruiyang@localhost ~]$ mysql --help | grep my.cnf
/etc/mysql/my.cnf /etc/my.cnf ~/.my.cnf  order of preference, my.cnf, $MYSQL_TCP_PORT,
```

MySQL 和 Oracle 是两个比较常见的数据库产品，其中 Oracle 必须读取配置文件才能正常启动，而 MySQL 可以不读取任何配置文件并能正常启动，这时，MySQL 中的各种数据结构分配的大小取决于源码中定义的默认值

## 2. 参数的定义

简单来说，我们可以把数据库参数看成一个`key - value（键值）对`，如下例：

```sql
MySQL [(none)]> show variables like 'innodb_buffer_pool_size'\G;
*************************** 1. row ***************************
Variable_name: innodb_buffer_pool_size
        Value: 8388608
1 row in set, 1 warning (0.00 sec)
MySQL [information_schema]> select * from GLOBAL_VARIABLES where VARIABLE_NAME like 'innodb_buffer_pool_size'\G
*************************** 1. row ***************************
 VARIABLE_NAME: INNODB_BUFFER_POOL_SIZE
VARIABLE_VALUE: 8388608
1 row in set, 1 warning (0.01 sec)
```

这个参数的`key`是`innodb_buffer_pool_size`，而`value`为`8388608`

查看参数及其值的方法：

1. 我们可以通过`show variables like`查看指定的参数的取值
2. 通过`information_schema 架构`下的`GLOBAL_VARIABLES 视图`来进行查找

## 3. 参数的类型

根据是否可以修改分为：

1. 动态参数：指 MySQL 实例运行中可以进行修改
2. 静态参数：在整个 MySQL 实例的生命周期内都不可以修改，故这种参数为只读参数

同一个参数，它会有两个作用范围：

1. global 范围：作用域为整个数据库
2. session 范围：作用域为当前建立的会话

```sql
MySQL [information_schema]> select @@global.read_buffer_size\G;
*************************** 1. row ***************************
@@global.read_buffer_size: 65536
1 row in set (0.00 sec)

MySQL [information_schema]> select @@session.read_buffer_size\G;
*************************** 1. row ***************************
@@session.read_buffer_size: 65536
1 row in set (0.00 sec)

MySQL [information_schema]> set read_buffer_size=524288;
Query OK, 0 rows affected (0.00 sec)

MySQL [information_schema]> select @@session.read_buffer_size\G;
*************************** 1. row ***************************
@@session.read_buffer_size: 524288
1 row in set (0.00 sec)

MySQL [information_schema]> select @@global.read_buffer_size\G;
*************************** 1. row ***************************
@@global.read_buffer_size: 65536
1 row in set (0.00 sec)
```

| 参数                     | 修改前 | 修改后 |
| ------------------------ | ------ | ------ |
| session.read_buffer_size | 65536  | 524288 |
| global.read_buffer_size  | 65536  | 65536  |

示例中仅仅是将当前会话的`read_buffer_size`修改了，如果再建立其他的会话，`read_buffer_size`依然和`global`的保持一致

## 4. 参数的修改

当前会话参数设置：

```sql
set session.VARIABLE_NAME=VARIABLE_VALUE;
```

数据库参数设置：

```sql
set global.VARIABLE_NAME=VARIABLE_VALUE;
```

若修改只读变量，会出现以下报错：

```sql
MySQL [information_schema]> set global datadir='c:db';
ERROR 1238 (HY000): Variable 'datadir' is a read only variable
```

# 二、日志文件

## 1. 日志文件的分类

MySQL 的日志文件分为以下几类：

1. error log（错误日志）
2. bin log（二进制日志）
3. slow query log（慢查询日志）
4. log（查询日志）

## 2. error log

错误日志（error log）文件对数据库的启动、运行、关闭过程进行了记录，该文件不仅仅显示错误信息，还会记录一些警告信息和正确的信息

查看 `error log` 文件所在路径：

```sql
MySQL [information_schema]> show variables like 'log_error';
+---------------+-------------------+
| Variable_name | Value             |
+---------------+-------------------+
| log_error     | .\GONGRUIYANG.err |
+---------------+-------------------+
1 row in set (0.00 sec)
```

所以，当数据库无法启动时，我们可以看看 error log 来判断问题出在了哪里。

error log 文件内容：

![image-20211222151644553](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202112221516678.png)

## 3. bin log

二进制日志（binary log）记录了所有对 MySQL 数据库执行`更改`的操作，更改的操作包括 update 、insert、delete、alter 等

### 3.1 开启 bin log 功能

1. 修改配置文件：

![image-20211222151146427](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202112221511498.png)

2. 重启 MySQL 服务
3. 连接 MySQL
4. 运行 show binary logs命令

```sql
MySQL [(none)]> show binary logs;
+----------------+-----------+
| Log_name       | File_size |
+----------------+-----------+
| log-bin.000001 |       154 |
+----------------+-----------+
1 row in set (0.00 sec)
```

### 3.2 查看 bin log 变化过程

运行两个 update 语句，观察 binlog 文件大小变化过程：

```sql
MySQL [(none)]> show binary logs;
+----------------+-----------+
| Log_name       | File_size |
+----------------+-----------+
| log-bin.000001 |       154 |
+----------------+-----------+
1 row in set (0.00 sec)

MySQL [gongruiyang]> update test set a = '1' where a = '2';
Query OK, 1 row affected (0.01 sec)
Rows matched: 1  Changed: 1  Warnings: 0

MySQL [gongruiyang]> show binary logs;
+----------------+-----------+
| Log_name       | File_size |
+----------------+-----------+
| log-bin.000001 |       434 |
+----------------+-----------+
1 row in set (0.00 sec)

MySQL [gongruiyang]> update test set a = '1' where a = '2';
Query OK, 0 rows affected (0.00 sec)
Rows matched: 0  Changed: 0  Warnings: 0

MySQL [gongruiyang]> show binary logs;
+----------------+-----------+
| Log_name       | File_size |
+----------------+-----------+
| log-bin.000001 |       434 |
+----------------+-----------+
1 row in set (0.00 sec)
```

> 1. 第一次执行 update 语句，数据发生变化时才会写入 binlog
> 2. 第二次执行 update 语句，数据没有发生变化不会写入 binlog

binary log 文件内容：

![image-20211222152216330](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202112221522405.png)

### 3.3 binlog 的作用

binary log 有以下几个作用：

1. 数据的恢复：某些数据的恢复需要二进制文件，例如：在一个数据库全备份文件恢复后，用户可以通过二进制日志恢复某个特定时刻的数据
2. 主从复制：通过复制和执行二进制文件使得一台远程 MySQL 数据库与一台 MySQL 数据库进行实时同步
3. 审计检查：用户可以通过二进制文件中的信息来进行审计，判断是否有对数据库进行注入攻击

### 3.4 binlog 文件和 index 文件

```shell
gongruiyang@gongruiyang$ ls log-bin.*
log-bin.000001  log-bin.index
```

* `log-bin.000001`就是二进制日志文件，后缀名为文件序号

* `log-bin.index`是用来管理二进制文件序号的

```shell
gongruiyang@gongruiyang$ cat log-bin.index
.\log-bin.000001
```

### 3.5 binlog 日志流转过程

当使用支持事务的存储引擎（例如：InnoDB）时，所有未提交的二进制日志都会被记录到一个缓存中去，等待事务提交后，直接将缓冲中的二进制日志写入二进制日志文件中去。

![image-20211222154024400](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202112221540475.png)

cache的大小由参数`binlog_cache_size`控制：

```sql
MySQL [gongruiyang]> show variables like 'binlog_cache_size'\G;
*************************** 1. row ***************************
Variable_name: binlog_cache_size
        Value: 32768
1 row in set, 1 warning (0.00 sec)
```

> 当一个进程开始一个事务时，会自动分配一个大小为 binlog_cache_size 的 cache 用来容纳二进制日志，当 cache 被写满后，会将 cache 中的二进制日志写入临时文件中去，故 binlog_cache_size 设置要适中
>
> ![image-20211222154655486](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202112221546553.png)



cache其他相关参数：

```sql
MySQL [gongruiyang]> show global status like 'binlog_cache%'\G;
*************************** 1. row ***************************
Variable_name: Binlog_cache_disk_use
        Value: 0
*************************** 2. row ***************************
Variable_name: Binlog_cache_use
        Value: 1
2 rows in set (0.00 sec)
```

* `Binlog_cache_disk_use`：记录使用临时文件的次数
* `Binlog_cache_use`：记录使用 cache 的次数

### 3.6 其他重要参数

`sync_binlog`：表示每向 cache 中写多少次后就同步到磁盘上，该值为 1 表示采用同步写磁盘的方式写二进制日志，这时表示不使用操作系统的 cache 来写二进制日志

```sql
MySQL [gongruiyang]> show variables like 'sync_binlog'\G;
*************************** 1. row ***************************
Variable_name: sync_binlog
        Value: 1
1 row in set, 1 warning (0.00 sec)
```

`log_slave_updates`：如果当前数据库是 slave 角色，则需要开启该参数，表示它会从 master 取得 binlog 写入自己的 binlog 文件中，进而实现主从复制

```sql
MySQL [(none)]> show variables like 'log_slave_updates'\G;
*************************** 1. row ***************************
Variable_name: log_slave_updates
        Value: OFF
1 row in set, 1 warning (0.00 sec)
```

`binlog_format`：表示记录二进制日志的格式，是动态参数，MySQL实例运行时可修改

```sql
MySQL [(none)]> show variables like 'binlog_format'\G;
*************************** 1. row ***************************
Variable_name: binlog_format
        Value: ROW
1 row in set, 1 warning (0.00 sec)
```

日志格式有三种：

1. STATEMENT 格式：记录原生 SQL 语句
2. ROW 格式：记录表的`行更改`情况，产生的日志会更大一些
3. MIXED 格式：默认情况下使用 STATEMENT 格式，一些情况下使用 ROW 格式

> 特殊情况：表引擎为 NDB、使用不确定函数 UUID() 等、使用 insert delay 语句、使用用户定义函数、使用临时表



## 4. slow query log

慢查询日志文件中记录的是：

1. 运行时间超过某阈值的所有 SQL 语句
2. 没有使用索引的 SQL 语句

慢查询日志可以存放在两个地方：

1. slow_log 文件中
2. mysql 架构下的 slow_log 表中

### 4.1 开启 slow log 功能相关参数

该文件可以帮助 DBA 来确认是否需要将某些查询语句进行优化，阈值由参数 `long_query_time` 控制

```sql
MySQL [(none)]> show variables like 'long_query_time'\G;
*************************** 1. row ***************************
Variable_name: long_query_time
        Value: 10.000000
1 row in set (0.00 sec)
```

查看是否启用慢查询日志功能参数：`slow_query_log`

```sql
MySQL [(none)]> show variables like 'slow_query_log'\G;
*************************** 1. row ***************************
Variable_name: slow_query_log
        Value: ON
1 row in set (0.00 sec)
```

是否开启记录`不走索引的 SQL 语句`参数：`log_queries_not_using_indexes`

每分钟`最大记录不走索引的 SQL 语句条数`参数：`log_throttle_queries_not_using_indexes`

```sql
MySQL [(none)]> show variables like 'log_queries_not_using_indexes'\G;
*************************** 1. row ***************************
Variable_name: log_queries_not_using_indexes
        Value: OFF
1 row in set (0.00 sec)

MySQL [(none)]> show variables like 'log_throttle_queries_not_using_indexes'\G;
*************************** 1. row ***************************
Variable_name: log_throttle_queries_not_using_indexes
        Value: 0
1 row in set (0.00 sec)
```

### 4.2 slow log 输出格式控制参数

控制 slow log 是输出到文件中还是表中参数`log_output`：FILE表示输出到文件，TABLE表示输出到表

```sql
MySQL [mysql]> show variables like 'log_output'\G;
*************************** 1. row ***************************
Variable_name: log_output
        Value: FILE
1 row in set (0.00 sec)
```

慢查询日志文件路径参数：`slow_query_log_file`

```sql
MySQL [(none)]> show variables like 'slow_query_log_file'\G;
*************************** 1. row ***************************
Variable_name: slow_query_log_file
        Value: GONGRUIYANG-slow.log
1 row in set (0.00 sec)
```

![image-20211222141423030](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202112221414246.png)

慢查询日志表：mysql 架构下 slow_log 表结构

```sql
MySQL [mysql]> show create table slow_log\G;
*************************** 1. row ***************************
       Table: slow_log
Create Table: CREATE TABLE `slow_log` (
  `start_time` timestamp(6) NOT NULL DEFAULT CURRENT_TIMESTAMP(6) ON UPDATE CURRENT_TIMESTAMP(6),
  `user_host` mediumtext NOT NULL,
  `query_time` time(6) NOT NULL,
  `lock_time` time(6) NOT NULL,
  `rows_sent` int(11) NOT NULL,
  `rows_examined` int(11) NOT NULL,
  `db` varchar(512) NOT NULL,
  `last_insert_id` int(11) NOT NULL,
  `insert_id` int(11) NOT NULL,
  `server_id` int(10) unsigned NOT NULL,
  `sql_text` mediumblob NOT NULL,
  `thread_id` bigint(21) unsigned NOT NULL
) ENGINE=CSV DEFAULT CHARSET=utf8 COMMENT='Slow log'
1 row in set (0.00 sec)
```

> 通过观察发现 slow_log 表是 `CSV引擎`



### 4.3 使用 mysqldumpslow 命令统计时间最长的 N 条语句

查询运行时间最长的 10 条 SQL ：

```shell
mysqldumpslow -s al -n 10 GONGRUIYANG-slow.log
```

`mysqldumpslow` 参数含义：

```shell
-s sort_type
    How to sort the output. The value of sort_type should be chosen from the following list:
    ·   t, at: Sort by query time or average query time
    ·   l, al: Sort by lock time or average lock time
    ·   s, as: Sort by rows sent or average rows sent
    ·   c: Sort by count
-n NUM
    Abstract numbers with at least N digits within names
```



## 5. query log

查询日志将所有对 MySQL 数据库的请求信息都记录了下来，包括 Access Denied 信息和未能正确执行的 SQL 语句

查看 query log 路径：

```sql
MySQL [gongruiyang]> show variables like 'general_log_file'\G;
*************************** 1. row ***************************
Variable_name: general_log_file
        Value: GONGRUIYANG.log
1 row in set, 1 warning (0.00 sec)

```

query log 文件内容：

![image-20211222151802902](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202112221518008.png)



# 三、socket 文件

在 Unix 系统下本地连接 MySQL 可以采用 Unix 域套接字方式，这种连接需要一个套接字文件。

查看套接字文件路径：

```sql
MySQL [information_schema]> show variables like 'socket';
+---------------+-------+
| Variable_name | Value |
+---------------+-------+
| socket        | MySQL |
+---------------+-------+
1 row in set (0.00 sec)
```



# 四、pid 文件

当 MySQL 实例启动时，会将自己的进程 ID 写入一个文件，这个文件就是 pid 文件。

查看 pid 文件路径：

```sql
MySQL [information_schema]> show variables like 'pid_file';
+---------------+------------------------------------------------------------+
| Variable_name | Value                                                      |
+---------------+------------------------------------------------------------+
| pid_file      | C:\ProgramData\MySQL\MySQL Server 5.7\Data\gongruiyang.pid |
+---------------+------------------------------------------------------------+
1 row in set (0.00 sec)
```

pid 文件内容：

```shell
gongruiyang@gongruiyang$ cat gongruiyang.pid
36740
```



# 五、MySQL 表结构文件

无论采用哪种存储引擎，MySQL 都会给每一个表和视图产生一个以 `.frm`为后缀名的文件，用以记录表的结构定义

```shell
[root@localhost performance_schema]# ls *.frm
cond_instances.frm
events_waits_current.frm
events_waits_history.frm
events_waits_history_long.frm
events_waits_summary_by_instance.frm
events_waits_summary_by_thread_by_event_name.frm
events_waits_summary_global_by_event_name.frm
file_instances.frm
file_summary_by_event_name.frm
file_summary_by_instance.frm
mutex_instances.frm
performance_timers.frm
rwlock_instances.frm
setup_consumers.frm
setup_instruments.frm
setup_timers.frm
threads.frm
```



# 六、InnoDB 存储引擎文件

## 1. 表空间文件

InnoDB 将存储的数据按照表空间进行存放的设计。

在默认情况下会有一个名为 `ibdata1` 的文件，该文件就是默认的表空间文件（tablespace file），用户可以通过参数设置：

```sql
MySQL [(none)]> show variables like 'innodb_data_file_path'\G;
*************************** 1. row ***************************
Variable_name: innodb_data_file_path
        Value: ibdata1:12M:autoextend
1 row in set, 1 warning (0.00 sec)
```

* 文件大小为 12 M，并且会自动增长

所有基于 InnoDB 存储引擎的`表中的数据`都会被记录到该文件中去，如果设置了参数`innodb_file_per_table`，则会将每个基于 InnoDB 存储引擎的表产生一个`独立表空间`，独立表空间的命名规则为：`TABLE_NAME.ibd`

```sql
MySQL [(none)]> show variables like 'innodb_file_per_table'\G;
*************************** 1. row ***************************
Variable_name: innodb_file_per_table
        Value: ON
1 row in set, 1 warning (0.00 sec)
```

![image-20211222165848265](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202112221658365.png)

## 2. redo log 文件

### 2.1 重做日志的定义及作用

在 InnoDB 的 Data 目录下有两个文件：

```shell
gongruiyang@gongruiyang$ ll ib_log*
-rwxrwxrwx 1 gongruiyang gongruiyang 50331648 Dec 22 15:06 ib_logfile0
-rwxrwxrwx 1 gongruiyang gongruiyang 50331648 Sep 15 15:20 ib_logfile1
```

这两个文件定义为 重做日志文件（redo log file），该文件用于记录 InnoDB 存储引擎的事务日志，例如：数据库在执行事务时，发生宕机，InnoDB 存储引擎就会使用重做日志来恢复到宕机前时刻，以此来保证数据的完整性。

每个 InnoDB 存储引擎至少有 1 个重做日志文件组，每个重做日志文件组下至少有 2 个重做日志文件，如默认的 ib_logfile0 和 ib_logfile1

日志写入顺序：InnoDB 存储引擎先写重做日志文件 1 ，当达到文件最后时，会切换到重做日志文件 2，当达到文件最后时，再切换到重做日志文件1，写入的日志覆盖不需要的日志

### 2.2 影响重做日志的参数

```sql
MySQL [(none)]> show variables like 'innodb_log_group_home_dir'\G;
*************************** 1. row ***************************
Variable_name: innodb_log_group_home_dir
        Value: .\
1 row in set, 1 warning (0.01 sec)

MySQL [(none)]> show variables like 'innodb_log_file_size'\G;
*************************** 1. row ***************************
Variable_name: innodb_log_file_size
        Value: 50331648
1 row in set, 1 warning (0.00 sec)

MySQL [(none)]> show variables like 'innodb_log_files_in_group'\G;
*************************** 1. row ***************************
Variable_name: innodb_log_files_in_group
        Value: 2
1 row in set, 1 warning (0.01 sec)
```

* `innodb_log_group_home_dir`：日志文件所在路径
* `innodb_log_file_size`：每个重做日志文件的大小
* `innodb_log_files_in_group`：日志文件组中重做日志文件的数量

### 2.3 redo log 的写入过程

redo log 的写入过程分为以下两部分：

1. 内存中产生的 redo log 先写入 redo log buffer 中
2. 当一定条件触发后，从 redo log buffer 中向 redo log file 中以每次 512KB 的大小原子性的写入

`触发条件`：① Master Thread 每 1 秒向磁盘中写一次 redo log 日志 ；② 事务提交时向磁盘中写 redo log 日志

![image-20211222174215464](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202112221742557.png)

第二个触发条件由参数 `innodb_flush_log_at_trx_commit`控制：

```sql
MySQL [(none)]> show variables like 'innodb_flush_log_at_trx_commit'\G;
*************************** 1. row ***************************
Variable_name: innodb_flush_log_at_trx_commit
        Value: 1
1 row in set, 1 warning (0.01 sec)
```

* 值为  0：提交事务时，不写 redo log
* 值为 1：同步写到磁盘上，伴随 fsync 的调用，保证数据一致性程度最高，保证持久性
* 值为 2：异步写到文件系统缓存中

其中 0 和 2 都有可能会导致事务数据的丢失



