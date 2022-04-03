

# Linux下进程相关操作

## 1. 进程概念

### 1.1 进程基本概念

> **进程定义**：一个具有一定独立功能的**程序**在一个**数据**集合上的一次**动态执行过程**。
>
> 简言之：**进程是程序的一次执行过程**



> **进程特性**：
>
> 1. 动态性：进程是**动态产生**的，往往都会经历**创建、运行、消亡**三个状态
> 2. 独立性：各个进程之间的**地址空间**相互**独立**
> 3. 并发性：任何进程都可以**一起**向前推进
> 4. 异步性：每个进程都以其**不可预知的速度**向前推进
> 5. 结构化：**进程 = 代码段 + 数据段 + PCB（进程控制块）**



> **进程与程序的区别**：
>
> 1. 进程是**动态的执行过程**；程序是**静态的代码**
> 2. 进程是**暂时**的，运行在**内存**中的一个状态变化的过程；程序是**永久**的，保存在**外存**中
> 3. 通过多次执行一个程序，进而产生多个进程，所以一个程序可以对应多个进程；
> 4. 进程通过调用多个程序，进而一个进程可以执行多个程序，所以一个进程可以对应多个程序



**进程是竞争计算机资源的基本单位**

### 1.2 Linux下PCB的定义

* 进程控制块（PCB）是用来**记录进程相关信息**和**管理进程**而设置的一种**数据结构**

* 进程控制块（PCB）是由**操作系统（OS）维护**的

* 系统**通过PCB感知进程的存在**

* **PCB随进程的创建而创建并填写，随着进程的消亡而释放**



> **PCB的组成**：
>
> * **进程标识符**：用于唯一标识该进程的整数
> * 进程名：通常是可执行文件名
> * 优先级：进程优先运行的权重
> * 寄存器值：用于保存当前进程运行到某一时刻各种数据信息
> * .....................



在Linux操作系统中，每一个进程都有一个PCB，每一个PCB都对应一个**task_struct结构体**，简言之，每创建一个进程就相当于创建一个task_struct结构体并填写其中的数据。



> Linux中的PCB定义在sched.h文件中
>
> 若想获得sched.h文件，需要到Linux内核官网下载内核源码
>
> 以Linux Kernel Source Code 2.6.32 为例展示task_struct部分源码

![image-20201230123634571](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20201230123634571.png)



## 2.进程查看

* 使用ps命令可以查看正在运行的进程瞬时的信息，而不是动态连续的信息

> 参数：
>
> * -a : 显示所有用户的进程
> * -u : 以用户为主的进程状态
> * -x : 列出较完整的信息



> 打印显示出来的信息：
>
> PID：进程标识符
>
> TTY：命令所运行的位置
>
> STAT：进程状态
>
> TIME：运行该命令所占用的CPU处理时间
>
> COMMAND：该进程所运行的命令



> 1.显示当前所有进程及详细信息

```shell
[gongruiyang@localhost ~]$ ps -ax
   PID TTY      STAT   TIME COMMAND
     1 ?        Ss     0:01 /usr/lib/systemd/systemd --switched-root --system --deserialize 21
     2 ?        S      0:00 [kthreadd]
     3 ?        S      0:00 [ksoftirqd/0]
     5 ?        S<     0:00 [kworker/0:0H]
     7 ?        S      0:00 [migration/0]
 ...................................................
```



> 2.显示某用户的进程及详细信息

```shell
[gongruiyang@localhost ~]$ ps -u gongruiyang 
   PID TTY          TIME CMD
  1736 ?        00:00:00 gnome-keyring-d
  1741 ?        00:00:00 gnome-session-b
  1748 ?        00:00:00 dbus-launch
  1749 ?        00:00:00 dbus-daemon
  1807 ?        00:00:00 gvfsd
  1812 ?        00:00:00 gvfsd-fuse
  1904 ?        00:00:00 ssh-agent
  1923 ?        00:00:00 at-spi-bus-laun
  1928 ?        00:00:00 dbus-daemon
 ...................................................
```



