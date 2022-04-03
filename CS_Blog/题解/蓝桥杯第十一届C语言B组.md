# 试题A：门牌制作

## 题目

![image.png](https://cdn.nlark.com/yuque/0/2020/png/1693220/1602995550511-e88ab94a-f6ad-4f42-a140-59cc6e5bb83f.png)

## 参考解题方法

### 思路

> 思路：遍历1-2020，将每个数字进行逐位分解，统计一共有多少个2
>
> 参考答案：624

### 代码

```cpp
int count2(int num) //统计每个数字有多少个2
{
    int count = 0;
    while (num)
    {
        int x = num % 10;
        if (x == 2) count++;
        num /= 10;
    }
    return count;
}
void A()
{
    const int end = 2020;
    const int begin = 1;
    int count = 0;      //统计 1 - 2020 一共多少个2
    for (size_t i = begin; i <= end; i++)   //遍历 1 - 2020
    {
        count += count2(i);
    }
    cout << count << endl;
}
```



# 试题B：既约分数

## 题目

![image.png](https://cdn.nlark.com/yuque/0/2020/png/1693220/1602996087995-d730c7b4-fc43-4138-91ef-f3bff5149451.png)

## 参考解题方法

### 思路

> 1.分子与分母互质
>
> 2.互质的两个数，例如1 和 2，其中2既可以当分母又可以当分子，所以只考虑分子小于分母的，然后乘以2，可以减少代码量
>
> 3.由于分子和分母都是可以取 1 - 2020，所以 1 / 1 也符合题意，最后加上这组数字
>
> 参考答案：2481215

### 代码

```cpp
bool isRelativePrime(int a, int b)  //两个数互质返回true，否则返回false
{
    int min = a > b ? a : b;
    for (size_t i = 2; i <= min; i++)
    {
        if (a%i == 0 && b%i == 0)   return false;
    }
    return true;
}
void B()
{
    const int begin = 1;
    const int end = 2020;
    int count = 0;

    for (size_t i = begin; i < end; i++)
        for (size_t j = i+1; j <= end; j++)
            if (isRelativePrime(i, j))  
                count++;
            
    int ans = count * 2 + 1;    //由于只考虑分子小于分母的情况，所以需要二倍 再 加一个 1/1
    cout << ans << endl;
}
```

# 试题C：蛇形填数

## 题目

![image.png](https://cdn.nlark.com/yuque/0/2020/png/1693220/1602997367090-0a71c522-055f-48e9-8fbc-e754b891d1e1.png)

## 参考解题方发

### 思路

> ![image.png](https://cdn.nlark.com/yuque/0/2020/png/1693220/1602997958737-9ea2d3b2-852f-4c6b-8519-32ce6e7c68a8.png)
>
> 我们先看第3行第3列的数字，可以发现13 = 前（3 + （3-1）-1）排数字个数 + 第（3+2）排数字个数/2 + 1
>
> 所以可得以下规律：第n排n列数字 = 前（n + (n-1) -1)排数字个数 + 第（n + (n-1)排数字个数/2 + 1
>
> 所以第20排20列数字 = 前38排数字总和 + 第39排数字个数/2 + 1
>
> 参考答案：761

### 代码

```cpp
int num(int n)
{
    int nums = 0;
    for (size_t i = 1; i <= n; i++)
        nums += i;
    return nums;
}
void C()
{
    const int n = 20;
    int ans = num(2 * n - 2) + (n * 2 - 1) / 2 + 1;
    cout << ans << endl;
}
```



# 试题D：跑步锻炼

## 题目

![image.png](https://cdn.nlark.com/yuque/0/2020/png/1693220/1602998566852-4ee1f420-6b20-4aeb-832d-dd6e70694c89.png)

## 参考解题方法

### 思路

> 设：a为2020年1月1日至2020年10月1日的总天数；b为总月数；d为总周数；e为即是1号又是周一的天数；L为总路程;
>
> L = a + b + d - e
>
> 对着日历数了以下**e=34** ( 眼都要瞎了 )
>
> 参考答案：8449

### 代码

```cpp
bool isLeapYear(const int year)
{
    if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
        return true;
    return false;
}

void D()
{
    const int beginYear = 2000;
    const int endYear = 2019;
    int E = 34;
    
    int leapYearNums = 0;
    for (size_t i = beginYear; i <= endYear; i++)
        if (isLeapYear(i)) leapYearNums++;

    
    int totalDays = (endYear - beginYear - leapYearNums) * 365 + leapYearNums * 366 + 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 1;
    int months = (endYear - beginYear) * 12 + 10;
    int weeks = totalDays / 7;
    int KMs = totalDays + months + weeks - E;
    
    cout << "totalDays = " << totalDays << endl;
    cout << "months = " << months << endl;
    cout << "weeks = " << weeks << endl;
    cout <<"KMs = "<< KMs << endl;
}
```

> 运行结果：
>
> totalDays = 7215
>
> months = 238
>
> weeks = 1030
>
> KMs = 8449

# 试题E：七段码

## 题目

![image.png](https://cdn.nlark.com/yuque/0/2020/png/1693220/1603000055324-3f4353e6-39f4-4e87-952f-7579ccc57322.png)

## 参考解题方法

### 思路

> ![image.png](https://cdn.nlark.com/yuque/0/2020/png/1693220/1603033058678-5c216fef-260c-4fb2-bdd6-39e7cd268eca.png)
>
> 我是用笔在草稿纸上面把每一中都画出来的,亮1个和亮6个种类数量是相同的，2个与5个种类数量是相同的
>
> 亮1个灯：7
>
> 亮2个灯：10
>
> 亮3个灯： 16
>
> 亮4个灯：16
>
> 亮5个灯：10 + 6
>
> 亮6个灯：7
>
> 亮7个灯：1
>
> 总数：73



# 试题F：成绩统计

## 题目

![image.png](https://cdn.nlark.com/yuque/0/2020/png/1693220/1603444095315-68ef8a3a-8949-4785-b5bc-e9ad426e94dc.png)

![image.png](https://cdn.nlark.com/yuque/0/2020/png/1693220/1603444105933-9f07e10b-f1ca-4cbb-80c2-52b70d885ab0.png)

![image.png](https://cdn.nlark.com/yuque/0/2020/png/1693220/1603444113842-0ade158a-caa5-4e02-8759-5f964c087693.png)

## 参考解题方法

### 思路

> 主要将四舍五入处理好就没啥大问题

### 代码

```cpp
void F()
{
    int N;
    cin >> N;

    //1.统计及格人数和优秀人数
    int jige_nums = 0;
    int youxiu_nums = 0;
    for (size_t i = 0; i < N; i++)  
    {
        int x;
        cin >> x;
        if (x >= 60)jige_nums++;
        if (x >= 85)youxiu_nums++;
    }

    //2.计算相应小数的1000倍
    int jigelv_I = (int)((float)jige_nums*1000.0 / (float)N);
    int youxiulv_I = (int)((float)youxiu_nums*1000.0 / (float)N);

    //3.处理四舍五入位
    if (jigelv_I % 10 > 4){
        jigelv_I /= 10;
        jigelv_I++;
    }else{
        jigelv_I /= 10;
    }
    if (youxiulv_I % 10 > 4){
        youxiulv_I /= 10;
        youxiulv_I++;
    }else{
        youxiulv_I /= 10;
    }

    cout << jigelv_I << "%" << endl;
    cout << youxiulv_I << "%" << endl;
}
```

# 试题G：回文日期

## 题目

![image.png](https://cdn.nlark.com/yuque/0/2020/png/1693220/1603444300611-87df3b36-4e34-480a-99f3-9c07ee3224a4.png)

![image.png](https://cdn.nlark.com/yuque/0/2020/png/1693220/1603444311274-006ea242-f02c-47b8-a8a7-cad8389a2a65.png)

![image.png](https://cdn.nlark.com/yuque/0/2020/png/1693220/1603444320339-98e14370-a62e-4bc3-886b-e5fb7e689cf5.png)



> 我不会.......嘻嘻嘻

# 试题 H: 子串分值和

# 题目

![image.png](https://cdn.nlark.com/yuque/0/2020/png/1693220/1603444610975-12646e7d-9e4a-4c73-8c1c-030b1b5f5a64.png)

![image.png](https://cdn.nlark.com/yuque/0/2020/png/1693220/1603444620400-514d107e-59b5-4083-b49f-5470e1ebab72.png)

![image.png](https://cdn.nlark.com/yuque/0/2020/png/1693220/1603444630661-4e75b807-19f2-4d77-8108-3aa961ee1011.png)

## 参考解题方法

### 思路

> set容器特点：自动删除放入的重复数据，保留不重复的数据
>
> 将子串分字符放入set容器中，获得set容器的size进行累加，进而获得所有子串中不重复字符的个数

### 代码

```cpp
void G()
{
    int count = 0;

    string S;
    cin >> S;
    
    int length = S.length();
    for (size_t i = 0; i < length-1; i++)
    {
        for (size_t j = i+1; j < length; j++)
        {
            set<char> s;
            for (size_t k = i; k <= j; k++)
                s.insert(S[k]);
            count += s.size();
        }
    }
    cout << count << endl;
}
```

> 该算法时间复杂度爆炸，能过50%就不错了，可从寻找子串处进行优化

# 试题I：平面切分

## 题目

![image.png](https://cdn.nlark.com/yuque/0/2020/png/1693220/1603524926038-6eca7e7b-77b0-440e-8106-4b457803b62c.png)

![image.png](https://cdn.nlark.com/yuque/0/2020/png/1693220/1603524938535-a19ed4ad-c4d3-4645-88da-c270a477a847.png)

> 我不会。。。嘻嘻嘻。。

# 试题J：字串排序

## 题目

![image.png](https://cdn.nlark.com/yuque/0/2020/png/1693220/1603525033510-0d3ab223-8ebc-4095-ba7a-62b8867dcd85.png)

![image.png](https://cdn.nlark.com/yuque/0/2020/png/1693220/1603525041697-80755231-7a32-4243-9c3b-72125609c4f5.png)

> 我不会。。。嘻嘻嘻。。