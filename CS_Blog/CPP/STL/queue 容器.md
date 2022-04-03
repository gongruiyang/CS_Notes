# queue 容器

## 基本概念

- - 概念：Queue是一种先进先出（FIFO）的数据结构，它有两个出口



![image.png](https://cdn.nlark.com/yuque/0/2020/png/1693220/1598015688335-9229d790-2b2c-45be-bd08-296b9990ec26.png)



## 常用接口

```cpp
/*
构造函数
    queue<T> name;  默认构造函数
    queue(const queue& name);   拷贝构造函数
赋值操作
    queue& operator=(const queue& name);    重载=运算符
数据存取
    push(elem);     尾插
    pop();          头删
    back();         获取队尾部元素
    front();        获取队头部元素
大小操作
    empty();        判断队列是否为空
    size();         返回队列中元素个数
*/
void test01()
{
    queue<int> q1;
    q1.push(1);
    q1.push(3);
    q1.push(5);
    q1.push(2);
    q1.push(4);

    int back = q1.back();
    int front = q1.front();
    
    queue<int> q2(q1);
    queue<int> q3 = q2;
    
    cout << "back:" << back << endl;
    cout << "front:" << front << endl;
    cout << "size:" << q1.size() << endl;
    cout << "IsEmpty ? " << q1.empty() << endl;

}
```