> 3.显示进程及其CPU和内存占用情况

```shell
[gongruiyang@localhost ~]$ ps -aux
USER        PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
root          1  0.0  0.1 128212  6836 ?        Ss   11:00   0:01 /usr/li
root          2  0.0  0.0      0     0 ?        S    11:00   0:00 [kthrea
root          3  0.0  0.0      0     0 ?        S    11:00   0:00 [ksofti
root          5  0.0  0.0      0     0 ?        S<   11:00   0:00 [kworke
root          7  0.0  0.0      0     0 ?        S    11:00   0:00 [migrat
root          8  0.0  0.0      0     0 ?        S    11:00   0:00 [rcu_bh
```



## 3. 进程创建

### 3.1 fork函数

```cpp
pid_t fork(void)
```

> fork调用一次，会在父进程中返回一个值，会在子进程中返回一个值
>
> 父进程中：返回子进程PID
>
> 子进程中：返回 0



> Q：为什么fork会返回两次呢？
>
> A：子进程复制父进程的堆和栈中的内容，此时，两个进程都处于fork函数中，都在等待fork函数执行结束并返回一个pid_t，所以会有两个返回值



> pid_t实际上就是int，被定义在sys/typpes.h中



---



代码演示：

> getpid()函数是获得**当前进程的进程号**
>
> getppid()函数是获得**当前进程父进程的进程号**

```cpp
#include <stdio.h>                                          
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int main()
{
  printf("       [fpid] [pid] [ppid]\n");
  pid_t fpid = fork();

  if(fpid < 0)	//创建进程失败
    perror("fork");
  else if(fpid == 0)//子进程执行这一段代码
    printf("child: %4d %4d %4d\n",fpid,getpid(),getppid());
  else	//父进程执行这一段代码
    printf("father: %4d %4d %4d\n",fpid,getpid(),getppid());

  while(1)	//父子进程都执行
  {
    sleep(1);
  }

  return 0;
}

```

运行结果：

```shell
       [fpid] [pid] [ppid]
father:  5081  5080   2717
child:      0  5081   5080
```

> 解释：pid为5080的父进程 创建了一个 pid为5081的子进程，其中 父进程的 父进程号为2717，子进程的 父进程号为5080



---



> fork特性：
>
> 子进程将父进程中打开的**所有文件描述符**都复制了一遍，父子进程中**相同编号的文件描述符**在内核中指向**同一个file结构体**，也就是说，file结构体的**引用数量增加**了



> 子进程不会继承父进程的一些数据：
>
> 1. 子进程**不复制**父进程设置的**锁**（若继承会导致排它锁矛盾）
> 2. 子进程**不复制**父进程的**pid**，而是产生自己的pid
> 3. 子进程**不复制**父进程中的**pending alarms**和**pending signals**，而是将自己的pending alarms清除，将pending signals置为空



> fork产生失败可能的两个原因：
>
> 1. 当前运行中的**进程数**已经达到了系统规定的**上限**，此时错误码（errno）的值为**EAGAIN**
> 2. 当前系统**内存容量**不足以开辟一个新的进程，此时错误码（errno）的值为**ENOMEM**

### 3.2 vfork函数

```cpp
pid_t vfork(void)
```

* 该函数返回值特点与fork相同

* vfork创建出来的子进程并不会直接将父进程的虚拟空间内容拷贝一份，而是与父进程共享一份虚拟空间，当子进程需要修改数据时，才会进行拷贝，这称之为写时拷贝技术
* 父进程使用vfork创建子进程后，父进程会被挂起，直到子进程终止或被替换后，才能继续推进
* 应当使用exit或_exit来终止vfork创建的子线程，不能使用return来终止。若使用return来终止子线程会导父进程回到调用vfork处，进而无限创建子进程进而产生段错误



