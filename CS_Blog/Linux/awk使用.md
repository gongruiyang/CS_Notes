# 0. 常用技巧

* `,`：表示默认空格
* `"\t"`：表示 Tab 
* `awk 'cmd' [FILES]`：FILES 可以为多个文件进行拼接，如果 FILES 不指定，则默认为 STDIN 等待用户输入

# 1. 按行打印

* `$N`：表示第 N 列
* `$0`：表示所有列

```shell
gongruiyang@gongruiyang:~/test/awk$ awk '{print $1}' test.txt
Fruit
apple
banana
orange
pair
gongruiyang@gongruiyang:~/test/awk$ awk '{print $2}' test.txt
Price
12.8
10.5
9.5
5.5
gongruiyang@gongruiyang:~/test/awk$ awk '{print $3}' test.txt
Origin
China
USA
Japan
China
gongruiyang@gongruiyang:~/test/awk$ awk '{print $0}' test.txt
Fruit       Price       Origin
apple       12.8        China
banana      10.5        USA
orange      9.5         Japan
pair        5.5         China
```

# 2. 显示行号 或 统计行数

* `NR`：number of record，即统计当前是第几行
* `NF`：number of filed，即统计有多少列

```shell
gongruiyang@gongruiyang:~/test/awk$ awk '{print NR,$0}' test.txt
1 Fruit       Price       Origin
2 apple       12.8        China
3 banana      10.5        USA
4 orange      9.5         Japan
5 pair        5.5         China
```

# 3. 按条件查找

* `NR==N`：表示只输出第 N 行记录
* `NF==N`：表示只输出有 N 列的行
* `$N >=< nums`：表示按条件过滤第 N 列

```shell
gongruiyang@gongruiyang:~/test/awk$ awk '$3=="China"{print NR,$0}' test.txt
2 apple       12.8        China
5 pair        5.5         China
gongruiyang@gongruiyang:~/test/awk$ awk '$2>=10{print NR,$0}' test.txt
1 Fruit       Price       Origin
2 apple       12.8        China
3 banana      10.5        USA
gongruiyang@gongruiyang:~/test/awk$ awk 'NR==3{print NR,$0}' test.txt
3 banana      10.5        USA
```

# 4. 定义全局变量

* `BEGIN{}`：用来定义全局变量
  * `FS`：表示输入分隔符
  * `OFS`：表示输出分隔符

```shell
'gongruiyang@gongruiyang:~/test/awk$ awk 'BEGIN{FS=","} {print $1, $2}'
hello! fucking world!
hello! fucking world!
hello,fucking,world!
hello fucking'
```

# 5. 显示行所属文件名

```shell
gongruiyang@gongruiyang:~/test/awk$ awk '{print NR, FILENAME, $0}' test*
1 test1.txt Fruit       Price       Origin
2 test1.txt apple       12.8        China
3 test1.txt banana      10.5        USA
4 test1.txt orange      9.5         Japan
5 test1.txt pair        5.5         China
6 test2.txt 石榴    100     China
7 test2.txt 西瓜    11      China
```

# 6. 隐藏某一列

```shell
gongruiyang@gongruiyang:~/test/awk$ awk '{$3="****"; print NR, FILENAME, $0;}' test*
1 test1.txt Fruit Price ****
2 test1.txt apple 12.8 ****
3 test1.txt banana 10.5 ****
4 test1.txt orange 9.5 ****
5 test1.txt pair 5.5 ****
6 test2.txt 石榴 100 ****
7 test2.txt 西瓜 11 ****
```



# 7. 打印最后一列

* `$NF`：表示最后一列

```shell
gongruiyang@gongruiyang:~/test/awk$ awk '{print $NF}' test*
Origin
China
USA
Japan
China
China
China
```

* 打印倒数第二列：$(NF - 1)

```shell
gongruiyang@gongruiyang:~/test/awk$ awk '{print $(NF-1)}' test*
Price
12.8
10.5
9.5
5.5
100
11
```

