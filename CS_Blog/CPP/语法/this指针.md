# this指针

## 什么是this指针？

> this：是一个指针，这个指针中保存的是**当前对象**的地址，指向**本对象**
>
> 作用域：非静态成员函数内部
>
> 用途：当类的**非static成员函数**访问**非static成员变量**时，编译器会**自动**将**对象本身的地址**作为一个**隐含的形参**传给函数（也就是说：即使你没有显式的将本对象地址传入函数中，编译器在编译的时候也会自动将本对象地址传给函数），访问各个成员均可以通过this进行

## 验证编译器暗地中将本对象地址传入函数

> 验证程序

```cpp
#include <iostream>
using namespace std;
class Studen
{
public:
	Studen(){}
	~Studen(){}
	void initializeStudent(string name, int id)
	{
		this->Name = name;
		this->ID = id;
	}
private:
	string Name;
	int ID;
};
int main()
{
	Studen s1, s2;
	s1.initializeStudent("小明", 001);
	s2.initializeStudent("小李", 002);

	system("pause");
	return 0;
}
```

![image-20201024165354339](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20201024165354339.png)

> 由上述程序的运行initializeStudent函数的**汇编代码**可以看出：在s1和s2分别调用initializeStudent函数时，编译器分别将s1的地址和s2的地址分别传入了initializeStudent函数中，由此可见，虽然编写程序的人未显式将本对象地址传入函数中，编译器暗地里将本对象的地址传入了函数



> 所以下面两个函数是等价的

```cpp
	void initializeStudent(string name, int id)
	{
		this->Name = name;
		this->ID = id;
	}
	void initializeStudent(Studen* This, string name, int id)
	{
		This->Name = name;
		This->ID = id;
	}
```



## this指针特点

- 只能在类的非static成员函数中使用

> 由下图可以看出：this只能在非静态成员函数中使用
>
> 原因：静态成员函数的调用不依赖于对象

![image-20201024171705703](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20201024171705703.png)



- this指针的数据类型：类名* const this

  ![image-20201024173312598](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20201024173312598.png)

- this指针**主要**通过ecx寄存器来传递

![image-20201024171853310](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20201024171853310.png)

![image-20201024171911291](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20201024171911291.png)

- this指针的指向不可以改变

![image-20201024173555194](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20201024173555194.png)

> 试图修改this的指向时报错误：
>
> 错误	1	error C2106: “=”: 左操作数必须为可修改对的左值
>
> 说明this不可修改指向

- this指针并非成员变量，所以不影响类的大小

## this指针的应用场景

- 场景一：获取本对象的地址

```cpp
#include <iostream>
using namespace std;
class A
{
public:
	void getAdd()
	{
		cout << "a1内存地址为：" << this << endl;
	}

};
int main()
{
	A a1;
	cout << "a1内存地址为：" << &a1 << endl;
	a1.getAdd();

	system("pause");
	return 0;
}
```

> 输出：
>
> a1内存地址为：00F6FD2B
> a1内存地址为：00F6FD2B



- 场景二：区别形参与成员变量

```cpp
class Student
{	
private:
	char Name[10];
	int ID;
public:
	Student(){}
	Student(char *Name, int ID)
	{
		strcpy(this->Name, Name);
		this->ID = ID;
	}
};
```

> 在形参和成员变量名字一样的情形下，可以使用this来区别变量名字



- 场景三：获取当前对象本身

```cpp
	Student getObj()
	{
		return *this;
	}
```