---



代码演示：

```cpp
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{

  pid_t fpid = vfork();

  if(fpid < 0)
    perror("vfork");
  else if(fpid == 0)
  {
    printf("child:%d\n",getpid());
    while(1)
    {
      printf("while\n");                 
      sleep(1);
    }
  }
  else
    printf("father:%d\n",getpid());

  printf("========================\n");

  return 0;
}

```

输出结果：

```
child:5898
while
while
while
while
while
^C
```

> 解释：由于子进程处于循环sleep中，导致父进程一直被挂起无法执行下面的代码



### 3.3 fork与vfork的异同

**异：**

1. fork创建的子进程是父进程的一个副本，子进程将父进程的堆栈中存的数据信息拷贝一份到另外开辟的内存中去，并不能共享这些数据；vfork创建的子进程并不会立刻开辟新内存拷贝数据，而是共享父进程的堆栈中的数据，直到子进程终止或者被替换之前，都是在父进程的空间中运行
2. vfork保证子进程先运行；fork是让两个进程异步运行

**同：**

1. fork和vfork都是调用一次，但是返回两次

### 3.4 写时拷贝技术

> 传统拷贝方法：
>
> 1. 为子进程的页表分配页帧
> 2. 为子进程的页分配页帧
> 3. 初始化子进程的页表
> 4. 把父进程的页内容复制到子进程对应页中

* 传统的拷贝父进程资源实现过于简单且效率低下
* 写时拷贝是一种可以推迟甚至免除拷贝数据的技术

> 写时拷贝(Copy - On - Write)技术：
>
> 子进程并不拷贝父进程的数据资源，而是父子进程共享父进程原有的数据资源，**只有当要写入的时候，才进行资源的复制**



实际上，COW不但在Linux进程上有实际应用，而且在C++的String类在g++环境下也支持COW技术

```cpp
#include <iostream>
#include <stdio.h>
#include <string>
using namespace std;

int main()
{
	string str1 = "Hello\n";
	string str2 = str1;

	printf("Befor Write: str1[%x] str2[%x]\n", str1.c_str(), str2.c_str());
	
	str2.clear();
	str2 = "H\n";
	printf("After Write: str1[%x] str2[%x]\n", str1.c_str(), str2.c_str());

	return 0;
}
```

输出结果：

```shell
[gongruiyang@localhost TestCOW]$ g++ COWtest.cpp -o test
[gongruiyang@localhost TestCOW]$ ./test 
Befor Write: str1[18cec38] str2[18cec38]
After Write: str1[18cec38] str2[18cec98]
```

> 解释：从输出结果来看，str1和str2一开始指向同一内存地址，共享Hello这个数据，当str2要进行写入的时候，str2指向的地址发生改变，重新开辟空间存放数据，即只有写入东西的时候才进行内存的再分配



## 4. 进程退出

### 4.1 exit函数

```cpp
void exit(int status)
```

* 功能：正常终止进程
* 头文件：stdlib.h
* status：返回给父进程的状态值，通常用0或EXIT_SUCCESS表示成功，通常用非0或EXIT_FAILURE表示异常程序终止



代码测试：

```cpp
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
  pid_t fpid = fork();
  int status;
  if(fpid < 0)
    perror("fork");
  else if(fpid == 0)
  {
    printf("ChildP: childPID[%d]\n",getpid());
    exit(EXIT_SUCCESS);		//正常退出子进程
  }
  else
  {
    int ret_pid = wait(&status); //父进程阻塞等待子进程退出
    printf("FatherP: %d is normally exited with status:%d\n",ret_pid,status);	
  }

  return 0;
}

```

输出结果：

```
ChildP: childPID[8071]
FatherP: 8071 is normally exited with status:0
```



### 4.2 _exit函数

```cpp
void _exit(int status);
```

