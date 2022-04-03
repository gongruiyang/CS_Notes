# 基础IO

## 1. C语言函数操作文件

| C库IO函数                                                    | 功能描述                                             |
| ------------------------------------------------------------ | ---------------------------------------------------- |
| FILE  *fopen(const  char *path, const char *mode);           | 通过一个文件流指针打开一个流并与文件相关联           |
| size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream); | 将ptr指向的数据写入流指针指向的流中                  |
| size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream); | 从流指针指向的流中读取数据保存在ptr指向的位置        |
| int fseek(FILE *stream, long int  offset,  int whence);      | 重新定位文件流指针从whence为起始位置偏移offset个字符 |
| int fclose(FILE *stream);                                    | 关闭文件流指针指向的流                               |

## 2. 系统调用函数操作文件

| 系统调用函数                                           | 功能描述                                                   |
| ------------------------------------------------------ | ---------------------------------------------------------- |
| int open(const char *pathname, int flags, mode_t mode) | 将文件名转换为一个新建的文件描述符，通过文件描述符操作文件 |
| ssize_t read(int fd, void *buf, size_t count);         | 将文件中的内容读取到指定空间中保存                         |
| ssize_t write(int fd, const void *buf, size_t count);  | 将buf中的内容写入文件中                                    |
| off_t lseek(int fd, off_t offset, int whence);         | 将与文件描述符相关联的文件的偏移量重新定位到参数偏移量处   |
| int close(int fd);                                     | 关闭一个文件描述符                                         |

### 2.1 open函数

```cpp
int open(const char *pathname, int flags, mode_t mode)
```

> 功能：将文件名转换为一个新建的文件描述符，通过文件描述符操作文件
>
> 头文件：
>
> * fcntl.h
>
> 参数：
>
> * pathname:要打开的文件名称（路径+名称）
>
> * flags : 以某种方式打开，在源码中以宏定义
>
>   | 必选宏（选择其中一个） | 含义         |
>   | ---------------------- | ------------ |
>   | O_RDONLY               | 只读方式打开 |
>   | O_WRONLY               | 只写方式打开 |
>   | O_RDWR                 | 读写方式打开 |
>
>   | 可选宏（非必选项  ） | 含义             |
>   | -------------------- | ---------------- |
>   | O_APPEND             | 追加             |
>   | O_TRUNC              | 截断             |
>   | O_CREAT              | 文件不存在则创建 |
>
>   其中必选宏与可选宏之间使用按位或的方式连接，例如 O_RDONLY | O_CREAT
>
> * mode ： 给文件设置权限，传入8进制数字
>
> 返回值：
>
> * -1 打开失败
> * \>= 0 唯一标识文件的文件描述符





---



> Q : 为什么宏之间要按位或？

源码中的宏定义：定义位于/usr/include/bits/fcntl-linux.h文件中

其中数字都是8进制

```cpp
#define O_RDONLY             00
#define O_WRONLY             01
#define O_RDWR               02
#define O_CREAT            0100
```

| 宏       | 8进制 | 2进制                               |
| -------- | ----- | ----------------------------------- |
| O_RDONLY | 00    | 00000000 00000000 00000000 00000000 |
| O_WRONLY | 01    | 00000000 00000000 00000000 00000001 |
| O_RDWR   | 02    | 00000000 00000000 00000000 00000010 |
| O_CREAT  | 0100  | 00000000 00000000 00000000 01000000 |

>O_RDWR | O_CREAT
>
>![image-20201231110943713](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20201231110943713.png)

### 2.2 read函数

```cpp
ssize_t read(int fd, void *buf, size_t count);
```

> 
>
> 功能：将文件中的内容读取到指定空间中保存
>
> 头文件：
>
> * unistd.h
>
> 参数：
>
> * fd : 文件描述符，open函数的返回值
> * buf : 将文件中的内容读到buf中
> * coount : 最大可以读多少个
>
> 返回值：返回读取到的字节数量







### 2.3 write函数

```cpp
 ssize_t write(int fd, const void *buf, size_t count);
```

> 功能：将buf中的内容写入文件中
>
> 头文件：
>
> * unistd.h
>
> 参数：
>
> * fd : 文件描述符，open函数的返回值
> * buf : 要写入文件的内容
> * count : 要写入文件内容的大小
>
> 返回值：写成功的字节数量



### 2.4 lseek函数

```cpp
off_t lseek(int fd, off_t offset, int whence);
```

> 功能：将与文件描述符相关联的文件的偏移量重新定位到参数偏移量处
>
> 头文件：
>
> * unistd.h
>
> 参数：
>
> * fd : 文件描述符，open函数的返回值
> * offset  : 偏移量
> * whence : 偏移起始位置
>
> | 宏       | 含义             |
> | -------- | ---------------- |
> | SEEK_SET | 文件内容起始位置 |
> | SEEK_CUR | 当前位置         |
> | SEEK_END | 文件内容结束为止 |
>
> 返回值：偏移量



