# 1. stack和queue源码

> * 由stack和queue源码可知，其实stack和queue是将deque容器进行再封装，其底层是一个deque容器
> * 对stack和queue操作，其实间接操作的是deque容器

`stack源码`

```cpp
template<class _Ty, class _Container = deque<_Ty> >
class stack
{   // LIFO queue implemented with a container
protected:
    _Container c;   // the underlying container
public:
    // test if stack is empty
    bool empty() const { return (c.empty()); }

    // test length of stack
    size_type size() const { return (c.size()); }

    // return last element of mutable stack
    reference top() { return (c.back()); }

    // return last element of nonmutable stack
    const_reference top() const { return (c.back()); }

    // insert element at end
    void push(const value_type& _Val) { c.push_back(_Val); }

    // erase last element
    void pop() { c.pop_back(); }
};
```

`queue源码`

```cpp
template<class _Ty, class _Container = deque<_Ty> >
class queue
{   
protected:
    _Container c;   // the underlying container
public:
    // test if queue is empty
    bool empty() const { return (c.empty()); }

    // return length of queue
    size_type size() const { return (c.size()); }

    // return first element of mutable queue
    reference front() { return (c.front()); }

    // return first element of nonmutable queue
    const_reference front() const { return (c.front()); }

    // return last element of mutable queue
    reference back() { return (c.back()); }

    // return last element of nonmutable queue
    const_reference back() const { return (c.back()); }

    // insert element at beginning
    void push(const value_type& _Val) { c.push_back(_Val); }

    // erase element at end
    void pop() { c.pop_front(); }
};
```

# 2. deque介绍

* deque（双端队列）：**是一个双开口的“连续空间”的数据结构**
* `双开口`：可以在首尾两端进行插入和删除操作
* `连续空间`：deque并不是真正连续的，而是由一段段连续的小空间组合而成的
* deque类似于一个**动态的二维数组**

# 3. deque底层数据结构解析

* 数据被以此存储在缓冲区中
* deque没有容量的概念，因为它是动态地以分段连续空间组合而成，随时可以增加一段新的空间并链接起来

![image-20210206150638833](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210206150638.png)

# 4. map与node-buffer扩容图解

* deque采用**一块map作为主控**，这里的map并非STL中的map容器，而是类似于动态一维数组的**一小块连续的空间**，其中每一个元素（node）都是一个**指针**，这个指针指向**另一段较大的连续线性空间**
* 中控区中指针指向的内存段称为**缓冲区**，缓冲区才是deque的**存储空间主体**

![image-20210206150821130](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210206150821.png)

# 5. deque的迭代器与map与buffer之间的关系

* deque是分段连续的，其`迭代器operator++和operator--`维持着其“整体连续”的假象

> deque迭代具有的结构：
>
> * cur：迭代器表示的当前元素
> * first：缓冲区Buffer的起始位置
> * last：缓冲区Buffer的结束为止
> * node：保存此时位于map中的哪一个指针

![image-20210207123912792](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210207123920.png)

# 6. deque的数据成员

deque源码中定义的数据成员

```cpp
protected:                      // Data members
  iterator start;	// 第一个缓冲区
  iterator finish;	// 最后一个缓冲区

  map_pointer map;	// map缓冲区， map_pointer  ==  T**
  size_type map_size;	// map内有多少个指针
```

* 两个迭代器：start迭代器和finish迭代器
* map二级指针：指向map中控区
* map_size：保存中控区的大小

# 7. deque::begin()和deque::end()

* deque::begin()返回start迭代器
* deque::end()返回finish迭代器
* start迭代器和finish迭代器都是deque的数据成员

```cpp
iterator begin() { return start; }
iterator end() { return finish; }
const_iterator begin() const { return start; }
const_iterator end() const { return finish; }
```

> 假设：
>
> 1个缓冲区Buffer是32字节，可以容纳8个int型数据
>
> 通过操作使得deque中保存了20个int型数据
>
> 20个数据需要3个Buffer来存储

![image-20210207124627070](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210207124627.png)

# 8. 迭代器的operator++和operator--源码解读

```cpp
void set_node(map_pointer new_node) 
{
    node = new_node;
    first = *new_node;
    last = first + difference_type(buffer_size());
}
self& operator++() 
{
	++cur;	// 切换至下一个元素
	if (cur == last) 	//如果cur达到所在缓冲区尾端
	{
		set_node(node + 1);	//	切换至下一个节点
		cur = first;	// cur指向新缓冲区的首端
	}
	return *this;	
}
self& operator--() 
{
	if (cur == first) 	// 如果cur达到所在缓冲区的首端
	{
		set_node(node - 1);	// 切换至上一个节点
		cur = last;	// cur指向新缓冲区的尾端
	}
	--cur;	//	切换至前一个元素
	return *this;
}
```

# 9. 其他成员函数源码解读

```cpp
reference front() { return *start; }	//返回start中cur指向的元素
reference back() {
    iterator tmp = finish;	// 临时迭代器指向finish
    --tmp;	//让tmp的cur指向最后一个元素的起始地址
    return *tmp;	//返回最后一个元素的值
}
size_type size() const { return finish - start; }	
bool empty() const { return finish == start; }
```

