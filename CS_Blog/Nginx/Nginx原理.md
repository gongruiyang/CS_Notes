# 一、代理服务器

## 1. 正向代理（http代理）

Nginx在做正向代理时，可以将其理解为一个跳板机，代理客户端访问外部资源，例如VPN

当我们在国内想要访问谷歌时，直接访问是访问不到的，此时我们可以在中间加入一个代理服务器，这个代理服务器可以是在香港或其他地方，我们将请求发送给这个代理服务器，这个代理服务器再将请求发送给谷歌，通过这个代理服务器间接的和谷歌进行数据的收发，这样就实现了与谷歌的交互

![image-20210720105705679](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210720105712.png)



## 2. 反向代理

Nginx在做反向代理时，可以提供性能稳定、配置灵活的转发功能。Nginx可以根据URL进行正则匹配后采用不同的转发策略。

例如：你想访问百度的一个百度贴吧部门服务器中的一个资源时，这是你在URL中输入这个资源的URL点击搜索时，百度就首先会将这个URL转发至贴吧部门的Nginx层，该部门的Nginx层根据配置文件中的路由转发正则表达式去匹配该URL，然后rewrite该URL，再将该URL转发至对应的webserver上进行处理

![](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210720105719.png)

## 3. 正反对比

`正向代理`是指Nginx作为`客户端代理服务器`，用来`代理客户端`，对于服务端来说，客户端的具体数量信息都被代理服务器屏蔽掉了，服务端并不知道实际发出请求是哪一个客户端

![image-20210720112423995](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210720112424.png)

`反向代理`是指Nginx作为`服务端代理服务器`，用来`代理服务端`，对于客户端来说，服务端的具体数量信息都被代理服务器屏蔽掉了，客户端并不知道实际处理请求的是哪一个服务端

![image-20210720112431956](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210720112432.png)

# 二、负载均衡内置策略

## 1. 轮询

给每一个服务器分配的请求数量都是相同的，均匀分配的坏处是通常后台服务器性能有差异，有时候希望性能较好的服务器可以多承担些请求，该策略无法满足需求。这个不适合对长连接和命中率有要求的场景。

![image-20210720113709232](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210720113709.png)

配置如下

```nginx
upstream backserver { 
    server 192.168.0.14; 
    server 192.168.0.15; 
} 
```



## 2. 加权轮询

这种策略是对轮询策略的优化，对每个后端服务器设置一个权重值，根据权重值决定请求的分配比例，这种做法解决了轮询策略的不足，但是该**权限值需要手动配置**，没有办法自动动态调节，同样不适合对长连接和命中率有要求的场景。

![image-20210720113721934](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210720113722.png)

配置如下：

```nginx
upstream backserver { 
    server 192.168.0.14 weight=8; 
    server 192.168.0.15 weight=10; 
} 
```



## 3. IP哈希

每个请求按访问ip的hash结果分配，这样每个访客固定访问一个后端服务器，可以解决session的问题。

![image-20210720142607737](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210720142607.png)

配置如下：

```ngin
upstream backserver { 
    ip_hash; 
    server 192.168.0.14:88; 
    server 192.168.0.15:80; 
} 
```



## 4. URL哈希

按访问url的hash结果来分配请求，使每个url定向到同一个后端服务器，后端服务器为缓存时比较有效。 

配置如下：

```nginx
upstream backserver { 
    server squid1:3128; 
    server squid2:3128; 
    hash $request_uri; 
    hash_method crc32; 
} 
```



## 5. fair

按后端`服务器的响应时间`来分配请求，响应时间短的优先分配。 

配置如下：

```nginx
upstream backserver { 
    server server1; 
    server server2; 
    fair; 
} 
```



# 三、动静分离

原始的资源请求都是将静态资源与动态资源都放在同一台服务器上面，当请求量较大时，会导致网站解析速度降低，为了提高网站解析速度，可以把动态页面和静态页面交由不同的服务器来进行解析，进而降低原来单个服务器的压力。

![image-20210721164151693](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210721164158.png)
