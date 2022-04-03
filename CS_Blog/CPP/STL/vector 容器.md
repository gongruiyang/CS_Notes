# vector 容器

- **vector基本概念**

- - **功能：**vector数据结构和**数组**相似，称为**单端数组**
  - **与数组区别：**数组是**静态**的，vector是可以**动态扩展**的
  - **动态扩展**：**寻找更大的空间，将原数据拷贝进新空间，释放原空间**，而不是在原空间之后续接内存空间

![image.png](https://cdn.nlark.com/yuque/0/2020/png/1693220/1597501059638-eb5f241e-2589-411e-a11a-adf882463db1.png)

- - vector的迭代器是支持**随机访问**的迭代器

- **vector构造函数**

```c++
/*
vector构造函数  
    vector<T> v;                    //默认构造函数
    vector(v.begin(),v.end());      //v[begin(),end())区间
    vector(n,elem);                 //n个elem
    vector(const vector &vec);      //拷贝构造函数
*/
void printVector(vector<int>& v)
{
    for each (int var in v)
        cout << var << " " ;
    cout << endl;
}
void test01()
{
    vector<int> v1;
    for (int  i = 0; i < 10; i++)
        v1.push_back(i);
    printVector(v1);

    vector<int> v2(v1.begin(), v1.end());
    printVector(v2);

    vector<int> v3(10, 100);
    printVector(v3);

    vector<int> v4(v3);
    printVector(v4);
}
```

- **vector赋值操作**

```c++
/*
vector赋值操作
    vector& operator=(const vector& v);     //重载=号操作
    assign(beg,end);                        //将[beg,end)区间的数据拷贝进本身
    assign(n,elem);                         //n个elem拷贝赋值给本身
*/
void test02()
{
    vector<int> v1;
    for (int i = 0; i < 10; i++)
        v1.push_back(i);
    printVector(v1);

    vector<int> v2 = v1;
    printVector(v2);

    vector<int> v3;
    v3.assign(v1.begin(), v1.end());
    printVector(v3);

    vector<int> v4;
    v4.assign(10, 100);
    printVector(v4);
}
```

- **vector容量和大小**

```c++
/*
vector容器的容量和大小操作
    empty();                判断容器是否为空
    capacity();             容器的容量
    size();                 返回容器中元素的个数
    resize(int n);          重新制定容器的长度为n,变长，用默认值 填充，变短，删除多余元素
    resize(int n,T elem);   重新制定容器的长度为n,变长，用elem   填充，变短，删除多余元素
*/
void test03()
{
    vector<int> v1;
    for (int i = 0; i < 10; i++)
        v1.push_back(i);
    printVector(v1);

    bool isEmpty = v1.empty();
    int capacity = v1.capacity();
    int size     = v1.size();
    cout << isEmpty << endl;        //0
    cout << capacity << endl;       //13
    cout << size << endl;           //10

    v1.resize(15);
    capacity = v1.capacity();
    cout << capacity << endl;       //19
    printVector(v1);    //0 1 2 3 4 5 6 7 8 9 0 0 0 0 0

    v1.resize(20, 100);
    capacity = v1.capacity();
    cout << capacity << endl;       //28
    printVector(v1);    //0 1 2 3 4 5 6 7 8 9 0 0 0 0 0 100 100 100 100 100
}
```

- **vector插入和删除**

```c++
/*
vector的插入和删除
    push_back(T elem);  尾部插入元素
    pop_back();         尾部删除元素

    insert(const_iterator pos,T elem);                  迭代器指向位置插入一个elem元素
    insert (const_iterator pos, int count, T elem);     迭代器指向位置插入count个elem元素

    erase(const_iterator pos);                          删除迭代器位置的元素
    (const_iterator start, const_iterator end);         删除某一区间的元素

*/
void test04()
{
    vector<int> v1;

    v1.push_back(10);
    v1.push_back(20);
    v1.push_back(30);
    v1.push_back(40);
    printVector(v1);    //10 20 30 40

    v1.pop_back();
    v1.pop_back();
    printVector(v1);    //10 20

    v1.insert(v1.begin(), 1000);
    v1.insert(v1.end(), 5, 100);
    printVector(v1);    //1000 10 20 100 100 100 100 100

    v1.erase(v1.begin());
    printVector(v1);    //10 20 100 100 100 100 100

    v1.erase(v1.begin(), v1.end());
    //v1.clear();
    printVector(v1);    //空
}
```

- **vector数据存取**

```c++
/*
vector数据存取
  按下表取元素：
    at(int index);      
    operator[]

    front();    取出头元素
    back();     取出尾元素
*/
void test05()
{
    vector<char> v1;
    v1.push_back('a');
    v1.push_back('b');
    v1.push_back('c');
    v1.push_back('d');

    for (int i = 0; i < v1.size(); i++)
        cout << v1.at(i) << " ";
    cout << endl;

    for (int i = 0; i < v1.size(); i++)
        cout << v1[i] << " ";
    cout << endl;

    cout << "front:" << v1.front() << endl;
    cout << "back:" << v1.back() << endl;
}
```

- **vector互换容器**

- - 将两个容器中的元素进行互换
  - **收缩空间**

```c++
/*
vector元素互换
    swap(vector v);
*/
void test06()
{
    vector<int> v1;
    vector<int> v2;

    v1.push_back(1);
    v1.push_back(2);
    v2.push_back('a');
    v2.push_back('b');

    v1.swap(v2);
    printVector(v1);
    printVector(v2);
}
void test07()
{
    //收缩内存
    vector<int> v1;
    for (int i = 0; i < 10000; i++)
        v1.push_back(i);
    cout << "capacity:" << v1.capacity() << endl;   //12138
    cout << "size:" << v1.size() << endl;           //10000

    v1.resize(3);
    cout << "capacity:" << v1.capacity() << endl;   //12138
    cout << "size:" << v1.size() << endl;           //3

    //利用匿名对象给v1容量缩小
    vector<int>(v1).swap(v1);
    cout << "capacity:" << v1.capacity() << endl;   //3
    cout << "size:" << v1.size() << endl;           //3
}
```

- **vector预留空间**

- - 减少vector在动态扩展容量时的扩展次数

```c++
/*
vector预留空间
    reserve(int len);   容器预留len个元素长度，预留位置不初始化，元素不可访问
*/
void test08()
{
    vector<int> v1;
    cout << "cap:" << v1.capacity() << " " << "size:" << v1.size() << endl;
    //0 0
    
    v1.reserve(10000);

    int count = 0;  //统计开辟次数
    int *p = NULL;
    for (int i = 0; i < 10000; i++)
    {
        v1.push_back(i);
        if (p != &v1[0])
        {
            p = &v1[0];
            count++;
        }
    }
    cout << "count:" << count << endl;  //24 -> 1
}
```