# 1. 模拟实现set需要理解的概念

* 在 STL 中的 set 容器，其底层数据结构是`红黑树`
* set 中存储的数据是以 key - value 的形式存储的
* 用户存储的数据是放在 key 中的， value 用户是无法访问的



# 2. 模拟实现set代码

改代码可以运行的前提是需要[红黑树的模拟实现]()中的红黑树代码

> map.hpp

```cpp
#pragma once
#include "RBTree.hpp"	// 红黑树的模拟实现

// set 中防止的是
template<class K>
class set
{

	typedef K ValueType;
	struct KOFP
	{
		const K& operator()(const ValueType& data)
		{
			return data;
		}
	};
	typedef RBTree<ValueType, KOFP> RBT;
	typedef typename RBT::iterator iterator;
public:
	set() :_t(){}

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
	void swap(set<K>& s)
	{
		_t.swap(s._t);
	}
	void clear()const
	{
		_t.clear();
	}
	iterator find(const K& key)
	{
		return _t.find(key);
	}

private:
	RBT _t;
};
```

