# 1. CPP程序的内存模型

`内存模型图解`

![image-20210127111741419](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210127111741419.png)

* `内核空间`：存放的是与操作系统相关的代码，用户没有权限去操作，如果要操作，需要调用**系统调用函数**来进行操作
* `栈区`：其空间**自上而下**进行增长，存放函数调用的数据（**函数参数**、**函数体中的局部变量**、**寄存器信息**），函数调用结束后，对应的**栈帧被回收**
* `堆区`：其空间**自下而上**进行增长，用户动态进行**动态申请的内存**就在堆区，用户需在代码中申请，也需要自己在代码中将申请的内存给释放掉，否则会导致**内存泄漏**
* `内存映射段`：放置**静态库**和**动态库**相关数据
* `数据段`：存放程序的**全局变量**及**静态变量**，该段中的数据的生命周期是跟随程序的生命周期
* `代码段`：放置**用户代码**和**字符串常量**，该位置的数据是不能修改的

`程序代码中数据分析`

![image-20210127113016261](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210127113016261.png)

`为什么CPP要创造出独有的new/delete开辟空间的方式？而不是用mallic/free？`

原因：在CPP程序中，使用malloc/free在堆区上给**内置类型**申请或释放空间是没有问题的，可是不能使用malloc/free来给**对象**申请或释放空间，原因是malloc**不会**主动**调用构造函数**，因此申请的空间不能称之为对象空间，仅仅只是一块**和对象同等大小的空间**，当使用free来释放空间时，并**不会**主动**调用析构函数**来对**对象资源**进行清理

# 2. CPP中动态内存管理方式

## 2.1 new/delete

* new/delete是用于申请和释放单个类型的空间

`申请单个内置类型空间`

```cpp
int* p1 = new int(10);
cout << *p1 << endl;	// 10
delete p1;	 
```

> 使用new申请一个int大小的空间，并将空间中的数据初始化为10，将空间首地址保存在指针p1中

`申请单个对象空间`

```cpp
#include <iostream>
using namespace std;

class Test
{
public:
	 Test(){ cout << "Test()" << endl; }
	~Test(){ cout << "~Test()" << endl; }
};

int main()
{
	Test* ptest = new Test();
	delete ptest;

	return 0;
}
```

```
Test()
~Test()
```

> 使用new给对象分配空间时，自动调用构造函数，使用delete给对象释放空间时，会自动调用析构函数

## 2.2 new[]/delete[]

* new[]/delete[]是用于申请和释放一段连续空间

`申请多个内置类型空间`

```cpp
int* p2 = new int[5]{0, 1, 2, 3, 4};
cout << p2[3] << endl;	// 3
delete[] p2;
```

> 使用new[num]申请num个连续的同类型空间并使用{}中的数据进行初始化，将空间首地址保存在指针p2中

`申请多个对象空间`

> 使用new[num]给num个对象初始化空间，使用delete释放指针指向的多个对象

```cpp
#include <iostream>
using namespace std;

class Test
{
public:
	 Test(){ cout << "Test()" << endl; }
	~Test(){ cout << "~Test()" << endl; }
};

int main()
{
	Test* ptest = new Test[5];
	delete[] ptest;

	return 0;
}
```

```
Test()
Test()
Test()
Test()
Test()
~Test()
~Test()
~Test()
~Test()
~Test()
```



# 3. 分配与释放不配套使用的后果

* `new/delete`与`new[]/delete[]`与`malloc/free`不配套使用会导致`程序中断`或者`内存泄漏`

## 3.1 new与delete[]

> new分配的空间使用delete[]来释放会导致程序中断

```cpp
	Test* ptest = new Test();
	delete[] ptest;
```

![image-20210127122303452](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210127122303452.png)



## 3.2 new[]与delete

> new[]分配的内存使用delete来释放会导致持续中断

```cpp
	Test* ptestarr = new Test[5];
	delete ptestarr;
```

![image-20210127122450461](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210127122450461.png)

## 3.3 malloc与delete

> malloc申请的空间使用delete来释放会导致内存泄露

```cpp
	Test* ptest = (Test*)malloc( sizeof(Test));
	delete ptest;
```

## 3.4 malloc与delete[]

> malloc申请的空间使用delete[]来释放会导致中断

