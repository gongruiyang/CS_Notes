# 1

![image-20210205133232676](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210205133232.png)

> 正确答案：C
>
>  (y = 123) && (x < 4)
>
> (y = 123)属于赋值语句，赋值后相当于 1&&( x < 4)
>
> 当循环4次后，x的值变成4后&&左边布尔表达式x < 4返回0，此时for循环继续的条件不再满足，便退出循环

# 2

![image-20210205133343797](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210205133343.png)

> 正确答案：B
>
> **%m.ns 输出占m列，但只取字符串中左端n个字符**。这n个字符输出在m列的右侧，左补空格。

# 3

![image-20210205134033808](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210205134033.png)

> 正确答案：C
>
> ```cpp
> int p[4]; 	//含有4个元素的数组
> int *p;		//int型指针
> int *(p[4]);	//指针数组，是一个数组，其中包含4个int型指针
> int *p[4];		//指针数组，是一个数组，其中包含4个int型指针
> int (*p)[4];	//数组指针，是一个指针，这个指针指向一块可以保存4个int型数据的未命名数组
> ```

# 4

![image-20210205134854108](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210205134854.png)

> 正确答案：D
>
> 前缀++--与*优先级相同，从左向右依次计算
>
> 后缀++--优先级高于后缀++---，同理优先级也高于*
>
> 优先级：* > +=
>
> ```cpp
> *p+=1;	//*p -> year, year += 1   -> 1010
> (*p)++;	//*p -> year, year++ -> 1010
> ++(*p);	//*p -> year, ++year -> 1010
> *p++; 	//p++ -> p = &year + 1, *p -> 取出year后面4个字节的位置数据
> ```

# 5

![image-20210205140128350](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210205140128.png)

> 正确答案：：C
>
>  63为系统中：char(1字节)、int(4字节)、double(8字节)、float(4字节)
>
> 不同类型数据之间进行数学运算，字节数小的会自动转换为字节数大的，所以最终以8字节double的数据类型接收答案



# 6

![image-20210205140232270](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210205140232.png)

> 正确答案：C
>
> 内联函数适用情况：
>
> 内联扩展是用来**消除函数调用 时的时间开销**。它通常用于**频繁执行**的函数

# 7

![image-20210205140243896](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210205140243.png)

> 正确答案：D
>
> A.函数参数的缺省默认值设定个数没有限制
>
> B.缺省参数必须连续且靠右
>
> C.函数参数可以不设置缺省默认值

# 8

![image-20210205140254195](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210205140254.png)

> 正确答案：D
>
> 静态数据成员需用static修饰，且必须在类外进行初始化，引用时“<类名>::”来引用，静态数据成员由所有对象共用一份

# 9

![image-20210205140302781](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210205140302.png)

> 正确答案：A
>
>  ```cpp
> 或运算符规则：
> 1 | 0 = 1
> 0 | 1 = 1
> 1 | 1 = 1
> 0 | 0 = 0
>     
> 1011
> 1010
> -----
> 1011 ---> 11
>  ```

# 10

![image-20210205140310958](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210205140310.png)

> 正确答案：D
>
> 父类公有成员在派生类中是否仍然是公有的取决于**继承方式**