### 2.5 close函数

```cpp
int close(int fd);
```

> 功能：关闭一个文件描述符
>
> 头文件：
>
> * unistd.h
>
> 参数：
>
> * fd : 文件描述符，open函数的返回值
>
> 返回值：
>
> * 0 表示关闭成功
> * -1 表示有错误发生

### 2.6 测试程序

```cpp
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
int main()
{
  //1.测试open函数：打开文件并创建文件描述符
  int fd = open("./testfile",O_RDWR | O_CREAT, 0664);
  if(fd < 0)
    perror("open");
  else
  {
    printf("1.创建文件成功，文件描述符为fd = %d\n",fd);
  }

  //2.测试write函数：向文件中写入数据
  const char* w_buf = "Hello World!";
  int w_ret = write(fd,w_buf,strlen(w_buf));
  if(!w_ret)
    perror("write");                                                        
  else
    printf("2.写入成功，写入了%d个字节的数据\n",w_ret);

  //3.测试lseek函数：重定位
  int offset = 0;
  int l_ret = lseek(fd,offset,SEEK_SET);
  printf("3.偏移量为%d\n",l_ret);

  //4.测试read函数：从文件中读取数据
  char r_buf[1024] = { 0 };
  int r_ret = read(fd,r_buf,sizeof(r_buf) - 1);
  if(!r_ret)
    perror("read");
  else
    printf("4.读取成功，读取了%d个字节的数据，读取内容为%s\n",r_ret,r_buf);

  //5.测试close函数：关闭文件描述符
  int c_ret = close(fd);
  if(!c_ret)
    printf("5.文件描述符%d关闭成功\n",fd);
  else
    perror("close");

  return 0;
}
```

```shell
[gongruiyang@localhost TestSysFile]$ gcc test.c -o test
[gongruiyang@localhost TestSysFile]$ ./test 
1.创建文件成功，文件描述符为fd = 3
2.写入成功，写入了12个字节的数据
3.偏移量为0
4.读取成功，读取了12个字节的数据，读取内容为Hello World!
5.文件描述符3关闭成功
[gongruiyang@localhost TestSysFile]$ ls
test  test.c  testfile
[gongruiyang@localhost TestSysFile]$ ll
总用量 20
-rwxrwxr-x. 1 gongruiyang gongruiyang 8872 12月 31 11:56 test
-rw-rw-r--. 1 gongruiyang gongruiyang 1159 12月 31 11:56 test.c
-rw-rw-r--. 1 gongruiyang gongruiyang   12 12月 31 11:56 testfile
```



## 3. 文件描述符

### 3.1 查看文件描述符

> Q：如何找到已经打开的文件描述符呢？
>
> A：
>
> 1. 在关闭文件描述符之前使用while循环，不让程序关闭文件描述符
>
> 2. 使用ps查看打开文件描述符的进程号
> 3. 使用cd /proc/进程号/fd 进入文件描述符文件夹

例如：查看进程号为6606打开的文件描述符

```shell
[gongruiyang@localhost TestSysFile]$ ps -aux | grep test
gongrui+   6606  0.0  0.0   4216   348 pts/0    S+   12:02   0:00 ./test
gongrui+   6686  0.0  0.0 112828   976 pts/1    R+   12:02   0:00 grep --color=auto test
[gongruiyang@localhost TestSysFile]$ cd /proc/6606/fd
gongruiyang@localhost fd]$ ll
总用量 0
lrwx------. 1 gongruiyang gongruiyang 64 12月 31 12:08 0 -> /dev/pts/0
lrwx------. 1 gongruiyang gongruiyang 64 12月 31 12:08 1 -> /dev/pts/0
lrwx------. 1 gongruiyang gongruiyang 64 12月 31 12:02 2 -> /dev/pts/0
lrwx------. 1 gongruiyang gongruiyang 64 12月 31 12:08 3 -> /home/gongruiyang/ClassLinunx/TestSysFile/testfile
```

> 1. 操作系统会为每一个进程在磁盘当中创建一个以进程号命名的文件夹，在该文件夹中有一个fd文件夹，保存的信息为该进程打开的文件描述符信息。
> 2. 当我们新创建出来一个新的进程，一定会打开3个文件描述符，分别对应 **标准输入（0） 标准输出（1）  标准错误（2）**



---

### 3.2 PCB与文件描述符的关系

> PCB的task_struct与文件描述符关系源码与图解：

**task_struct部分源码**

```cpp
struct task_struct {
..........
/* open file information */
	struct files_struct *files;
..........
};
```

