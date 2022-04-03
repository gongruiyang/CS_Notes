# 一、conf配置分块

## 1. 全局块

从配置文件开始直至events块之间的内容，主要会设置一些影响 Nginx 服务器整体运行的配置指令

```nginx
worker_processes  1;	# 该值越大，可以支持的并发处理量越大，实际量受到软硬件设备的制约
```

## 2. events 块 

涉及到 Nginx 服务器与用户的网络连接，常用的设置包括但不限于：

* 是否开启对多 work process 下的网络连接进行序列化

```nginx
accept_mutex on | off; # on 开启，默认为开启状态
```



* 是否允许同时接收多个网络连接

```nginx
multi_accept on | off; # on 开启，默认为关闭状态
```



* 选取哪种事件驱动模型来处理连接请求

```nginx
use METHOD;
```

METHOD可以是：select、poll、kqueue、epoll、rtsig、/dev/poll、eventport



* 设置每个 work process 可以同时支持的最大连接数

```nginx
worker_connections  1024;	# 支持最大的用户连接数，默认值为512
```

## 3. http 块

这是 Nginx 配置最频繁的部分

### 3.1 http 全局块

http 全局块配置的指令包括但不限于：

* 文件的引入：`include`
* MIME-TYPE定义：
* 日志定义：`access_log`
* 日志格式：`log_format`
* http连接超时时间：`keepalive_timeout`
* 单连接请求上线数值

### 3.2 server 块

这块与虚拟主机有直接联系，虚拟主机从用户角度看，和一台独立的主机是一样

每个 http 块包含多个 server 块

每个 server 块也包含两部分：

* server 全局块 ：本虚拟主机的监听配置、本虚拟主机的IP配置、本虚拟主机的名称配置
* 若干个 location 块：作用为基于请求字符串（例如：server_name/uri_string），对于`uri_string`进行匹配，对不同的请求资源分别处理。功能有：地址定向、数据缓存、应答控制、第三方模块配置

```nginx
server {
    # server 全局块
    listen       80;	# 监听端口
    server_name  localhost;	# 主机名称
	
    # location 块
    location / { # 根目录下的请求打到 http://NAME
        root   html;
        index  index.html index.htm;
        # 反向代理配置
        proxy_pass http://NAME;	
    }
}
```

## 3. conf文件内容解释

```nginx
# 全局配置
worker_processes  1;	# 该值越大，可以支持的并发处理量越大，实际量受到软硬件设备的制约

events {
    worker_connections  1024;	# 支持最大的用户连接数
}

http {
    # HTTP全局配置
    include       mime.types;
    default_type  application/octet-stream;
    sendfile        on;
    keepalive_timeout  65;
	
    # 负载均衡策略配置
    upstream NAME{	# NAME随便定，为了给后面 lcation 中的 proxy_pass 使用
        # 服务器资源: server 服务地址:端口号
        server 127.0.0.1:80 weight=1;	# weight控制轮询权重
        server 127.0.0.1:81 weight=1;
    }
    
    server {
        listen       80;	# 监听端口
        server_name  localhost;
		
        # 根目录下的请求打到 http://NAME
        location / {
            root   html;
            index  index.html index.htm;
            # 反向代理配置
            proxy_pass http://NAME;	
        }

        error_page   500 502 503 504  /50x.html;
        location = /50x.html {
            root   html;
        }
    }
}
```





# 二、反向代理配置demo

## demo1：反向代理一台服务器

![image-20210728194624317](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210728194624.png)

> 实现Windows浏览器访问Linux服务器，反向代理至tomcat
>
> 1. 配置Windows中hosts文件，域名解析至Linux的IP
> 2. 配置Linux的Nginx中conf文件，反向代理至tomcat中

步骤一：配置hosts文件

`C:\Windows\System32\drivers\etc\hosts`

```shell
192.168.209.128 www.123.com		# 用于测试nginx配置
```

步骤二：配置conf文件

![image-20210728115740657](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210728115747.png)



## demo2：反向代理多台服务器

![反向代理多台服务器](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210729115605.png)

> nginx.conf

```nginx
server {
    listen 9001;
    server_name 192.168.58.128;

    location ~ /fir/ {
        proxy_pass http://127.0.0.1:8080;
    }

    location ~ /sec/ {
        proxy_pass http://127.0.0.1:8081;
    }
}
```

> Tomcat_1 server.xml

```xml
<Connector port="8080" protocol="HTTP/1.1"
           connectionTimeout="20000"
           redirectPort="8443" />
```

> Tomcat_2 server.xml

```xml
<Connector port="8081" protocol="HTTP/1.1"
           connectionTimeout="20000"
           redirectPort="8443" />
```

