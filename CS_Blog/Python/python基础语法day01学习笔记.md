# 1. 注释

## 1.1 单行注释

> \# 后面加文字

```python
print("Hello!") # 这是一个 单行注释
```

## 1.2 多行注释

> 两个''''''之间

```python
"""
这是
多行
注释
"""
```

# 2.算术运算符

## 2.2 新运算符

| 运算符 | 描述         | 实例     |
| ------ | ------------ | -------- |
| //     | 对商向下取整 | 9//2 = 4 |
| **     | 幂           | 2**3 = 8 |

## 2.2 * 新用法

> "+"*10  就相当于将10个+拼在一起

```python
>>> "+"*3
'+++'
```

# 3. 查看解释器大小

```shell
[gongruiyang@localhost Learn_Python]$ which python
alias python='/usr/bin/python3.6'
	/usr/bin/python3.6
[gongruiyang@localhost Learn_Python]$ ls -lh /usr/bin/python3.6
-rwxr-xr-x. 2 root root 12K 4月   2 2020 /usr/bin/python3.6
```

> 由此可见 **Python3.6解释器大小为 12K**



# 4. 变量定义

> 使用 = 来给变量赋值
>
> * = 左边为变量名
> * = 右边为变量值

```python
QQ_name = "gongruiyang"
QQ_number = 123
QQ_pwd = 133.456
```

# 5.数据类型

## 5.1 基本数据类型

> Python不需要指定数据类型，Python解释器会根据 = 右侧的数据，**自动推导**出数据准确类型

```python
myName = "gongruiyang"	# str 字符串类型 字符串必须有双引号
myAge = 21				# int 整数类型
myWeight = 74.5			# float 浮点数类型
myGebder = false		# bool 布尔类型
```

| 数字型                                      | 非数字型 |
| ------------------------------------------- | -------- |
| 整形（int）                                 | 字符串   |
| 浮点型（float）                             | 列表     |
| 布尔型（bool）<br />true ： 1<br />false：0 | 元组     |
| 复数型（complex）                           | 字典     |

数字型变量 中可以直接进行 算术运算

## 5.2 使用type函数查看一个变量的类型

```python
a = 10 
type(a)			#int
type(2 ** 32) 	# long
```

## 5.3 使用+进行字符串拼接

```python
firstName = "gong"
lastName = "ruiyang"
name = firstName + lastName 	#gongruiyang
```

# 6. 标准输入流读取数据

> 使用input函数实现键盘键入
>
> * 在Python中可以使用 **input函数** 从 **键盘** 等待用户的输入
> * 用户输入的任何内容都被认为是一个 **字符串**

语法如下：

```python
变量名 = input("提示信息：")
```

演示样例：

```python
a = input("请输入数据：")
print(a)                 
```

# 7. 类型转换函数

| 类型转换函数 | 说明                  |
| ------------ | --------------------- |
| int(x)       | 将 x 转换为一个整数   |
| float(x)     | 将 x 转换为一个浮点数 |

演示样例：

```python
print(int("123"))			# 123
print(float("123.7"))		# 123.7
type(int("123"))			# int
type(float("123.7"))		# float
```

# 8. 变量的格式化输出

> 输出**文字信息**的同时，想要一起输出**变量值**，就要使用**格式化操作符**
>
> **%**被称之为**格式化操作符**
>
> **%**和不同的**字符**连用，**不同类型的数据** 需要使用 **不同的格式化字符**

| 格式化字符 | 含义                                                     |
| ---------- | -------------------------------------------------------- |
| %s         | 字符串                                                   |
| %d         | 十进制数，%06d表示输出显示6位数，不足的位补0             |
| %f         | 浮点数，%.02f表示小数点后只显示两位，小数点后不足的位补0 |
| %%         | 输出%                                                    |

* 语法格式

```python
print("格式化字符串"%变量名)
print("格式化字符串"%(变量名1，变量名2))
```

# 9. 练习：买苹果程序设计

```python
in_price = input("请输入苹果单价：")                                      
in_nums = input("请输入苹果数量：")                                       
price = float(in_price)                                                   
nums = int(in_nums)                                                       
cost = price * nums                                                       
print("总价为：%.02f\n单价为：%.02f\n数量为：%04d\n" % (cost,price,nums)) 
```

```shell
[gongruiyang@localhost Learn_Python]$ python BuyApple.py 
请输入苹果单价：10.8
请输入苹果数量：20
总价为：216.00
单价为：10.80
数量为：0020
```

# 10. 内置关键字

* 关键字 是Python内置标识符

```python
import keyword
print(keyword.kwlist)
```

> * **import关键字**可以导入一个**工具包**
> * **keyword工具包**中有**所有关键字**

```
['False', 'None', 'True', 'and', 'as', 'assert', 'break', 'class', 'continue', 'def', 'del', 'elif', 'else', 'except', 'finally', 'for', 'from', 'global', 'if', 'import', 'in', 'is', 'lambda', 'nonlocal', 'not', 'or', 'pass', 'raise', 'return', 'try', 'while', 'with', 'yield']
```

# 11.Python之父推荐的命名规范

![image-20210103222217866](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210103222217866.png)