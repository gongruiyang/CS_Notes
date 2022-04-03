

# for...else循环

`语法格式`

```python
for 变量 in 集合 :
    # TO DO
else :
    # 如果for中执行了break,则不执行else中的代码
    # 如果for顺利执行完毕，则执行else中的代码
```

`演示程序1`

```python
for num in [1, 2, 3, 4, 5, 6]:
    print(num)
    if num == 5:
        break
else:
    print("else")    
print("for done")
```

`输出`

```
1
2
3
4
5
for done
```

`演示程序2`

```python
students_info = [
    {"name": "小美", "sex":"nv"},
    {"name": "张三", "sex":"nan"}
]
find_name = "张三"
for i in students_info:
    if i["name"] == find_name:
        print("找到了！")
        break
else:
    print("没找到！")
print("for done")
```

`输出`

```
找到了！
for done
```



# 变量的引用

`id函数`

![image-20210109222335361](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210109222335361.png)



```python
a = 1
b = a
print(id(a))
print(id(1))
print(id(b))
a = 2
print(id(a))
print(id(2))
```

```
140718941434112
140718941434112
140718941434112
140719122968864
140719122968864
```



`函数参数传递的是引用`

```python
def func(num1, num2):
    print(id(num1))
    print(id(num2))
num1 = 2
num2 = 2
print(id(num1))
print(id(num2))
print(id(2))
func(num1,num2)
```

```
140719116284192
140719116284192
140719116284192
140719116284192
140719116284192
```