```cpp
	Test* ptest = (Test*)malloc( sizeof(Test));
	delete[] ptest;
```

![image-20210127122921160](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210127122921160.png)

## 3.5 new与free

>new申请的空间使用free来释放会导致内存泄露

```cpp
	Test* ptest = new Test();
	free ptest;
```

```cpp
Test()
```

## 3.6 new[]与free

>new[]申请的空间使用free来释放会导致程序中断

```cpp
	Test* ptest = new Test[5];
	free(ptest);
```

![image-20210127123159685](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210127123159685.png)

# 4. new与delete的底层实现解析

* new和delete是动态申请和释放内存的操作符
* new底层调用了operator new来申请空间，delete底层调用了operator delete来释放空间
* operator new和operator delete是系统提供的全局函数

`operator new`

```cpp
/*
operator new：
	该函数实际通过malloc来申请空间，当malloc申请空间成功时直接返回；
	申请空间失败，尝试执行空间不足应对措施，如果该应对措施用户提前设置了，则继续申请，否则抛异常。
*/
void *__CRTDECL operator new(size_t size) _THROW1(_STD bad_alloc) {
    // try to allocate size bytes
    void *p;
    while ((p = malloc(size)) == 0)		//这里的0代表 NULL
        if (_callnewh(size) == 0)
        {
            // report no memory
            // 如果申请内存失败了，这里会抛出bad_alloc 类型异常
            static const std::bad_alloc nomem;	//实例化bad_alloc类型异常
            _RAISE(nomem);	//抛出bad_alloc类型异常的实例
        }
    return (p); //返回申请的内存空间首地址
}
```

>* `size`：要申请内存的大小，单位是byte
>* `bad_alloc`：内存申请失败后抛出的异常类型
>* `nomem`：**bad_alloc类型的异常**的实例变量
>* `_callnewh`：应对措施
>
>由operator new源码中可以看出，**new底层实际调用了malloc来分配内存**，只不过malloc函数的形参是由new自动推导计算传进去的，不需要程序员手动计算传入

`operator delete`

```cpp
/*
free的实现:
	通过一个宏定义函数实现的
*/
#define free(p) _free_dbg(p,_NORMAL_BLOCK)

/*
operator delete: 
	该函数最终是通过free来释放空间的
*/
void operator delete(void *pUserData) {
    _CrtMemBlockHeader * pHead;
    RTCCALLBACK(_RTC_Free_hook, (pUserData, 0));
    if (pUserData == NULL)
        return;
    _mlock(_HEAP_LOCK); /* block other threads */
    __TRY
        /* get a pointer to memory block header */
        pHead = pHdr(pUserData);
    /* verify block type */
    _ASSERTE(_BLOCK_TYPE_IS_VALID(pHead->nBlockUse));
    _free_dbg( pUserData, pHead->nBlockUse );	//  <----这里调用了free来释放
    __FINALLY
        _munlock(_HEAP_LOCK); /* release other threads */
    __END_TRY_FINALLY
        return; 
}

```

> 由operator delete源码可以看出，**delete最底层实际调用了free来释放内存**

# 5. new与delete原理总结

## 5.1 内置数据类型

> * 如果是给内置数据类型申请空间，使用new/delete或malloc/free是没有任何区别的；
> * new/delete是申请单个的空间
> * new[]/delete[]是申请的是连续多个的空间
> * malloc/free是申请若干个连续的空间

## 5.2 自定义数据类型

> 如果是给自定义类对象申请空间时：
>
> * new：调用operator new函数，operator new函数中调用malloc申请空间，在申请的空间上调用构造函数，完成对象的初始化
> * new[]：调用operator new函数，operator new函数中调用malloc完成对N个对象的空间申请，在申请的空间上调用N次构造函数，完成对N个对象的初始化
> * delete：调用operator delete函数，operator delete函数中调用free释放空间，释放空间之前调用析构函数，进行对象资源的清理
> * delete[]：调用operator delete函数，operator delete函数中调用free释放空间，释放空间之前调用N次析构函数，进行对象资源的清理



# 6. 内存块的组织方式解析（_CrtMemBlockHeader结构体）

