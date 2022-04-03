## 1、版本控制

- **版本控制的定义：**是指对**软件开发过程**中的**代码文件、配置文件、说明文档等文件**的变更进行的**管理**
- **版本控制的功能**：

1. **追踪文件的变更**：它将什么人在什么时间更改了什么文件的哪些内容如实的记录了下来，每次一文件改变，文件的版本号递增
2. **支持并行开发**：软件开发往往是多人协同作业，版本控制可以解决版本同步问题和不同开发者之间的开发通信问题

## 2、Linux下git基本操作

- 第一步：git clone URL

> 作用：将远程仓库克隆至本地中

![image.png](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210303122431.png)

```
[gongruiyang@localhost ~]$ git clone https://github.com/gongruiyang/Learn_Linux.git
正克隆到 'Learn_Linux'...
remote: Enumerating objects: 3, done.
remote: Counting objects: 100% (3/3), done.
remote: Compressing objects: 100% (3/3), done.
remote: Total 3 (delta 0), reused 3 (delta 0), pack-reused 0
Unpacking objects: 100% (3/3), done.
```

- 第二步：git add File

> 本地提交修改信息

```
[gongruiyang@localhost Learn_Linux]$ git add ./*
```

- 第三步：git commit -m "备注信息"

> 提交本地版本管理

```
[gongruiyang@localhost Learn_Linux]$ git commit -m "增加了一个test.c文件哦！"
[master e99e576] 增加了一个test.c文件哦！
 Committer: gongruiyang <gongruiyang@localhost.localdomain>
 2 files changed, 8 insertions(+), 1 deletion(-)
 create mode 100644 test.c
```

- 第四步：git push origin master

> 将本次版本提交到远程服务器仓库

```
[gongruiyang@localhost Learn_Linux]$ git push origin master 
Username for 'https://github.com': 1252574653@qq.com
Password for 'https://1252574653@qq.com@github.com': 
Counting objects: 6, done.
Compressing objects: 100% (4/4), done.
Writing objects: 100% (4/4), 448 bytes | 0 bytes/s, done.
Total 4 (delta 1), reused 0 (delta 0)
remote: Resolving deltas: 100% (1/1), completed with 1 local object.
To https://github.com/gongruiyang/Learn_Linux.git
   fa2a762..e99e576  master -> master
```

![image.png](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210303122431.png)

> 远程服务器中我的仓库里便增加了一个test.c文件

## 3、版本控制系统GitHub与SVN的对比

1. GitHub是分布式管理系统，SVN是集中式管理系统，这就导致GitHub更安全
2. GitHub把内容按照元数据方式存储，SVN是按照文件存储