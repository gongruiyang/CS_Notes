# map容器

## 1. 基本概念

* map是关联式容器，按照key的大小来存储由`<key,value>`组合而成的元素
* 键值key通常用来唯一的标识元素，即**key不可重复，value可重复**
* map底层实现为`红黑树`，红黑树是`平衡二叉树`

**map模板参数说明**

![image-20210217110500369](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210217110507.png)

> * key： 键值对中key的类型
> * T： 键值对中value的类型
> * Compare:：比较器的类型，map中的元素是按照key来比较的，缺省情况下按照小于来比较，一般情况下(内置类型元素)该参数不需要传递，如果无法比较时(**自定义类型**)，需要用户自己**显式传递比较规则**(一般情况下按照函数指针或者仿函数来传递)
> * Alloc：通过空间配置器来申请底层空间，不需要用户传递，除非用户不想使用标准库提供的空间配置器
> * 注意：在使用map时，需要包含头文件。



## 2. 构造函数

```cpp
map (const key_compare& comp = key_compare(),
     const allocator_type& alloc = allocator_type());
```

> 构造一个空的map

##  3. 迭代器

```cpp
/* begin:首元素的位置，end最后一个元素的下一个位置 */
iterator begin();	
iterator end();

/* 与begin和end意义相同，但是所指向的元素不能修改 */
const_iterator begin();
const_iterator end();

/* 与begin和end意义相同，但cbegin和cend所指向的元素不能修改 */
const_iterator cbegin();
const_iterator cend();

/* 反向迭代器，rbegin在end位置，rend在begin位置，其++和--操作与begin和end操作移动相反 */
reverse_iterator rbegin();
reverse_iterator rend();

/* 与rbegin和rend位置相同，但所指向的元素不能修改 */
const_reverse_iterator rbegin();
const_reverse_iterator rend();

/* 与rbegin和rend位置相同，但crbegin和crend所指向的元素不能修改 */
const_reverse_iterator crbegin();
const_reverse_iterator crend();
```

## 4. 容量与元素访问

【容量】

```cpp
bool empty();	// 检测map中的元素是否为空，是返回true，否则返回false
size_type size();	// 返回map中有效元素的个数
size_type max_size();	// 返回map最大容纳量
```



【元素访问】

```cpp
/* 返回key对应的value */
mapped_type& operator[] (const key_type& k);
mapped_type& operator[] (key_type&& k);
```

> * key存在时：通过key找到与key对应的value然后返回其引用  
> * key不存在时：operator[]用默认value与key构造键值对然后插入，返回该默认value



```cpp
/* 返回key对应的value */
mapped_type& at (const key_type& k);
const mapped_type& at (const key_type& k) const;
```

> * key存在时：通过key找到与key对应的value然后返回其引用  
> * key不存在时：抛出异常



```cpp
iterator find (const key_type& k);
const_iterator find (const key_type& k) const;
```

> * key存在时：返回key元素的迭代器
> * key不存在时：返回 map::end() 迭代器



```cpp
size_type count (const key_type& k) const;
```

> * 返回map中含有该key的数量：0或1
> * 常用来检测某个元素是否存在于map中



## 5. 构造键值对

【使用pair类】

```cpp
template <class T1, class T2> struct pair;
```

> * pair类将一对不同类型的值（T1和T2）耦合在一起。单个值可以通过其公共成员(first和second)访问
> * pair是tuple的特例
> * first：pair中的第一个值
> * second：pair中的第二个值
> * first_type：pair中的第一个值的数据类型
> * second_type：pair中的第二个值的数据类型

【make_pair函数】

```cpp
template <class T1, class T2>
pair<V1,V2> make_pair (T1&& x, T2&& y); 
```

> * 作用：构造一个pair对象，第一个元素设置为x，第二个元素设置为y
> * 元素类型可由元素值隐式推导出来

【代码演示】

```cpp
pair<int, string> pair1 = make_pair(1, "first");
pair<int, string> pair2(2, "second");
pair<int, string> pair3(pair1);
pair3.first = 3;
pair3.second = "third";
```