* 功能：立即终止子进程，并关闭所有属于该进程的文件描述符，该进程的所有子进程过继给init进程，并向父进程发送SIGCHLD信号，将status作为子进程退出状态返回给父进程
* 头文件：unistd.h
* status：返回给父进程的状态值，通常用0或EXIT_SUCCESS表示成功，通常用非0或EXIT_FAILURE表示异常程序终止



代码测试：

```cpp
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
  pid_t fpid = fork();
  int status;
  if(fpid < 0)
    perror("fork");
  else if(fpid == 0)
  {
    printf("ChildP: childPID[%d]\n",getpid());
    _exit(EXIT_SUCCESS);
  }
  else
  {
    int ret_pid = wait(&status);                                              
    printf("FatherP: %d is normally exited with status:%d\n",ret_pid,status);
  }

  return 0;
}

```

输出结果：

```
ChildP: childPID[8586]
FatherP: 8586 is normally exited with status:0
```

### 4.3 exit与_exit的异同

异：

1. exit定义在stdlib.h中；_exit定义在unistd.h中
2. exit会进行缓冲区刷新；_exit不会刷新缓冲区可能导致数据丢失

同：

1. 退出子进程并向父进程返回退出状态信息
2. exit是_exit的封装形式



代码测试：

```cpp
#include <stdio.h>
#include <stdlib.h>

int main()
{
  printf("Hello!\n");
  printf("World!");
  exit(0);
                     
  return 0;
}
```

```cpp
#include <stdio.h>  
#include <unistd.h>  
int main()  
{  
  printf("Hello!\n"); 
  printf("World!");  
  _exit(0);           
                      
  return 0;           
}                     
```

exit输出结果：

```
Hello!
World!
```

_exit输出结果：

```
Hello!
```

> 程序解释：由测试代码可以看出_exit由于不刷新缓冲区导致了数据丢失，而exit将__exit进行封装了，便得到更加安全，保全了数据



## 5. 进程等待

### 5.1 wait函数

```cpp
pid_t wait (int* status)
```

1. 包含头文件：sys/types.h和sys/wait.h
2. status：出参，用于保存子进程退出时的状态。
   * 正常退出情况下，一个int中的**低16位中的高8位**保存返回值（取出返回值：status & 0x7f ）
   * 异常退出情况下，一个Int中的**低7位**保存异常退出信号值（取出异常信号值：(status >> 8) & 0xff）
3. 返回值：退出进程的pid



> 功能：阻塞父进程一直等待子进程的退出，当子进程退出后，父进程才脱离阻塞继续推进



```cpp
WIFEXITED(int status)
```

该宏是用于检测进程是否正在退出：

宏值若为0代表非正常退出，若为非0则代表正常退出



---



代码演示：

```cpp
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
  pid_t fpid = fork();
  int status = 0;
    
  if(fpid < 0)
    perror("fork");
  else if(fpid == 0)
  {
    printf("Child:[%d]\n",getpid());
    exit(3);
  }
  else
  {
    printf("Father:[%d]\n",getpid());
    pid_t ret_pid = wait(&status);
    if(WIFEXITED(status))
        printf("%d has exited with return code %d\n",ret_pid,WEXITSTATUS(status));
    else
        perror("wait");
  }

  return 0;
}

```

输出结果：

```
Father:[6985]
Child:[6986]
6986 has exited with return code 3
```



### 5.2 waitpid函数

```cpp
pid_t waitpid(pid_t pid, int* status, int options)
```

参数：

* status：出参，用于保存子进程的退出状态
* pid

| **> 0**  |              **只等待进程ID为指定pid的子进程**               |
| :------: | :----------------------------------------------------------: |
| **= -1** |            **等待任何一个子进程，与wait功能一样**            |
| **= 0**  |       **等待任一个【子进程组ID=父进程组ID】的子进程**        |
| **< -1** | **等待任一个【子进程组ID= \|父进程组ID\|】的子进程 （\|\|意为绝对值）** |

