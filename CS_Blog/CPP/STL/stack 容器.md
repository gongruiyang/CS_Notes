# stack 容器

## 基本概念

- 概念：stack是一种**先进后出**（FILO）的数据结构，它只有一个出口。
- 由于栈只有栈顶可以对外界访问，所以**栈不可以遍历**

![image.png](https://cdn.nlark.com/yuque/0/2020/png/1693220/1598064950962-a0303f3d-5023-438f-813f-189c77ca46f9.png)

## 常用接口

```cpp
/*
构造函数：
    stack<T> name;
    stack<T>(const stack& name);    拷贝构造函数
赋值操作：
    stack& operator=(const stack& name);
数据存取：
    push(elem);     进栈
    pop();          出栈
    top();          取栈顶
大小操作：
    empty();    判断栈是否为空
    size();     求栈长度
*/
void test01()
{
    stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);
    s.push(4);

    stack<int> s1(s);

    stack<int> s2;
    s2 = s1;

    while (!s2.empty())
    {
        cout << "栈顶元素：" << s2.top() << endl;
        s2.pop();
    }
    cout << "栈大小：" << s2.size() << endl;
}
```