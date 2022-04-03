# 字典

## 1. 字典概念

* `dictionary`（字典）是除列表以外python之中最灵活的数据类型
* 字典同样可以用来存储多个数据
* 字典和列表的区别
  * 列表是有序的对象集合
  * 字典是无序的对象集合
* 字典用`{}`定义
* 字典使用`键值对`存储数据，键值对之间使用`,`分隔
  * `键key`是`索引`
  * `值value`是`数据`
  * `键`和`值`之间使用`:`分割
  * `键`必须是`唯一`的
  * `值`可以取`任何数据类型`，但`键`只能使用`数字`、`字符串`和`元组`

```python
ID_info_dic = {"姓名：":"张三", "身高：":1.75, "体重：":77.5,"年龄：":18}
```

## 2. 字典常用操作

### 2.1 取值

```python
info_dic = {"name:" : "Mike"}
info_dic["name:"] 	#Mike
```

### 2.2 增加/修改

```python
info_dic = {"name:" : "Mike"}
info_dic["age:"] = 18 		#原字典中 不存在 便增加
info_dic["name:"] = "Lisa" 	#原字典中 存在   便修改
```

### 2.3 删除

```python
info_dic = {"name" : "Mike", "weight":77.5}
info_dic.pop("name")
```

### 2.4 统计元素个数

```python
info_dic = {"name" : "Mike", "weight":77.5}
print(len(info_dic)) 	# 2
```

### 2.5 合并字典

```python
info_dic = {"name" : "Mike", "weight":77.5}
temp_info_dic = {"height":180}
info_dic.update(temp_info_dic)
print(info_dic) 	# {'name': 'Mike', 'weight': 77.5, 'height': 180}
```

### 2.6 清空字典

```python
info_dic = {"name" : "Mike", "weight":77.5}
info_dic.clear()
print(info_dic)		#{}
```

### 2.7 循环遍历字典

```python
info_dic = {"name" : "Mike", "weight":77.5}
for key in info_dic:
    print("%s : %s" % (key, info_dic[key]))
```