**files_struct部分源码**

```cpp
struct files_struct {
  /*
   * read mostly part
   */
	atomic_t count;
	struct fdtable *fdt;
	struct fdtable fdtab;
  /*
   * written part on a separate cache line in SMP
   */
	spinlock_t file_lock ____cacheline_aligned_in_smp;
	int next_fd;
	struct embedded_fd_set close_on_exec_init;
	struct embedded_fd_set open_fds_init;
	struct file * fd_array[NR_OPEN_DEFAULT];
};
```



**图解：**

![image-20201231151018766](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20201231151018766.png)

> 由源码可以看出：
>
> 1. 每一个进程的task_struct结构体中都包含一个指向files_struct结构体的指针，files_struct结构体中有一个fd_array数组，该数组中保存的都是指向file结构体的指针，每一个file结构体对应一个文件
> 2. **文件描述符其实就是内核当中fd_array数组的下标**
> 3. **fd_array数组中前三个数据元素对应的就是：标准输入 标准输出 标准错误**

**结论：每创建一个新进程，系统默认会打开三个文件描述符：标准输入 标准输出 标准错误**

### 3.3 分配文件描述符规则

> 规则：最小未占用原则
>
> 意思就是将从0开始往上寻找第一个未被占用的文件描述符分配给正在打开的文件

### 3.4 文件描述符泄漏问题

> 文件描述符也叫文件句柄
>
> 当我们打开一个文件，操作系统就会文件分配一个文件描述符，如果在使用完毕之前，没有及时的关闭文件，就会造成文件句柄泄漏的问题



> Q : 一个进程当中最大打开的文件数量是多少？
>
> A : 使用ulimit命令

```shell
[gongruiyang@localhost ClassLinunx]$ ulimit -a
................
open files                      (-n) 1024
................
```

> 可在输出打印中找到open files最大数为1024
>
> 该最大值是可以修改的



演示代码

```cpp
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
  int count = 0;

  while(1)
  {
    int fd = open("testfile",O_RDWR | O_CREAT, 0664);
    if(fd < 0)
    {
      perror("open");
      break;                                         
    }
    else
    {
      printf("fd:%d\n",fd);
      count++;
    }
      
  }
  printf("count = %d\n",count);
    
  return 0;
}

```

```
fd:1023
open: Too many open files
count = 1021
```



## 4.文件描述符 和 文件流指针 的对比

> 文件流指针：fopen函数返回的，文件流指针是由C库维护的
>
> 文件描述符：open函数返回的，文件描述符是由操作系统维护的



文件流指针源码：源文件路径/usr/include/stdio.h

```cpp
typedef struct _IO_FILE FILE;
```

> 由此可以看出FILE是\_IO_FILE的别名，我们再来看看\_IO_FILE的部分源码：源文件路径：/usr/include/libio.h

```cpp
struct _IO_FILE {
    ...............
    char* _IO_read_ptr; /* Current read pointer */
    char* _IO_read_end; /* End of get area. */
    char* _IO_read_base;  /* Start of putback+get area. */

    char* _IO_write_base; /* Start of put area. */
    char* _IO_write_ptr;  /* Current put pointer. */
    char* _IO_write_end;  /* End of put area. */
    
    int _fileno;
    ...............
};
```

> 由\_IO_FILE结构体源码可以了解到，**读写缓冲区分别使用了3个char型指针**，并且**用_fileno保存文件描述符**
>
> ![image-20201231160151730](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20201231160151730.png)

**总结：文件流指针将文件描述符进行了封装**



> 针对文件流指针而言的缓冲区，是C库维护的
>
> exit函数在退出线程的时候，会刷新缓冲区，原因是操作的是文件流指针
>
> \_exit哈数在退出线程的时候，不会刷新缓冲区，原因是_exit是系统调用，内核无法涉及到C库维护的缓冲区，所以不会刷新



## 5.重定向

### 5.1 清空重定向 >

> 将输出位置调整到文件中，每一次从终端向文件输入数据前都会将先前的内容**清空**

```shell
[gongruiyang@localhost TestDup]$ echo "123"
123
[gongruiyang@localhost TestDup]$ echo "123" > testfile
[gongruiyang@localhost TestDup]$ cat testfile 
123
[gongruiyang@localhost TestDup]$ echo "456" > testfile 
[gongruiyang@localhost TestDup]$ cat testfile 
456
```

### 5.2 追加重定向 >>

> 将输出位置调整到文件中，每一次从终端向文件输入数据都会被**追加**在源文件内容之后

```shell
[gongruiyang@localhost TestDup]$ echo "123" >> testfile
[gongruiyang@localhost TestDup]$ cat testfile 
123
[gongruiyang@localhost TestDup]$ echo "456" >> testfile 
[gongruiyang@localhost TestDup]$ cat testfile 
123
456
```