* options

|  **WNOHANG**  |          **指定子进程结束才阻塞父进程，返回值为0**           |
| :-----------: | :----------------------------------------------------------: |
|     **0**     |        **与wait相同，阻塞父进程，一直等待子进程退出**        |
| **WUNTRACED** | **子进程若处于暂停状态：立刻返回<br />子进程若处于结束状态：不予理会** |

* 返回值

| > 0  | 已经结束的子进程的pid                |
| ---- | ------------------------------------ |
| 0    | 使用options为WUNTRACED且无子进程退出 |
| -1   | 调用出错（例如：无子进程）           |



> 功能：非阻塞父进程版本wait
>
> wait中调用了waitpid



代码测试：

```cpp
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>                                             
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
  pid_t fpid = fork();
  pid_t ret_waitpid = 0;
  int status;
    
  if(fpid < 0)
    perror("fork");
  else if(fpid == 0)	//子进程
  {
    printf("Child:%d\n",getpid());
    sleep(3);
    exit(0);	//子进程退出
  }
  else		//父进程
  {
    while(!ret_waitpid)	//循环waitpid等待子进程的退出
    {
      ret_waitpid = waitpid(fpid,&status,WNOHANG);
      if(ret_waitpid == 0)
      {
        printf("%d has not exit\n",fpid);	//子进程未退出就打印
        sleep(1);
      }
    }//while结束：说明此事ret_waitpid接收到了子进程exit后返回的pid
    if(ret_waitpid == fpid)
      printf("%d has exited with return code:%d\n",fpid,status);
  }

  return 0;
}

```

输出结果：

```
9311 has not exit
Child:9311
9311 has not exit
9311 has not exit
9311 has exited with return code:0
```



## 6. 程序替换

> 创建子进程的目的是为了完成其他的事情，完成其他任务，这个时候就用到了程序替换



辅助代码：子进程需要做的任务，可执行文件名为child_task,绝对路径为：/home/gongruiyang/ClassLinunx/pidTest/child_task

```cpp
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void child_task(int argc, char* argv[])
{
  for(int i = 0; i < argc; i++)
    printf("argv[%d] = %s\n",i,argv[i]);
}
//main函数有俩参数：argc表示程序的运行参数个数，argv用于保存程序参数
int main(int argc,char* argv[])
{
  child_task(argc,argv );
                                                                    
  return 0;
}
```

### 6.1 execl函数和execv函数

```cpp
int execl(const char *path, const char *arg, ...);
```

* path：用于保存想要执行的可执行文件绝对路径
* arg：执行可执行文件所需要的参数，如果不需要参数可以填NULL
* ...：不定参数，说明后面的参数可以是1个，也可以是多个

代码测试：

```cpp
#include <stdio.h>
#include <unistd.h>                                                      
#include <stdlib.h>

int main()
{
  pid_t  fpid = fork();

  if(fpid < 0)
    perror("fork");
  else if(fpid == 0)
  {
    execl("/home/gongruiyang/ClassLinunx/pidTest/child_task","-a","-b");
  }
  else
  {
    printf("Father Do!\n");
    sleep(1);
  }

  sleep(1);
  return 0;
}
```

输出结果：

```
[gongruiyang@localhost pidTest]$ ./exectest 
Father Do!
argv[0] = -a
argv[1] = -b
```



---



```cpp
int execv(const char *path, char *const argv[]);
```

* path：用于保存想要执行的可执行文件绝对路径
* argv：参数列表

代码测试：

```cpp
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
  pid_t  fpid = fork();
  if(fpid < 0)
    perror("fork");
  else if(fpid == 0)
  {

    char* child_argv[32];
    child_argv[0] = "-a";
    child_argv[1] = "-b";
    child_argv[2] = "-c";                                               
    child_argv[3] = NULL; //必须要有

    execv("/home/gongruiyang/ClassLinunx/pidTest/child_task",child_argv)
  }
  else
  {
    printf("Father Do!\n");
    sleep(1);
  }

  sleep(1);
  return 0;
}
```

