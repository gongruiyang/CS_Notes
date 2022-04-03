### String 容器

- **string 基本概念**

- - stirng是C++风格的字符串，而string本质上是一个类
  - char*是C风格的字符串

- **string和char\*的区别**

- - char* 是一个指针
  - string是一个类，类内部封装了char*，管理这个字符串，是一个char*型的容器

- **string的特点**

- - string类内部封装了很多**成员方法**，如：find、copy、delete、replace、insert
  - string管理char*所分配的内存，无需担心复制越界或取值越界问题

- **String 构造函数**

```c++
/*
构造函数
 - string();                    空string
   string(const char* s);       用c语言的str初始化string
 - string(const string& str);   用str初始化string
 - string(int n,char c);        用n个字符c初始化string
*/
void test01()
{
    string s1("abc");
    cout << "s1 = " << s1 << endl;

    string s2;
    cout << "s2 = " << s2 << endl;

    char *s = "abc";
    string s3(s);
    cout << "s3 = " << s3 << endl;

    string s4(s3);
    cout << "s4 = " << s4 << endl;

    string s5(10,'a');
    cout << "s5 = " << s5 << endl;

}
```

#### 

- **String 赋值操作**

```c++
/*
    string& operator=(const char* s);       
    string& operator=(const string& s);
    string& operator=(char c);
    string& assign(const char* s);
    string& assign(const char* s, int n);   s的前n个字符赋值给string
    string& assign(const string& s);
    string& assign(int n, char c);          将n个字符c赋值给string
*/
void test02()
{
    const char* s = "abc";
    string s1 = s;
    cout << "s1 = " << s1 << endl;

    string s2 = s1;
    cout << "s2 = " << s2 << endl;

    char c = 'a';
    string s3;
    s3 = c;
    cout << "s3 = " << s3 << endl;
    
    string s4;
    s4.assign(s);
    cout << "s4 = " << s4 << endl;

    string s5;
    s5.assign(s, 1);
    cout << "s5 = " << s5 << endl;

    string s6;
    s6.assign(s4);
    cout << "s6 = " << s6 << endl;

    string s7;
    s7.assign(10, 'a');
    cout << "s7 = " << s7 << endl;
}
```

- **string字符串拼接**

```c++
/*
string字符串拼接
    string& operator+=(const char* str);
    string& operator+=(const char c);
    string& operator+=(const string& str);
    string& append(const char* s);
    string& append(const char* s, int n);           将s字符串前n个字符追加在string尾部
    string& append(const string& s);
    string& append(const stirng& s,int pos,int n);  将s字符串从pos位置开始的n个字符追加在string尾部
*/
void test03()
{
    char *s = "C++";
    string str = "I like ";
    char c = '!';

    string s1;
    s1 += s;
    cout << "s1 = " << s1 << endl;

    string s2;
    s2 += (str + s1);
    cout << "s2 = " << s2 << endl;

    string s3;
    s3 += (s2 + c);
    cout << "s3 = " << s3 << endl;

    string s4 = "s4 ";
    s4.append(s);
    cout << "s4 = " << s4 << endl;

    string s5 = "s5 ";
    s5.append(s, 1);
    cout << "s5 = " << s5 << endl;

    string s6 = "s6 ";
    s6.append(s5);
    cout << "s6 = " << s6 << endl;

    string s7 = "s7 ";
    s7.append(s6,2,3);
    cout << "s7 = " << s7 << endl;
}
```

- **string查找和替换**

- - 查找：查找指定字符串是否存在

- - - 查到返回位置；查不到返回-1
    - find  是**从左往右**查
    - rfind 是**从右往左**查

- - 替换：在指定的位置替换字符串

```c++
/*
string字符串查找和替换
    从左往右查找：
    int find(const string& str, int pos = 0) const;     从pos开始查找str             第一次出现的位置
    int find(const char* s, int pos = 0) const;         从pos开始查找s               第一次出现的位置
    int find(const char* s, int pos, int n) const;      从pos开始查找s的前n个字符     第一次出现的位置
    int find(const char c, int pos = 0) const;          从pos开始查找字符c           第一次出现的位置
    
    从右往左查找：
    int rfind(const string& str,int pos = npos) const;  从pos开始查找str             最后一次出现的位置
    int rfind(const char* s,int pos = npos) const;      从pos开始查找s               最后一次出现的位置
    int rfind(const char* s,int pos,int n) const;       从pos开始查找s的前n个字符     最后一次出现的位置
    int rfind(const char c,int pos = 0) const;          从pos开始查找字符c           最后一次出现的位置
    
    替换
    string& replace(int pos, int n, const string& str);     从pos开始的n个字符替换为str字符串
    string& replace(int pos, int n, const char* s);         
*/
void test04()
{
    string s = "abcdefghijklmnopqrstuvwxyzabc";

    //int pos1 = s.find("ac");          //-1
    int pos1 = s.find("ab");            //0
    int pos2 = s.find("ab", 1);         //26
    int pos3 = s.find("ab", 3, 1);      //26
    int pos4 = s.find('a', 1);          //26

    int pos5 = s.rfind("ab");           //26
    int pos6 = s.rfind("ab",2);         //0
    int pos7 = s.rfind("ab", 3, 1);     //0
    int pos8 = s.rfind('a', 1);         //0

    s.replace(1, 5, "aaaaaa");          //aaaaaaaghijklmnopqrstuvwxyzabc
    cout << s << endl;
}
```

- **string字符串比较**：按照ASCII码进行对比

- -  = 返回 0
  -  \> 返回 1
  -  < 返回 -1



```c++
/*
string字符串比较
    int compare(const string& s) const;
    int compare(const char* s) const;
*/
void test05()
{
    string s  = "123456";
    string s1 = "123456";
    string s2 = "555555";
    string s3 = "111111";

    int equal = s.compare(s1);  //0
    int less = s.compare(s2);   //-1
    int great = s.compare(s3);  //1
    cout << equal << endl << great << endl << less << endl;
}
```

- **string中字符的存取**

```c++
/*
string字符存取
    char& operator[](int n);    //通过[]方式取 字符
    char& at(int n);            //通过at取    字符
*/
void test06()
{
    string s = "abcdabcd";
    char c1 = s[1];                     //b
    char c2 = s.at(1);                  //b
    cout << c1 << endl << c2 << endl;
}
```

- **string中字符的插入和删除**



```c++
/*
对string字符串进行插入和删除字符操作:  
    string& insert(int pos, const char* s);         //插入字符串
    string& insert(int pos, const string& str);     //插入字符串
    string& insert(int pos, int n, char c);         //在指定位置插入n个字符c
    string& erase(int pos, int n = npos);           //删除从pos开始的n个字符
*/
void test07()
{
    string s1 = "abc";
    s1.insert(1, "111");
    cout << s1 << endl;     //a111bc

    string s2 = "abc";
    s2.insert(3, "123");
    cout << s2 << endl;     //abc123

    string s3 = "abc";
    s3.insert(3, 2, '1');
    cout << s3 << endl;     //abc11

    string s4 = "abc";
    s4.erase(1, 2);
    cout << s4 << endl;     //a
}
```

- **string中插入和删除字符串**

```c++
/*
string中插入和删除字符串
    string substr(int pos = 0, int n = npos) const;     将从pos开始的n个字符组成的字符串返回
*/
void test08()
{
    string s = "abcdefghi";
    string substr = s.substr(1, 5);
    cout << substr << endl;         //bcdef
}
```