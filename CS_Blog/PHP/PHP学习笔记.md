# PHP脚本标记格式

```php
<?php
    PHP Code
?>
```

# PHP注释格式

* 行注释

```php
<?php
    // 行注释
?>
```

* 块注释

```php
<?php
    /*
    	块注释
    */
?>
```

# PHP变量

## 变量的使用

* 定义变量

```php
    $var1;  //定义变量（未初始化）
```

* 定义并初始化

```php
    $var2 = 1; //定义并初始化变量
```

* 修改变量

```php
    $var3 = 0;
    $var3 = 1;  //修改变量
```

* 删除变量

```php
    $var3 = 1;
    unset($var3);   // 删除变量：释放 var3 的内存空间
```

## 预定义变量

```php
    $a = 'C';
    $C = 'a';
    echo $$a;	// $($a -> C) = $C -> a
    echo $$C;	// $($C -> a) = $a -> C
```

## 变量传值

* 值传递

```php
$a = $b
```

* 引用传递：两个变量指向同一存储空间

```php
$a = &$b
```



## 常量

* 一经初始化后不再允许修改
* 不需要使用 `$` 符号
* 命名规范：使用全大写

```php
const a = 10;
define('b', 10); // 此定义下变量名可不准守变量名规则
```

* 特殊名字常量的访问

```php
define('-_-','smile');
echo constant('-_-');
```



## 系统常量

| 系统常量名   | 具体含义           |
| ------------ | ------------------ |
| PHP_VERSION  | PHP版本号          |
| PHP_INT_SIZE | 整形大小           |
| PHP_INT_MAX  | 整形表示的最大数值 |



```php
    echo PHP_VERSION;     //5.4.8
    echo PHP_INT_SIZE;    //4字节
    echo PHP_INT_MAX;     //2147483647
```



## 文件属性常量

| 文件属性常量   | 具体含义                                         |
| -------------- | ------------------------------------------------ |
| \__DIR__       | 当前被执行的脚本所在的绝对路径（不包含本文件名） |
| \__FILE__      | 当前被执行的脚本所在的绝对路径（包含本文件名）   |
| \__LINE__      | 当前所属的行数                                   |
| \__NAMESPACE__ | 当前所属的命名空间                               |
| \__CLASS__     | 当前所属的类                                     |
| \__METHOD__    | 当前所属的方法                                   |



# 数据类型

## 八种基本数据类型

* 数据类型指数据本身的类型，而不是变量的类型
* PHP是一种若类型语言，变量本身没有数据类型

> 基本数据类型


| 基本数据类型 | 表示 | 所占内存 |
| :------: | :------------: | :---: |
| 整形     | int / integer  | 4字节 |
| 浮点型   | float / double | 8字节 |
| 字符串型 |     string     | 一个字符一字节 |
| 布尔类型 | bool / boolean | 1字节 |



> 复合数据类型

| 复核数据类型 |  表示  |      |
| :----------: | :----: | ---- |
|     对象     | object |      |
|   数组类型   | array  |      |



> 特殊数据类型

| 特殊数据类型 |   表示   |                   含义                    |
| :----------: | :------: | :---------------------------------------: |
|   资源类型   | resource | 存放资源数据（PHP外部数据、文件、数据库） |
|    空类型    |   NULL   |                 无法运算                  |



## 数据转换

* 自动转换：系统根据需求判断
* 强制转换：使用`(数据类型)`即可

> 1. 以`字母开头`的字符串转成数字，永远`会是0`
> 2. 以`数字开头`的字符串转成数字，直到碰到`字符为止`，且不会出现两个小数点

## 其他进制输出

* PHP中会将所有进制输出为`10进制`

## 整形越界现象

* 整形数据越界后会使用浮点型表示

```php
    echo gettype(PHP_INT_MAX + 1);  // double
```

整数：32位都是有效数字

浮点数：高 7 位表示 10 的整数次幂，低 25 位表示具体数值

# 运算符

## 比较运算符

| 运算符 | 含义                       |
| ------ | -------------------------- |
| >      | 大于                       |
| <      | 小于                       |
| >=     | 大于等于                   |
| <=     | 小于等于                   |
| ==     | 数值相同，可能数据类型不同 |
| !=     | 数值不同                   |
| ===    | 数值相同 且 数据类型相同   |
| !==    | 数值不同 或 数据类型不同   |

## 逻辑运算符

| 逻辑运算符 | 含义   |
| ---------- | ------ |
| &&         | 逻辑与 |
| \|\|       | 逻辑或 |
| ！         | 逻辑非 |

## 连接运算符

`.`相当于C++中对于字符串进行拼接的`+`

`.=`相当于C++中对于字符串进行拼接的`+=`

```php
$s1 = 'Good ';
$s2 = 'Boy!';
echo $s1.$s2;	// Good Boy!

$s1 = 'Good ';
$s2 = 'Boy!';
$s1 .= $s2;
echo $s1;	//Good Boy!
```

## 错误抑制符

* `@`：PHP中有一些错误可以预知，但是无法避免，且不想暴露给用户，此时可使用`错误抑制符`
* 开发时不会用到，上线时可能会用到

```php
echo (10 / 0);	//	PHP Warning:  Division by zero
echo @(10 / 0);	// 无输出
```

## 三目运算符

* 格式：`表达式1 ？ 表达式2 : 表达式3 `;

```ph
echo 1 > 2 ? 1 : 2;     // 2
```

## 自操作运算符

* `++`：自增
* `--`：自减

## 位运算符

| 位运算符 | 含义     |
| -------- | -------- |
| &        | 按位与   |
| \|       | 按位或   |
| ^        | 按位异或 |
| ~        | 按位非   |
| <<       | 左移     |
| >>       | 右移     |



