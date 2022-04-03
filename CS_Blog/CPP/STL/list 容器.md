# list 容器

## 基本概念

- - 功能：将数据进行链式存储
  - 链表(list)是一种物理存储单元上**非连续的存储结构**，数据元素的**逻辑顺序**是通过链表中的**指针链**接实现的
  - 链表由结点组成
  - 结点由**数据域**和**指针域**组成
  - list的底层是一个**双向循环链表**
  - list中的迭代器只支持**前移**和**后移**，属于**双向迭代器**

![image.png](https://cdn.nlark.com/yuque/0/2020/png/1693220/1597989567022-684b549d-db5f-4144-b9bc-404716a4120a.png)

- - list的优点：

- - - 采用动态内存分配，不会产生空间浪费或溢出
    - 链表执行插入和删除操作方便，无需移动大量元素

- - list的缺点：

- - - 链表有一个额外的指针域耗费空间，遍历时耗费时间

- - 插入和删除操作不会导致原迭代器失效，这与vector不同



## list构造函数

```cpp
/*
构造函数
    list<T> name;
    list(beg,end);
    list(n,elem);
    list(const list& list);
*/
void test01()
{
    list<int> L1;
    list<int> L2(3, 10);
    list<int> L3(L2);
    list<int> L4(L3.begin(), L3.end());

    printList(L1);
    printList(L2);
    printList(L3);
    printList(L4);
}
```



## list赋值和交换

```cpp
/*
list赋值和交换
    assign(beg,end);
    assign(n,elem);

    list& operator=(const list& name);
    swap(list);
*/
void test02()
{
    list<int> L1(3, 100);
    list<int> L2(3, 99);
    list<int> L3(5, 10);

    cout << "L1：";
    printList(L1);  //100 100 100
    cout << "L2：";
    printList(L2);  //99 99 99

    L1.swap(L2);
    cout << "交换后" << endl;

    cout << "L1：";
    printList(L1);  //99 99 99
    cout << "L2：";
    printList(L2);  //100 100 100

    L1.assign(5,1);
    cout << "L1：";
    printList(L1);  //1 1 1 1 1 

    L2 = L1;
    cout << "L2：";
    printList(L2);  //1 1 1 1 1 

    L1.assign(L3.begin(), L3.end());
    cout << "L1：";
    printList(L1);  //10 10 10 10 10
}
```



## list大小操作

```cpp
/*
list大小操作
    size();             返回list容器中元素的个数
    empty();            判断list容器是否为空
    resize(n);          重新指定容器长度为n，若变长则填充 默认填充值 ，若变短则 删除 多余元素
    resize(n,elem);     重新指定容器长度为n，若变长则填充 elem       ，变变短则 删除 多余元素
*/
void test03()
{
    list<int> L1;
    L1.assign(2,5);

    cout << "Size:" << L1.size() << endl;   //2
    if (!L1.empty())
        cout << "不空" << endl;               //不空
    else
        cout << "空" << endl;

    L1.resize(5);
    printList(L1);      //5 5 0 0 0

    L1.resize(7, 1);
    printList(L1);      //5 5 0 0 0 1 1
}
```



## list插入和删除

```cpp
/*
list插入和删除
    push_back(elem);    尾插
    push_front(elem);   首插
    pop_back();         尾删
    pop_front();        首删

    insert(pos,elem);       在迭代器pos位置插入 elem            元素
    insert(pos,n,elem);     在迭代器pos位置插入 n个elem         元素
    insert(pos,beg,end);    在迭代器pos位置插入 [beg,end)区间的 元素

    erase(beg,end);         删除[beg,end)区间的数据，返回下一个数据的位置
    erase(pos);             删除迭代器pos位置的数据，返回下一个数据的位置

    clear();                清空容器中的数据
    remove(elem);           删除所有与elem值匹配的元素
*/
void test04()
{
    list<int> L1,L2(3,0);
    L1.push_back(1);
    L1.push_back(1);
    L1.push_front(2);
    L1.push_front(2);
    printList(L1);  // 2 2 1 1

    L1.pop_back();
    L1.pop_front();
    printList(L1);  // 2 1

    L1.insert(L1.begin(), 0);
    printList(L1);  //0 2 1

    L1.insert(L1.begin(), 3, 1);
    printList(L1);  //1 1 1 0 2 1

    L1.insert(L1.end(), L2.begin(), L2.end());
    printList(L1);  //1 1 1 0 2 1 0 0 0

    L2.erase(L2.begin(), --L2.end());
    printList(L2);  //0

    L1.erase(--L1.end());
    printList(L1);  //1 1 1 0 2 1 0 0

    L2.clear();
    cout << "L2 Size:" << L2.size()<<endl; //0

    L1.remove(1);
    printList(L1); //0 2  0 0
}
```



## list数据存取

```cpp
/*
list数据存取
    front();    获取链头数据
    back();     获取链尾数据
*/
void test05()
{
    list<int> L1;
    L1.push_back(1);
    L1.push_back(2);
    L1.push_back(3);

    int front = L1.front();
    int back = L1.back();
    cout << "front:" << front << endl;      // 1
    cout << "back:" << back << endl;        // 3
}
```

> 总结：
>
> list容器由于底层数据结构是**链表**，所以**无法通过[]和at来访问数据**

## list反转和排序

```cpp
/*
list反转和排序
    reverse();      反转链表
    sort();         排序链表
*/
bool _sort(int x, int y)
{
    return x > y;
}
void test06()
{
    list<int> L1, L2;
    L1.push_back(1);
    L1.push_back(2);
    L1.push_back(3);
    L2.push_back(3);
    L2.push_back(2);
    L2.push_back(1);
    printList(L1);  //1 2 3
    printList(L2);  //3 2 1

    L1.reverse();
    printList(L1);  //3 2 1

    L2.sort();
    printList(L2);  //1 2 3

    L2.sort(_sort);
    printList(L2);  //3 2 1
}
```