## 6. 元素修改

### 6.1 value_type宏定义

```cpp
typedef pair value_type;
```

> 这里的value_type就是一个pair类

### 6.2 插入

```cpp
/* 单个元素插入 */
pair<iterator,bool> insert (const value_type& val);	// 插入一个键值对

/* 指定插入位置 */
iterator insert (const_iterator position, const value_type& val);

/* 范围插入 */
template <class InputIterator>
void insert (InputIterator first, InputIterator last);

/* 初始化列表 */
void insert (initializer_list<value_type> il);

/*****************************************************/
/* 测试代码 */
/*****************************************************/
map<char, int> myMap;

/* single insertion */
pair<map<char, int>::iterator, bool> ret_sig = myMap.insert(pair<char, int>('a', 1));
if (ret_sig.second)	cout << "single insertion success!" << endl;

/* position insertion */
map<char, int>::iterator pos = myMap.begin();
map<char, int>::iterator ret_pos = myMap.insert(pos, pair<char, int>('b', 2));

/* range insertion */
map<char, int> anotherMap;
anotherMap.insert(myMap.begin(), myMap.find('a'));

/* initializer_list insertion */
myMap.insert({ { 'c', 3 }, { 'd', 4 }, { 'e', 5 } });

for (auto e : myMap)
    cout << e.first << "--->" << e.second << endl;
```

> **输出**：
>
> ```cpp
> single insertion success!
> a--->1
> b--->2
> c--->3
> d--->4
> e--->5
> ```



### 6.3 删除

```cpp
/* 按照位置删除 */
iterator  erase (const_iterator position);

/* 按照k值删除 */
size_type erase (const key_type& k);

/* 按照范围删除 */
iterator  erase (const_iterator first, const_iterator last);

/*****************************************************/
/* 测试代码 */
/*****************************************************/
map<char, int> map1({{ 'a', 1 }, { 'b', 2 }, { 'c', 3 }, 
                     { 'd', 4 }, { 'e', 5 }, { 'f', 6 }});
map1.erase('f');
map1.erase(map1.find('e'));
map1.erase(map1.begin(), map1.find('b'));
for (auto e : map1)
    cout << e.first << "--->" << e.second << endl;
```

> 输出：
>
> ```cpp
> b--->2
> c--->3
> d--->4
> ```

### 6.4 修改

```cpp
mapped_type& operator[] (const key_type& k);
mapped_type& operator[] (key_type&& k);

/*****************************************************/
/* 测试代码 */
/*****************************************************/
map<char, int> myMap({ { 'a', 1 }, { 'b', 2 } });
myMap['a'] = 4;
myMap['c'] = 3;
for (auto e : myMap)
    cout << e.first << "--->" << e.second << endl;
```

> **输出**：
>
> ```cpp
> a--->4
> b--->2
> c--->3
> ```

### 6.5 清空

```cpp
void clear();

/*****************************************************/
/* 测试代码 */
/*****************************************************/
map<char, int> map1({ { 'a', 1 }, { 'b', 2 }, { 'c', 3 } });
map.clear();
```

### 6.6 交换数据

```cpp
void swap (map& x);

/*****************************************************/
/* 测试代码 */
/*****************************************************/
map<char, int> map1({ { 'a', 1 }, { 'b', 2 }, { 'c', 3 } });
map<char, int> map2({ { 'd', 4 }, { 'e', 5 }, { 'f', 6 } });
map1.swap(map2);
```

> 输出：
>
> ```cpp
> ========= map1 =========
> a--->1
> b--->2
> c--->3
> ========= map2 =========
> d--->4
> e--->5
> f--->6
> after swap
> ========= map1 =========
> d--->4
> e--->5
> f--->6
> ========= map2 =========
> a--->1
> b--->2
> c--->3
> ```

# multimap容器

* multimap是关联式容器，数据组织与map容器相同
* 底层数据结构也是红黑树
* multimap的key是可以重复的
* multimap中没有重载operator[]