# 1. 模拟实现map需要理解的概念

* STL中的map的底层数据结构是一个`红黑树`
* map 中存储数据是以` key - value `的形式存储的

# 2. 模拟实现map

以下代码可以运行的前提是，拥有[红黑树的模拟实现]()中红黑树的代码

> map.hpp

```cpp
#include "RBTree.hpp"	// 红黑树的模拟实现

// map 中放置的是 K—V 模型
template<class K, class V>
class map
{
	typedef pair<K, V> ValueType;
	struct KOFP	// 为了方便红黑树中的插入：比较的是K而不是Pair结构体地址
	{
		const K& operator()(const ValueType& data)
		{
			return data.first;
		}
	};

	typedef RBTree<ValueType, KOFP> RBT;
public:
	// iterator可能是红黑树中的类型，也可能是静态成员变量，需要显式说明
	typedef typename RBT::iterator iterator;

	map() :_t(){}

	// iterator
	iterator begin()
	{
		return _t.begin();
	}
	iterator end()
	{
		return _t.end();
	}

	// capacity
	size_t size()
	{
		return _t.size();
	}
	bool empty()
	{
		return _t.empty();
	}

	// modify
	pair<iterator, bool> insert(const ValueType& data)
	{
		return _t.insert(data);
	}
	void swap(map<K, V>& m)
	{
		_t.swap(m._t);
	}
	void clear()const
	{
		_t.clear();
	}
	iterator find(const K& key)
	{
		return _t.find(key, V());
	}

	// acess
	V& operator[](const K& key)
	{
		return (*(_t.insert(make_pair(key, V()))).first).second;
	}
private:
	RBT _t;
};
```