## 运算符优先级

* 相同优先级的运算符结合方式
  * 左结合：自左向右运算
  * 右结合：自右向左运算
  * 无结合：无法运算
* 下标`自上而下`优先级`降低`

![image-20210628114931096](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210628115230.png)

# 流程控制

## if 语句

```php
if( /* bool */ )
{
    // TODO
}
else if( /* bool */)
{
    // TODO
}
else
{
    // TODO
}
```

## switch 语句

```php
switch (/* var */)
{
    case 值1:
        // TODO
        break;
    case 值2:
        // TODO
        break;
    case 值3:
        // TODO
        break;
    default:
        // TODO
}
```

## for 循环

```php
for(/* 表达式 */; /* 表达式 */; /* 表达式 */)
{
    // TODO
}
```

## while 循环 与 do while循环

* while 循环

```php
while( /* bool */)
{
    // TODO
}
```

* do while循环

```php
do
{
    // TODO
}
wile(/* bool */);
```



# 文件包含

* 包含含义

在当前文件中可以使用别的文件中定义的函数或变量等资源

* 包含文件的 4 种方式

`include`和`Require`

`include_once`和`Require_once`

```php
    include_once 'header.php';	// 相对路
    echo PI;
```

* 文件包含原理：代码嵌入

# PHP代码执行过程

1. 读入代码文件（PHP文件）
2. 编译：将`PHP代码`文件转化成`字节码`（生成`opcode`）
3. `zendengine`解析`opcode`并按照`字节码`去进行`逻辑运算`,此时依然是字节码文件
4. 转换成对应的 HTML 文件

## 换行符

`PHP_EOL`：表示换行

```php
define ('PHP_EOL', "\n");
```

# 函数

```php
function($var1, $var2)
{
    // TODO
    return $var1.$var2;
}
```



# 面向对象编程

## 类

* 变量 `$this` 代表自身的对象
* 使用 `new`运算符来实例化该类的对象
* 使用`->`来访问对象内部函数与变量
* 构造函数`function __construct`
* 析构函数`function __destruct`
* 成员变量或成员函数前使用`public`或`private`或`protected`修饰确定访问权限，若不声明则为`public`

```php
class Site {
    /* 成员变量 */
    protected $var1 = 0;
    private $var2 = 0.0;
    public $var3 = "";

    /**
     * Site constructor.
     * @param int $var1
     * @param float $var2
     * @param string $var3
     */
    public function __construct($var1, $var2, $var3)
    {
        $this->var1 = $var1;
        $this->var2 = $var2;
        $this->var3 = $var3;
    }
    
    function __destruct() {
        print "销毁 " . $this->name . "\n";
    }

    /**
     * @return int
     */
    public function getVar1()
    {
        return $this->var1;
    }

    /**
     * @param int $var1
     */
    public function setVar1($var1)
    {
        $this->var1 = $var1;
    }

    /**
     * @return float
     */
    public function getVar2()
    {
        return $this->var2;
    }

    /**
     * @param float $var2
     */
    public function setVar2($var2)
    {
        $this->var2 = $var2;
    }

    /**
     * @return string
     */
    public function getVar3()
    {
        return $this->var3;
    }

    /**
     * @param string $var3
     */
    public function setVar3($var3)
    {
        $this->var3 = $var3;
    }
}
```

## 接口

* 使用接口，可以指定某个类必须实现哪些方法，但不需要定义这些方法的具体内容。
* 接口是通过 `interface`关键字来定义的，就像定义一个标准的类一样，但其中定义所有的方法都是空的。
* 接口中定义的所有方法都必须是`公有`，这是接口的特性
* 使用 `implements` 操作符来实现一个接，类中必须实现接口中定义的所有方法
* 类可以`实现多个接口`，用`逗号`来分隔多个接口的名称

```php
// 声明一个'iTemplate'接口
interface iTemplate
{
    public function setVariable($name, $var);
    public function getHtml($template);
}


// 实现接口
class Template implements iTemplate
{
    private $vars = array();

    public function setVariable($name, $var)
    {
        $this->vars[$name] = $var;
    }

    public function getHtml($template)
    {
        foreach($this->vars as $name => $value) {
            $template = str_replace('{' . $name . '}', $value, $template);
        }

        return $template;
    }
}
```

## 抽象类







# 常用函数

## 输入输出函数

* print_r ：类似var_dump函数，也可以用来打印数组

```php
$arr = array(1,2,3,4,5);
print_r($arr);
/*
Array
(
    [0] => 1
    [1] => 2
    [2] => 3
    [3] => 4
    [4] => 5
)
 */
```

---

* print ：类似echo输出内容，是一种结构不是函数

```php
print($x);
print $x;
```

## 类型控制函数

---

* is_XXX 函数：判断变量的数据类型

```php
is_XXX(变量名) 	-> bool
```

---

* var_dump 函数

```php
var_dump(变量名) 	-> bool
```

`Bool类型`数据不能用`echo`来查看，需要使用`var_dump`函数

---

* gettype 函数：获取变量类型

```php
gettype(变量名)	-> string
```

---

* settype 函数：设定数据类型，与强制转换不同，会改变原数据本身

```php
settype(变量名，类型)		-> void
```

---

* 进制转换函数

| 进制转换函数 |   含义   |
| :----------: | :------: |
|  **hexdec**  | 16 -> 10 |
|    dechex    | 10 -> 16 |
|    decbin    | 10 -> 2  |
|    bindec    | 2 -> 10  |

---

* 空判断

![image-20210628112155861](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210628112202.png)

---

## 时间相关函数



## 数学相关函数



## 日常遇见

* 判断变量是否为正帧数