输出结果：

```
Father Do!
argv[0] = -a
argv[1] = -b
argv[2] = -c
```





### 6.2 execlp函数和execvp函数

**加了p之后，其中path可以不填绝对路径，该函数会去PATH环境变量中寻找**



```cpp
int execlp(const char *path, const char *arg, ...);
```

* path：用于保存想要执行的可执行文件路径
* arg：执行可执行文件所需要的参数，如果不需要参数可以填NULL

代码测试：

```cpp
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
  pid_t  fpid = fork();
  if(fpid < 0)
    perror("fork");
  else if(fpid == 0)
  {
    execlp("ls","ls","-l",NULL);	//在环境变量中寻找ls并执行
  }
  else
  {
    printf("Father Do!\n");
    sleep(1);
  }

  sleep(1);
  return 0;
}

```

输出结果：

```
Father Do!
总用量 144
-rwxrwxr-x. 1 gongruiyang gongruiyang 8561 12月 30 17:33 child_task
-rw-rw-r--. 1 gongruiyang gongruiyang  348 12月 30 17:34 child_task.c
............................................................
```



---



```cpp
int execvp(const char *path, char *const argv[]);
```

* path：用于保存想要执行的可执行文件路径
* argv：参数列表

测试代码：

```cpp
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
  pid_t  fpid = fork();
  if(fpid < 0)
    perror("fork");
  else if(fpid == 0)
  {
    char* child_argv[10];
    child_argv[0] = "ls";
    child_argv[1] = "-l";
    child_argv[2] = NULL;    
    execvp("ls",child_argv);
  }
  else
  {
    printf("Father Do!\n");
    sleep(1);
  }

  sleep(1);
  return 0;
}
```

结果输出：

```
Father Do!
总用量 144
-rwxrwxr-x. 1 gongruiyang gongruiyang 8561 12月 30 17:33 child_task
-rw-rw-r--. 1 gongruiyang gongruiyang  348 12月 30 17:34 child_task.c
```





### 6.3 execle函数和execve函数

辅助程序：打印环境变量

```cpp
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char* argv[],char* env[])
{
  printf("env-var:");                        
  for(int i = 0 ; env[i] != NULL; i++)
    printf("env[%d]:%s\n",i,env[i]);
  return 0;
}
```



```cpp
int execle(const char *path, const char *arg,..., char * const envp[]);
```

* path：用于保存想要执行的可执行文件绝对路径
* arg：执行可执行文件所需要的参数，如果不需要参数可以填NULL
* ...：不定参数，说明后面的参数可以是1个，也可以是多个
* envp：自定义环境变量，可以填NULL



```cpp
int execvpe(const char *path, char *const argv[], char *const envp[]);
```

* path：用于保存想要执行的可执行文件路径
* argv：参数列表
* envp：自定义环境变量，可以填NULL



**加了e之后，相比于之前，增加了一个环境变量参数，可以使用自定义环境变量**



测试代码：

```cpp
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
  pid_t  fpid = fork();
  if(fpid < 0)
    perror("fork");
  else if(fpid == 0)
  {
    char* child_env[10];
    child_env[0] = "MYVAL=1000";
    child_env[1] = "TMP=12";
    child_env[2] = NULL;
    execvpe("/home/gongruiyang/ClassLinunx/pidTest/envtest",NULL,child_env);
    //execvle("/home/gongruiyang/ClassLinunx/pidTest/envtest",NULL,child_env);
  }                                                                               
  else
  {
    printf("Father Do!\n");
    sleep(1);
  }

  sleep(1);
  return 0;
}

```

输出结果：

```
Father Do!
env-var:env[0]:MYVAL=1000
env[1]:TMP=12
```





