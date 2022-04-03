# 1. priority_queue容器介绍

* `priority_queue`称为`优先级队列`
* 优先级队列总是能保证**优先级最高**的元素位于**队头**
* 其底层数据结构在**默认条件**下是一个**大顶堆**
* **默认条件**下插入数据，按照**严格的弱排序**条件，父结点上的数据总是大于子节点的数据

> 大顶堆图解

![image-20210131133804984](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210131133804984.png)



# 2. 成员函数介绍

## 2.1 构造函数

（1）优先级队列first是一个存放int型数据的空队列

```cpp
priority_queue<int> first;
```

（2）优先级队列second是使用数组a中前四个数据，按照为**less排序（默认规则）**规则形成**大堆**，**最大的数据**60位于队头

```cpp
int a[]= {10,60,50,20};
priority_queue<int> second (a, a+4);
```

（3）优先级队列third是使用数组a中前四个数据，按照**greater排序**方式形成**小堆**，**最小的数据**20位于队头

```cpp
int a[]= {10,60,50,20};
priority_queue< int, vector<int>, greater<int> >  third(a, a+4);
```

（4）使用 自定义排序方式之**重载比较运算符** 构造优先级队列

> 需要重载待比较类中的`<`或`>`,如果不重载比较运算符，**默认比较的是对象的地址**

```cpp
class MyInt{
private:
	int data;
public:
	MyInt(int data) :data(data){}
	bool operator<(const MyInt& x) const
	{
		return this->data < x.data;
	}
    bool operator>(const MyInt& x) const
	{
		return this->data > x.data;
	}
};

void Test1()
{
	MyInt a(0);
	MyInt b(1);
	MyInt c(2);

	priority_queue<MyInt> q;
	q.push(a);
	q.push(b);
	q.push(c);
}
```

> 最大值位于队首
>
> ![image-20210131140937979](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210131140937979.png)

```cpp
priority_queue< MyInt, vector<MyInt>, greater<MyInt> > q;
```

> 最小值位于队首
>
> ![image-20210131144225608](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210131144225608.png)

（5）使用 自定义排序方式之**比较器（仿函数）** 构造优先级队列

```cpp
class MyInt{
private:
	int data;
public:
	MyInt(int data) :data(data){}
	bool operator<(const MyInt& x) const
	{
		return this->data < x.data;
	}
	bool operator>(const MyInt& x) const
	{
		return this->data > x.data;
	}
	int getData()
	{
		return data;
	}
};

struct MyCompare
{
	bool operator()(MyInt left, MyInt right)
	{
		return left.getData() < right.getData();
	}
};

void Test1()
{
	MyInt a(0);
	MyInt b(1);
	MyInt c(2);
	MyInt d(3);
	MyInt e(4);

	priority_queue< MyInt, vector<MyInt>, MyCompare > q;
	q.push(a);
	q.push(b);
	q.push(c);
	q.push(d);
	q.push(e);
	
}
```

>比较器中使用的是**greater规则**来排序数据，作用相当于在类中重载大于号运算符
>
>![image-20210131142455653](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210131142455653.png)

## 2.2 方法函数

| 函数  | 含义                 |
| ----- | -------------------- |
| empty | 测试容器是否为空     |
| size  | 返回容器中数据的个数 |
| top   | 获取队头元素的引用   |
| push  | 插入数据             |
| pop   | 删除队首元素         |

（1）empty函数

```cpp
bool empty ( ) const;
```

* 返回值：true表示优先级队列为空，false表示容器不为空

（2）size函数

```cpp
size_type size ( ) const;
```

* 返回值：返回优先级队列中数据的个数

（3）top函数

```cpp
const value_type& top ( ) const;
```

* 返回值：返回队首元素的**引用**

（4）push函数

```cpp
void push ( const T& x );
```

* 向优先级队列中插入一个数据，push函数先调用底层容器的push_back函数来插入新数据，再调用push_heap算法保持优先级队列中堆的属性

（5）pop函数

```cpp
void pop ( );
```

* 删除队首元素，ppop函数先调用pop_heap算法来保证优先级队列中堆的属性，在调用底层容器的pop_back函数来移除数据

# 3. 模拟实现

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

namespace PRIORITY_QUEUE
{
	template<class T, class Container = vector<T>/*, class Com = less<T>*/>
	class priority_queue
	{
	private:
		Container _con;
	public:
		priority_queue()
		{}
		template<class Iterator>
		priority_queue(Iterator begin, Iterator end)
			:_con(begin, end)
		{
			make_heap(_con.begin(), _con.end());
		}

		void push(const T& data)
		{
			_con.push_back(data);
			push_heap(_con.begin(), _con.end());
		}
		void pop()
		{
			pop_heap(_con.begin(), _con.end());
			_con.pop_back();
		}
		T& top(){ return _con.front(); }
		bool empty(){ return _con.empty(); }
		size_t size(){ return _con.size(); }
	};//class end
}
```

