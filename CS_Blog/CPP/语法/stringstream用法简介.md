# 1. 将int转化为string

```cpp
#include <iostream>
#include <sstream>
using namespace std;

int main()
{
	stringstream ss;
	ss.str("");	// 清空ss

	// int -> string
	int a = 123456;
	string s;
	ss << a;	
	ss >> s;

	cout << s << endl;

	system("pause");
	return 0;
}
```



# 2. 字符串拼接

```cpp
#include <iostream>
#include <sstream>
using namespace std;

int main()
{
	stringstream sstream;
	// 将多个字符串放入 sstream 中
	sstream << "first" << " " << "string,";
	sstream << " second string";
	cout << "strResult is: " << sstream.str() << endl;

	// 清空 sstream
	sstream.str("");
	sstream << "third string";
	cout << "After clear, strResult is: " << sstream.str() << endl;

	system("pause");
	return 0;
}
```

【注意】

1. stringstream实际是在其底层维护了一个**string类型的对象**用来保存结果。
2. 多次数据类型转化时，一定要**用clear()来清空**，才能正确转化，但clear()不会将stringstream底层的string对象清空。
3. 可以使用**s. str("")**方法将**底层string对象设置为空字符串**。
4. 可以使用**s.str()**将让stringstream**返回其底层的string对象**。
5. stringstream使用string类对象代替字符数组，可以避免缓冲区溢出的危险，而且其会对参数类型进行推演，不需要格式化控制，也不会出现格式化失败的风险，因此使用更方便，更安全。  