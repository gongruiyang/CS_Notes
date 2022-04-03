# deque 容器

- deque容器基本概念

- - 功能：双端队列，可以对头端和尾端进行删除和插入操作

- deque和vector区别：

- - vector对于头部的插入删除效率低，数据量越大，效率越低；deque对头部的插入删除效率较高
  - vector访问元素时的速度比deque快



![image.png](https://cdn.nlark.com/yuque/0/2020/png/1693220/1597666406376-38a5c67a-1fe0-4f25-91f5-7e1d016b6671.png)

- deque内部工作原理：  

- - deque内部有个中控器，维护每段缓冲区中的内容，缓冲区中存放真实数据
  - 中控器维护的时每个缓冲区的地址，使得使用deque时像一片连续的内存空间

![image.png](https://cdn.nlark.com/yuque/0/2020/png/1693220/1597911117944-2ebe56fc-7d58-417b-b3d3-beaa198ae45a.png)



- deque构造函数

```c++
/*
构造函数
    deque<T> name;      默认构造函数
    deque(beg,end);     将[beg,end)区间中的元素拷贝给本身
    deque(n,elem);      构造函数将n个elem拷贝给本身
    deque(const deque& name);   拷贝构造函数
*/
void test01()
{
    deque<int> d1;
    d1.push_back(100);
    d1.push_back(200);
    d1.push_back(300);
    d1.push_front(1);
    d1.push_front(2);
    d1.push_front(3);
    printDeque(d1);     //3 2 1 100 200 300

    deque<int> d2(d1.begin(), d1.end());    
    printDeque(d2);     //3 2 1 100 200 300

    deque<int> d3(3, 100);      
    printDeque(d3);     //100 100 100

    deque<int> d4(d3);
    printDeque(d4);     //100 100 100
}
```

- deque赋值操作



```c++
/*
deque复制操作
    deque& operator=(const deque<T>& name);     重载=操作符
    
    assign(beg,end);    将[beg,end)区间中的数据拷贝赋值给本身
    assign(n,elem);     将n个elem拷贝赋值给本身
*/
void test02()
{
    deque<int> q1(5, 100);
    printDeque(q1);     //100 100 100 100 100
    
    deque<int> q2;
    q2 = q1;
    printDeque(q2);     //100 100 100 100 100

    q2.assign(5, 15);
    printDeque(q2);     //15 15 15 15 15

    q2.assign(q1.begin(), q1.end());
    printDeque(q2);     //100 100 100 100 100
}
```



- deque大小操作

```c++
/*
deque大小操作
    deque.empty();      判断容器是否为空
    deque.size();       返回容器中元素的个数

    重新指定容器size，若变长以默认值或elem填充，若变短则删除多余数据
    deque.resize(num);  
    deque.resize(num,elem);
*/
void test03()
{
    deque<int> q1;
    q1.assign(5, 10);
    printDeque(q1); //10 10 10 10 10

    cout << q1.size() << endl;      //5
    cout << q1.empty() << endl;     //0

    q1.resize(7);
    printDeque(q1);     //10 10 10 10 10 0 0
    q1.resize(9, 10);
    printDeque(q1);     //10 10 10 10 10 0 0 10 10
}
```



- deque插入和删除

```c++
/*
deque插入和删除操作
    两端：
        push_back(elem);    尾插
        push_front(elem);   首插
        pop_back();         尾删
        pop_front();        首删

    指定位置：
        insert(pos,elem);       在迭代器pos位置插入 1个elem         元素
        insert(pos,n,elem);     在迭代器pos位置插入 n个elem         元素
        insert(pos,beg,end);    在迭代器pos位置插入 [beg,end)区间的 元素
        
        clear();                清空所有数据
        erase(beg,end);         删除[beg,end)区间的数据
        erase(pos);             删除迭代器pos位置上的数据
*/
void test04()
{
    deque<int> q1;
    q1.push_back(1);
    q1.push_back(1);
    q1.push_front(2); 
    q1.push_front(2);
    printDeque(q1);     // 2 2 1 1

    q1.pop_back();
    q1.pop_front();
    printDeque(q1);     // 2 1

    q1.insert(q1.begin() + 1, 5);
    q1.insert(q1.begin() + 3, 5, 4);
    printDeque(q1);     //2 5 1 4 4 4 4 4

    deque<int> q2;
    q2.insert(q2.begin(), q1.begin(), q1.end());
    printDeque(q2);     //2 5 1 4 4 4 4 4

    q2.clear();
    cout << q2.size() << endl;  //0

    printDeque(q1);         //2 5 1 4 4 4 4 4
    q1.erase(q1.begin());
    printDeque(q1);         //5 1 4 4 4 4 4

    printDeque(q1);         //5 1 4 4 4 4 4
    q1.erase(q1.begin()+1, q1.end()-1);
    printDeque(q1);         //5 4
}
```



- deque数据存取

```c++
/*
deque数据存储
    at(int idx);    返回索引idx所指的数据
    operator[]； 返回索引idx所指的数据
    front();    返回容器中第一个数据元素
    back();     返回容器中最后一个数据元素
*/
void test05()
{
    deque<int> q1;
    q1.push_back(1);
    q1.push_back(2);
    q1.push_back(3);
    q1.push_back(4);

    cout << "第一个元素：" << q1.front() << endl;
    cout << "最后一个元素：" << q1.back() << endl;

    for (int i = 0; i < q1.size(); i++)
        cout << q1[i] << " ";
    cout << endl;

    for (int i = 0; i < q1.size(); i++)
        cout << q1.at(i) << " ";
    cout << endl;
}
```



- deque排序

```c++
/*
deque排序
    sort(iterator beg,iterator end);    对beg和end区间内的元素进行排序
*/
void test06()
{
    deque<int> q1;
    for (size_t i = 10; i >= 1; i--)
        q1.push_back(i);
    printDeque(q1);
    sort(q1.begin(), q1.end());
    printDeque(q1);
}
```