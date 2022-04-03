# Linux

### uname -- 查看系统信息

![image-20200804202918004](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20200804202918004.png)

---



### vmstat -- 查看虚拟内存信息

![image-20200804213751818](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20200804213751818.png)

![image-20200804214111253](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20200804214111253.png)

---



### stat -- 显示文件状态信息

![image-20200804214544694](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20200804214544694.png)

---



### grep -- 打印给定匹配模式的行内容

*  grep选项中，-E选项可以用来扩展选项为正则表达式；
  * $表示匹配文件末尾，字符需要在$之前表示以字符结尾 a$表示以a结尾
  * ^表示匹配文件起始，字符需要在^之后表示以字符起始 ^a表示以a起始

![image-20200804220259074](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20200804220259074.png)

---



### top -- 查看CPU资源使用状态

![image-20200804220653359](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20200804220653359.png)

---



### netstat -- 查看网络连接状态

![image-20200804220841571](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20200804220841571.png)

---



### free -- 查看内存资源使用状态

![image-20200804221137379](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20200804221137379.png)

---



###  df -- 查看磁盘分区资源状态

![image-20200804221328454](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20200804221328454.png)

---



### 批量删除某一后缀名的所有文件

* find . -name "*.c" -maxdepth 1 | xargs rm 
  * find . -name "*.c" -maxdepth 1 找到当前目录下.*结尾的文件，目录深度为1
  * xargs是一个强有力的命令，它能够捕获一个命令的输出，然后传递给另外一个命令，用于很多不支持|管道来传递参数的命令
* find . -name "*.c" | xargs rm 会将子目录下的文件也会删除

![image-20200804222318024](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20200804222318024.png)

---



### ps -- 查看当前进程信息



---



### env -- 查看环境变量



### firewall-cmd   -- 防火墙命令

* 开放端口命令

```shell
[root@localhost logs]# firewall-cmd --add-port=8080/tcp --permanent 
success
```

* 查看开放的端口信息

```shell
[root@localhost logs]# firewall-cmd --list-all
public (active)
  target: default
  icmp-block-inversion: no
  interfaces: ens32
  sources: 
  services: dhcpv6-client ssh
  ports: 8080/tcp 8081/tcp 80/tcp 9001/tcp
  protocols: 
  masquerade: no
  forward-ports: 
  source-ports: 
  icmp-blocks: 
  rich rules: 
	
```

* 重新加载防火墙配置

```shell
[root@localhost logs]# firewall-cmd --reload 
success
```