### 5.3 重定向原理

> echo本来是要将数据通过终端进程的 标准输出[1] 将数据打印输出，经过重定向后，1号文件描述符中指向file结构体的指针从原来指向的文件改变指向重定向后的文件



> 重定向接口：

```cpp
int dup2(int oldfd, int newfd);
```

> 功能：dup2() makes newfd be the copy of oldfd, closing newfd first if necessary
>
> 头文件：
>
> * unistd.h
>
> 参数：
>
> * oldfd :  
> * newfd : 
>
> 返回值：重定向成功返回newfd，失败返回-1



演示程序：

```cpp
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
  int fd = open("./testfile",O_RDWR | O_CREAT,06664);
  if(fd < 0)
    perror("open");
  else
  {
     //将标准输出重定向到文件中去
    dup2(fd,1);
  }
  printf("test message!\n");                          

  return 0;
}

```

```shell
[gongruiyang@localhost TestDup]$ gcc test.c -o test
[gongruiyang@localhost TestDup]$ ./test 
[gongruiyang@localhost TestDup]$ cat testfile 
test message!
```



## 6. ext2文件系统

**ext2文件系统**

![image-20201231204955673](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20201231204955673.png)

> * Block Bitmap : 本质是一个位图，每一个比特位表示Data blocks中数据块的使用情况，若比特位为1，则表示占用，若比特位为0，表示未被占用
> * inode Bitmap : 本质是一个位图，每一个比特位表示inode table当中inode块的使用情况，若比特位为1，则表示占用，若比特位为0，表示未被占用
> * Data blocks : 实际存储文件的区域，在这个区域中，将磁盘分成了不同的小block
> * inode Table : inode结点的集合，inode结点描述了文件的存储情况（文件在哪些Block块中存储的）



> 文件存储过程：
>
> 1. 去Block Bit Map区域查找空闲的block块，将文件存储在空闲的block块当中
> 2. 通过inode BitMap获取空闲的inode节点，通过inode节点去描述文件在Data Block区域当中存储的位置
> 3. inode+文件名称作为目录的目录项被保存下来

**存储原理：分散存储，相比于线性存储，减少了许多磁盘碎片**

![image-20201231183148799](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20201231183148799.png)

> 文件的获取过程：
>
> 1. 通过文件名+inode节点号找到inode对应的文件信息
> 2. 在Data blocks区域当中获取当前文件存储的内容，在进行拼接，拼接完成之后就是文件内容了



## 7. 软链接 和 硬链接

### 7.1 软链接

> 软链接文件就相当于一个文件的快捷方式
>
> 特点：
>
> 1. 软链接文件具有独立的inode节点号
>
> 2. 删除源文件时，应当将软链接到该文件的软链接文件一同删除，否则会出现如下情况
>
>    ![image-20201231233330287](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20201231233330287.png)



创建软链接命令：

```shell
ln -s 原文件名 软链接文件名
```

例如：

```shell
[gongruiyang@localhost TestLink]$ ln -s sourceFile softLinkFile
[gongruiyang@localhost TestLink]$ ll
总用量 0
lrwxrwxrwx. 1 gongruiyang gongruiyang 10 12月 31 23:28 softLinkFile -> sourceFile
-rw-rw-r--. 1 gongruiyang gongruiyang  0 12月 31 23:25 sourceFile
```



### 7.2 硬链接

> 硬链接文件就相当于源文件的一个副本，除了文件名外，其他一模一样



创建硬链接命令：

```shell
ln  原文件名 硬链接文件名
```

例如：

```shell
[gongruiyang@localhost TestLink]$ ln sourceFile hardLinkFile
[gongruiyang@localhost TestLink]$ ll
总用量 0
-rw-rw-r--. 2 gongruiyang gongruiyang 0 12月 31 23:38 hardLinkFile
-rw-rw-r--. 2 gongruiyang gongruiyang 0 12月 31 23:38 sourceFile
```



### 7.3 inode对比

```shell
[gongruiyang@localhost TestLink]$ ls -li
总用量 0
33554820 -rw-rw-r--. 2 gongruiyang gongruiyang  0 12月 31 23:38 hardLinkFile
33554821 lrwxrwxrwx. 1 gongruiyang gongruiyang 10 12月 31 23:43 softLinkFile -> sourceFile
33554820 -rw-rw-r--. 2 gongruiyang gongruiyang  0 12月 31 23:38 sourceFile
```

> 由上述命令输出结果可知：
>
> inode[sourceFile] = 33554820
>
> inode[softLinkFile] = 33554821
>
> inode[hardLinkFile] = 33554820

**总结：软链接文件的inode与源文件不同，硬链接文件的inode与源文件相同**