```cpp
/*
For diagnostic purpose, blocks are allocated with extra information and
stored in a doubly-linked list.  This makes all blocks registered with
how big they are, when they were allocated, and what they are used for.
为了诊断程序的目的，内存块块被分配了额外的信息并存储在一个双链表中。
这使得所有的块都注册了它们的大小、分配的时间以及它们的用途。
 */

#define nNoMansLandSize 4

typedef struct _CrtMemBlockHeader
{
        struct _CrtMemBlockHeader * pBlockHeaderNext;	//指向前继的指针
        struct _CrtMemBlockHeader * pBlockHeaderPrev;	//指向后继的指针
    
        char *                      szFileName;	//存储的发起分配操作的那行代码所在的文件的路径和名称
        int                         nLine;		//存储的发起分配操作的那行代码所在的行号
#ifdef _WIN64
        /* These items are reversed on Win64 to eliminate gaps in the struct
         * and ensure that sizeof(struct)%16 == 0, so 16-byte alignment is
         * maintained in the debug heap.
            这些项在Win64上被反转，以消除结构中的间隙，
            并确保sizeof（struct）%16==0，因此在调试堆中保持16字节对齐。
         */
        int                         nBlockUse;	//类型
        size_t                      nDataSize;	//请求分配内存的大小
#else  /* _WIN64 */
        size_t                      nDataSize;
        int                         nBlockUse;
#endif  /* _WIN64 */
        long                        lRequest;	// 请求号
        unsigned char               gap[nNoMansLandSize];	//gap[4]-> 4bytes 
        /* followed by:
         *  unsigned char           data[nDataSize];
         *  unsigned char           anotherGap[nNoMansLandSize];
         */
} _CrtMemBlockHeader;
```

* `struct _CrtMemBlockHeader`存放了你**申请的内存块的相关信息**，而这个结构体就存放在你malloc返回的指针上面
* 堆上面申请的内存块以**双向链表**的方式组织在一起，方便进行内存管理
* 所以你申请的内存不仅仅只有你指定大小的内存，会给你**多分配32字节的空间**存储申请内存的相关信息

`图解内存结点`

![image-20210127143300665](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210127143300665.png)

# 7. malloc/free和new/delete的区别

malloc/free和new/delete的`共同点`是：都是从**堆**上申请空间，并且需要用户**手动释放**。

malloc/free和new/delete的`不同点`是：

1. malloc和free是**函数**，new和delete是**操作符**

2. malloc申请的空间**不会初始化**，new**可以初始化**

3. malloc申请空间时，需要**手动计算空间大小并传递**，new**无需手动计算与传递**，只需在其后跟上空间的类型即可

4. malloc的**返回值为void***, 在**使用时必须强转**，new**不需要强转**，因为new后跟的是空间的类型

5. malloc申请空间失败时，**返回的是NULL**，因此使用时必须判空，new不需要，但是new**需要捕获异常**

6. 申请自定义类型对象时，malloc/free**只会开辟空间，不会调用构造函数与析构函数**，而new在申请空间后**会调用构造函数**完成对象的初始化，delete在释放空间前会**调用析构函数**完成空间中资源的清理

# 8. 内存泄露

`内存泄漏的解释`

内存泄漏指因为疏忽或错误造成程序未能释放已经不再使用的内存的情况。内存泄漏并不是指内存在物理上的消失，而是应用程序分配某段内存后，因为设计错误，失去了对该段内存的控制，因而造成了内存的浪费。

`内存泄露的危害`

长期运行的程序出现内存泄漏，影响很大，如操作系统、后台服务等等，出现内存泄漏会导致响应越来越慢，最终卡死

`内存泄露的分类`

* **堆内存泄露**：堆内存指的是程序执行中依据须要分配通过malloc / calloc / realloc / new等从堆中分配的一块内存，用完后必须通过调用相应的 free或者delete 删掉。假设程序的设计错误导致这部分内存没有被释放，那么以后这部分空间将无法再被使用，就会产生Heap Leak。
* **系统资源泄露**：指程序使用系统分配的资源，比方套接字、文件描述符、管道等没有使用对应的函数释放掉，导致系统资源的浪费，严重可导致系统效能减少，系统执行不稳定。

`避免内存泄露的手段`

* 事前检测：使用**智能指针**管理内存
* 事后检测：使用**内存泄露检测工具**：Windows（VLD）、Linux（valgrind、mtrace、dmalloc）