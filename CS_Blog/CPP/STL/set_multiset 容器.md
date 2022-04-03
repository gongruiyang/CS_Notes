# set/multiset 容器

## set基本概念

- 所有元素都会在插入时自动被排序
- set/multiset属于**关联式容器**，底层结构是用二叉树实现
- set个multiset区别：
  - set**不允许**容器中有**重复元素**
  - multiset**允许**容器中有**重复元素**

## set构造和赋值

```cpp
/*
set构造和赋值
	有序不可重复
	set<T> name;			默认构造函数
	set(const set& name);	拷贝构造函数

	set& operator=(const set& name);
*/
void test01()
{
    //默认构造函数
	set<int> s1;
	s1.insert(4);
	s1.insert(4);
	s1.insert(3);
	s1.insert(2);
	s1.insert(1);
	printSet(s1);	//1 2 3 4	

    //拷贝构造函数
	set<int> s2(s1);
	printSet(s2);	//1 2 3 4	

    //=赋值
	set<int> s3;
	s3 = s2;
	printSet(s3);	//1 2 3 4
}
```

## set大小和交换

```cpp
/*
set大小和交换
	size();			//返回容器中元素的数目
	empty();		//判断是否为空
	swap(set);		//交换两个集合中的数据
*/
void test02()
{
	set<int> s1;
	s1.insert(4);
	s1.insert(4);
	s1.insert(3);
	s1.insert(2);
	s1.insert(1);
	printSet(s1);	//1 2 3 4	

	set<int> s2;
	s2.insert(9);
	s2.insert(5);
	printSet(s2);	//5 9
	
	//数据交换
	cout << "交换后：" << endl;
	s1.swap(s2);
	printSet(s1);	//5 9
	printSet(s2);	//1 2 3 4

	//计算容器大小
	cout << "大小：" << s1.size() << endl;	//2

	//判断容器是否为空
	cout << s1.empty() << endl;				//0
}
```



## set插入和删除

```cpp
/*
set插入和删除
	insert(elem);		插入数据
	clear();			清空数据

	erase(pos);			删除某位置元素
	erase(beg,end);		删除区间元素
	erase(elem);		删除容器中elem元素
*/
void test03()
{
	set<int> s1;
	s1.insert(8);
	s1.insert(7);
	s1.insert(6);
	s1.insert(5);
	s1.insert(4);
	s1.insert(3);
	s1.insert(2);
	s1.insert(1);

	printSet(s1);	//1 2 3 4 5 6 7 8
	s1.erase(s1.begin());
	printSet(s1);	//2 3 4 5 6 7 8
	
	s1.erase(++s1.begin(), --s1.end());
	printSet(s1);	//2 8

	s1.erase(8);
	printSet(s1);	//2

	s1.clear();
	printSet(s1);	//空
}
```



## set查找和统计

- set中数据不可重复，所以count函数返回值为0或1
- multiset中数据可重复，所以count函数返回值为相应值的个数

```cpp
/*
set查找与统计
	find(key);		查找key是否存在，若存在返回该键的元素的迭代器；若不存在返回set.end()
	count(key);		统计返回key的个数
*/
void test04()
{
	set<int> s1;
	s1.insert(8);
	s1.insert(7);
	s1.insert(6);
	s1.insert(5);
	s1.insert(4);
	s1.insert(3);
	s1.insert(2);
	s1.insert(1);

	set<int>::iterator it = s1.find(5);
	if (it != s1.end())
		cout << "找到了，在" << *it << "位置上" << endl;
	else
		cout << "找不到！" << endl;

	int set_num = s1.count(6);
	cout << "6的个数：" << set_num << endl;		//1

	multiset<int> ms;
	ms.insert(1);
	ms.insert(1);
	ms.insert(1);
	int ms_num = ms.count(1);
	cout << "1的个数：" << ms_num << endl;		//3
}
```



## set和multiset区别

- set插入数据时，会返回插入结果，表示是否插入成功
- multiset插入数据时，不会返回任何东西，不做任何检测

```cpp
/*
set的insert方法返回值：_Pairib insert(value_type&& _Val)
					typedef pair<iterator, bool> _Pairib;
multiset的insert方法返回值：iterator insert(value_type&& _Val)
*/
void test05()
{
	set<int> s1;
	pair<set<int>::iterator,bool> ret = s1.insert(1);
	if (ret.second)
		cout << "第一次插入成功！" << endl;
	else
		cout << "第一次插入失败！" << endl;

	ret = s1.insert(1);
	if (ret.second)
		cout << "第二次插入成功！" << endl;
	else
		cout << "第二次插入失败！" << endl;

	multiset<int> ms1;
	multiset<int>::iterator it = ms1.insert(1);
	cout << *it << endl;
	 
	it = ms1.insert(3);
	cout << *it << endl;
}
```



## pair对组创建

```cpp
/*
对组的创建
	默认构造方法
	make_pair
交换对组
	swap(pair);
*/
void test06()
{
	pair<string, int> p1("Tom", 18);
	pair<string, int> p2 = make_pair("Jerry", 18);
	cout << "Name:" << p1.first << " Age:" << p1.second << endl;
	cout << "Name:" << p2.first << " Age:" << p2.second << endl;

	p1.swap(p2);
	cout << "交换后：" << endl;
	cout << "Name:" << p1.first << " Age:" << p1.second << endl;
	cout << "Name:" << p2.first << " Age:" << p2.second << endl;
}
```



## 改变set容器排序规则

```cpp
/*
利用仿函数重写set内部排序方法
*/
class MySort{
public:
	bool operator()(int v1,int v2){
		return v1 > v2;
	}
};
void test07()
{
	set<int,MySort> s1;
	s1.insert(1);
	s1.insert(2);
	s1.insert(3);
	s1.insert(4);
	for each (int var in s1)
		cout << var << " ";
	cout << endl;
}
```

- **自定义类型**需要指定排序规则

```cpp
class Dog
{
public:
	Dog(){}
	~Dog(){}
	Dog(int age, int weight){
		this->age = age;
		this->weight = weight;
	}
	int getAge(){
		return this->age;
	}
	int getWeight(){
		return this->weight;
	}
	friend ostream& operator<<(ostream& os, Dog& d);
private:
	int age;
	int weight;
};
ostream& operator<<(ostream& os, Dog& d){
	cout << d.getAge() << " " << d.getWeight() << endl;
	return os;
}
class CompareDog{
public:
	bool operator()(Dog d1,Dog d2){
		return d1.getAge() > d2.getAge();
	}
};
void test08()
{
	set<Dog, CompareDog> s1;
	Dog d1(1, 2), d2(2, 3), d3(2, 4), d4(3, 4);
	s1.insert(d1);
	s1.insert(d2);
	s1.insert(d3);
	s1.insert(d4);

	for each (Dog var in s1)
	{
		cout << var;
	}
	cout << endl;
}
```

