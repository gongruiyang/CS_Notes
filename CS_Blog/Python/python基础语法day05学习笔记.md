#  字符串

## API测试

```python
# count函数
s ="hello world!"
print(s.count('o'))

# 判断整数函数
s = "一千零一"
print(s.isdigit())      # false
print(s.isnumeric())    # true
print(s.isdecimal())    # false

# 判断开头函数
s = "hello world!"
print(s.startswith("hello"))    # true
print(s.endswith("d!"))         # true

# find函数
s = "hello world!"
print(s.find("llo",0,len(s)))          # 2
print(s.find("abc"))          # -1

# replace函数
s = "hello world!"
s_new = s.replace("hello","python")
print(s_new)                        # python world!
print(s)                            # hello world!

# 对齐函数
poem = ["1", "23", "456", "789"]
for str in poem:
    print(str.rjust(15,'*'))

# 取出空格函数
s = "\t 111 11  1 1 1 \t\n"
s_new = s.strip()
print(s_new)		#111 11  1 1 1

# 按特定字符分割函数
s = "1234.888"
s_list = s.split('.')	# ["1234", "888"]
for i in s_list:
    print(i)		

# 按特定字符拼接字符串函数
s = "1234\n\t888\n\t999\n   000\t"
s_list = s.split()	# 默认以空格拆分
s_new = ".".join(s_list)	
print(s_new)	# 1234.888.999.000

# 字符串切片
s = "python"
print(s[1:6])	# ython
print(s[0:1])	# p
print(s[-1:])	# n
print(s[len(s)-1:len(s)])	# n
print(s[::-1])	# nohtyp 逆序
```



# 公共函数

| 函数              | 描述                              | 备注                        |
| ----------------- | --------------------------------- | --------------------------- |
| len(item)         | 计算容器中元素个数                |                             |
| del(item)         | 删除变量                          | del 有两种方式              |
| max(item)         | 返回容器中元素最大值              | 如果是字典，只针对 key 比较 |
| min(item)         | 返回容器中元素最小值              | 如果是字典，只针对 key 比较 |
| cmp(item1, item2) | 比较两个值，-1 小于/0 相等/1 大于 | Python 3.x 取消了 cmp 函数  |

**注意**

* **字符串** 比较符合以下规则： "0" < "A" < "a"



```python
s = "python"
print(max(s))	# y
print(min(s))	# h
```



# 运算符*与高级数据类型

```python
list1 = [1, 2, 3] * 3   # [1, 2, 3, 1, 2, 3, 1, 2, 3]
print(list1)
tuple1 = (1, 2, 3) * 3  # (1, 2, 3, 1, 2, 3, 1, 2, 3)
print(tuple1)
s = "hello" * 3     # hellohellohello
print(s)
list1.extend([[1, 2, 3], [3, 3, 3]])    # [1, 2, 3, 1, 2, 3, 1, 2, 3, [1, 2, 3], [3, 3, 3]]
print(list1)
```

