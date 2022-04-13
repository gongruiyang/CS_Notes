# 一、基本用法

* 登录至 ip 为 hostname 的 user 账户上 

```
ssh user@hostname
```

* ssh 的默认端口号为 22 ，若想使用特定端口号可使用 `-p`

```
ssh user@hostname -p 22
```

# 二、配置文件登录

创建文件 `~/.ssh/config`

然后在文件中以以下格式来配置服务器：

```
Host myserver1
    HostName IP地址或域名
    User 用户名

Host myserver2
    HostName IP地址或域名
    User 用户名
```

使用以下命令来登录配置好的服务器：

```
ssh myserver
```

# 三、使用密钥登录

创建密钥：使用`ssh-keygen`来创建一个密钥

```
ssh-keygen
```

执行结束后，`~/.ssh/`目录下会多两个文件：

* `id_rsa`：私钥
* `id_rsa.pub`：公钥

之后想免密登录至某个服务器，就将公钥传给哪个服务器即可

上传方式一：在本地服务器使用`ssh-copy-id`命令，在输入一次密码即可

```
ssh-copy-id myserver
```

上传方式二：将本地服务器刚刚产生的公钥`id_rsa.pub`中的内容复制到需登录的服务器`~/.ssh/authorized_keys`文件里即可

# 四、远程执行命令

例如以下命令：

```
ssh myserver ls -a
```

这个命令的执行流程：登录至服务器，执行`ls -a`命令后，将标准输出重定向至本地服务器的标准输出

# 五、scp 传文件

* 传输若干个单独文件，`:`至关重要

```
scp file1 file2 destination:
```

* 传输文件夹

```
scp -r fold/ destination:
```

例子 1：

```
scp a.sh myserver:
scp -r fold myserver:
```

例子 2：配置服务器 vim 和 tmux

```
scp .vimrc .tmux.conf myserver:
```


