# 1. 列表

## 1.1 概念

* `List`（列表）是Python中的一种数据容器，在其他语言中叫``数组``

![image-20210106205544016](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210106205544016.png)

## 1.2 定义

```python
列表名 = []
```



`字符串列表样例`

```python
anme_list = ["张三","李四","王五","赵六"]
```

`整形列表样例`

```python
nums_list = [0,1,2,3,4,5,6,7,8]
```

`混合数据类型列表`

```python
m_list = ["张三", 3, 3,16]
```



## 1.3 取数据或取下标

### 1.3.1 根据下标取内容

```python
name_list[0]  # 张三
```



### 1.3.2 根据内容取下标

```python
name_list.index("张三") 	# 0
name_list.index("赵四") 	#ValueError: '赵四' is not in list
```

## 1.4 修改数据

### 1.4.1 根据下标修改数据

```python
name_list[0] = "zhangsan"
```

## 1.5 增加数据

### 1.5.1 append函数

* `append`函数可以向**列表末尾**追加数据

```python
name_list.append("王小二")
```

### 1.5.1 insert函数

* `insert`函数可以在**列表指定位置**上插入数据

```python
name_list.insert(1,"lisi")
```

### 1.5.2 extend函数

* 追加一个列表中的所有数据至末尾

```python
temp_name_list = [1, 2, 3]
name_list.extend(temp_name_list)	#字符串和数字混合列表
```



## 1.6 删除数据

### 1.6.1 remove函数

* `remove`函数删除指定内容的数据

```python
name_list.remove("张三")
```

### 1.6.2 pop函数

* `pop`函数无参数情况下，默认删除最后一个元素，传入索引使删除指定位置的数据

```python
name_list.pop()		#删除最后一个元素
name_listpop(0)		#删除第0个元素
```

### 1.6.3 clear函数

* `clear`函数清空列表中的所有数据

```python
name_list.clear() 
```



### 1.6.4 del关键字（不建议使用）

* `del`关键字是将数据从`内存`中删除，不建议使用

```python
del name_list[0]
```



## 1.7 列表统计

### 1.7.1 len函数求列表长度

* `len`可以求出指定容器的数据个数
* 该函数`返回值`为容器长度

```
nums_list = [1,2,3,4,5,6,7,8,9,10]
list_length = len(nums_list)	# 10
```

### 1.7.2 count函数统计元素频数

* `count`函数可以统计一个元素在list中出现的次数
* 该函数`返回值`为出现次数

```python
nums_list = [1,1,1,4,5,6,7,8,9,10]
count = nums_list.count(1)	# 3
```



## 1.8 列表排序

### 1.8.1 sort函数

* `sort`函数将列表默认进行`升序`排序
* 参数为`reverse=true`为进行`降序`排序

```
nums_list = [10,9,8,7,6,5,4,3,2,1]
nums_list.sort() 	# 1,2,3,4,5,6,7,8,9,10

```

### 1.8.2 reverse函数

* `reverse`函数将列表数据逆置

```python
nums_list = [10,9,8,7,6,5,4,3,2,1]
nums_list.reverse() 	# 1,2,3,4,5,6,7,8,9,10
```

## 1.9 列表的遍历

### 1.9.1 for循环遍历

```python
nums_list = [1,2,3]
for i in nums_list :
    print("%d " % i,end="")
print("")
# 1 2 3 
```

# 2. 元组

* `Tuple`（元组）与列表相似，不同之处在于元组的 **元素不能修改**
* 元组索引从0开始，数据之间用`,`分离

```
ID_info_tuple = ("张三",18,1.75)
```

## 2.1 count函数

* `count`函数统计一个数据在`tuple`中出现的次数

```python
ID_info_tuple.count("张三")	# 1
```

## 2.2 index函数

* `index`函数根据内容返回对应下标

```python
ID_info_tuple.index("张三") 	# 0
```

