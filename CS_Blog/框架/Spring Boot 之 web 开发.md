# 一、静态资源访问

## 1. 静态资源目录

只要静态资源放在`/resources`下，以`/static` 或者`/public`或者`/resources`或者`/META_INF/resources`为父路径都可以访问到

访问路径：当前项目根路径/ + 静态资源名，例如：/xxx.jpg

* 可以使用`spring.mvc.static-path-pattern`在`applications.yaml`中来**控制**静态资源`访问路径`的模式：

```yaml
spring:
  mvc:
    static-path-pattern: /res/**
```

访问静态资源的路劲模式变为：/res/xxx.jpg

* 可以使用`spring.web.resources.static-locations`在`applications.yaml`中来**改变**静态`资源存储的路径`：

```yaml
spring:
  web:
    resources:
      static-locations:  [classpath:/res, classpath:/ans]
```

静态资源就可以放在`/res`和`/ans`路径中了

请求原理：一个请求进来，先会去找 `Controller` 中寻找处理该请求的 Handler 函数，如果找不到，就会将请求交给`静态资源处理器`，去`/resource`目录下寻找静态资源，如果静态资源找不到就会返回 404

## 2. WebJars

在 WebJars 官网寻找 jar 包的依赖放到 pom 文件中

```xml
        <dependency>
            <groupId>org.webjars</groupId>
            <artifactId>jquery</artifactId>
            <version>3.5.1</version>
        </dependency>
```

访问静态资源规则：/webjar/**

例如：`http://localhost:8080/webjars/jquery/3.5.1/jquery.js`



## 2.  欢迎页支持

有两种方法来支持 index 欢迎页

* 在合法的静态资源路径下放一个 index.html
* 使用一个 controller 来处理 index.html 请求

访问方式：`http://localhost:8080/index.html`









# 二、Rest风格请求处理

## 1. Rest 风格解释

Rest风格请求

1. `@RequestMapping(value = "/XXX",method = RequestMethod.METHOD)`
2. Rest 风格：使用 HTTP 请求方法来表示对资源的操作

Rest风格解析：

| HTTP 方法 | handler 含义 |
| --------- | ------------ |
| GET       | 获取资源     |
| POST      | 保存资源     |
| PUT       | 修改资源     |
| DELETE    | 删除资源     |

Rest 风格代码

```java
@RequestMapping(value = "/user",method = RequestMethod.GET)
public String getUser(){
    return "GET-张三";
}
@RequestMapping(value = "/user",method = RequestMethod.POST)
public String saveUser(){
    return "POST-张三";
}
@RequestMapping(value = "/user",method = RequestMethod.PUT)
public String putUser(){
    return "PUT-张三";
}
@RequestMapping(value = "/user",method = RequestMethod.DELETE)
public String deleteUser(){
    return "DELETE-张三";
}
```

## 2. Rest风格配置

1. 在`@RequestMapping`中加上 method 选项

2. 在 `applications.yaml`中开启配置

   ```yaml
   spring:
     mvc:
       hiddenmethod:
         filter:
           enabled: true
   ```

## 3. 表单使用 Rest 风格提交请求

表单只能提交 post 和 get 请求，测试后发现 put 和 delete 请求无法实现

```html
<form action="/user" method="get">
    <input value="REST-GET" type="submit"/>
</form>
<form action="/user" method="post">
    <input value="REST-POST" type="submit"/>
</form>
<form action="/user" method="put">
    <input value="REST-PUT" type="submit"/>
</form>
<form action="/user" method="delete">
    <input value="REST-DELETE" type="submit"/>
</form>
```

如果想提交 delete 和 put 请求需要对表单进行如下配置：

```html
<form action="/user" method="post">
    <input name="_method" type="hidden" value="PUT">
    <input value="REST-PUT" type="submit"/>
</form>
<form action="/user" method="post">
    <input name="_method" type="hidden" value="DELETE">
    <input value="REST-DELETE" type="submit"/>
</form>
```

这样，表单便可以发出 delete 和 put 请求

注意：如果是 PostMan 进行请求，直接设置请求方法即可

## 4. Rest 风格请求原理探究

![image-20220111172309727](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20220111172309727.png)





# 三 、请求参数处理

## 1. 请求映射原理探究（TODO）



## 2. URL参数提取及相关注解

### 2.1 @RequestParam

`@RequestParam`：将URL中的参数值注入的 handler 函数的参数上

```java
    @RequestMapping(path = "/getUser")
    public String test1(@RequestParam("userName") String name){
        return "userName";
    }
```



### 2.2 @GetMapping / @PathVariable

`@GetMapping`：URL中可以携带变量，使用`/{VARIABLE}`的方式

`@PathVariable`：将URL中的`路径变量`注入到 handler 参数中

* 单个注入

```java
    @GetMapping("/car/{id}/owner/{name}")
    public Map<String, String> test1(@PathVariable("id") String id, @PathVariable("name") String name){
        Map<String, String> hash = new HashMap<>();
        hash.put(id, name);
        return hash;
    }
```

* Map 注入

```java
    @GetMapping("/car/{id}/owner/{name}")
    public Map<String, String> test1(@PathVariable Map<String, String> hash){
        return hash;
    }
```



### 2.3  其他注解



`@RequestHeader`：用于获取 GET 请求头内容

`@RequestBody`：用于获取请求体

`@CookieValue`：用于获取 Coockie 值

### 3.4 页面跳转数据转发注解

`@ResponseBody`：表示该 handler 可以接收其他 handler 发出的请求，并且用改注解修饰的 handler 返回值只会被解析成 json 数据串，无法进行请求转发

使用`forward:/XXXX`来进行请求的跳转，并使用`@RequestAttribute`来捕捉调用请求的参数，注意：**类的注解不能使用@RestController，要用@Controller**

```java
    @GetMapping("/goto")
    public String goToNewPage(HttpServletRequest request)
    {
        request.setAttribute("msg", "success");
        request.setAttribute("code", 200);
        return "forward:/success";
    }

    @ResponseBody
    @GetMapping("/success")
    public Map<String, Object> success(HttpServletRequest request,
                          @RequestAttribute("msg") String msg,
                          @RequestAttribute("code") Integer code)
    {
        Map<String, Object> hashMap = new HashMap<>();
        hashMap.put(msg, code);
        hashMap.put("request", request);
        return hashMap;
    }
```



## 4. 参数处理原理探究（TODO）





# 四、响应数据与内容协商

## 1. 响应数据类型

![image-20220112160543319](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202201121605465.png)



## 2. 设置响应数据类型

### 2.1 json

`@ResponseBody`：此 handler 的返回值转为 json 数据串

```java
    @Resource
    Person person;

    @ResponseBody
    @RequestMapping(path = "/get")
    public Person getPerson(){
        return person;
    }
```



### 2.2 xml

导入 xml 依赖：

```xml
        <dependency>
            <groupId>com.fasterxml.jackson.dataformat</groupId>
            <artifactId>jackson-dataformat-xml</artifactId>
        </dependency>
```

开启内容协商模式：

```yaml
# 开启请求参数内容协商模式
spring:
  mvc:
    contentnegotiation:
      favor-parameter: true
```

controller 中的 handler 函数

```java
@Controller
public class staticTestController {
    @Resource
    Person person;
    @ResponseBody
    @RequestMapping(path = "/get")
    public Person getPerson(){
        return person;
    }
}
```

请求URL：`http://localhost:8080/get?format=xml`



# 五、视图解析与模板引擎



## 1. 视图解析



## 2. 模板引擎 - Thymeleaf

### 2.1 Thymeleaf 简介

> Thymeleaf is a modern server-side Java template engine for both web and standalone environments.
>
> Thymeleaf's main goal is to bring elegant natural templates to your development workflow — HTML that can be correctly displayed in browsers and also work as static prototypes, allowing for stronger collaboration in development teams.
>
> With modules for Spring Framework, a host of integrations with your favourite tools, and the ability to plug in your own functionality, Thymeleaf is ideal for modern-day HTML5 JVM web development — although there is much more it can do.

* Thymeleaf是一个用于web和独立环境的现代**服务器端Java模板引擎**。
* Thymeleaf的主要目标是将优雅的自然模板引入到您的开发工作流程中——HTML可以在浏览器中正确显示，也可以作为静态原型工作，从而在开发团队中实现更强的协作。
* Thymeleaf具有Spring框架模块、与您喜爱的工具的大量集成以及插入您自己的功能的能力，非常适合现代HTML5 JVM web开发——尽管它可以做的事情还有很多。

### 2.2 语法简介

- Simple expressions:
  - Variable Expressions: `${...}`
  - Selection Variable Expressions: `*{...}`
  - Message Expressions: `#{...}`
  - Link URL Expressions: `@{...}`
  - Fragment Expressions: `~{...}`

- Literals
  - Text literals: `'one text'`, `'Another one!'`,…
  - Number literals: `0`, `34`, `3.0`, `12.3`,…
  - Boolean literals: `true`, `false`
  - Null literal: `null`
  - Literal tokens: `one`, `sometext`, `main`,…
- Text operations:
  - String concatenation: `+`
  - Literal substitutions: `|The name is ${name}|`
- Arithmetic operations:
  - Binary operators: `+`, `-`, `*`, `/`, `%`
  - Minus sign (unary operator): `-`
- Boolean operations:
  - Binary operators: `and`, `or`
  - Boolean negation (unary operator): `!`, `not`
- Comparisons and equality:
  - Comparators: `>`, `<`, `>=`, `<=` (`gt`, `lt`, `ge`, `le`)
  - Equality operators: `==`, `!=` (`eq`, `ne`)
- Conditional operators:
  - If-then: `(if) ? (then)`
  - If-then-else: `(if) ? (then) : (else)`
  - Default: `(value) ?: (defaultvalue)`
- Special tokens:
  - No-Operation: `_`

| 表达式名称 | 表达式 | 用途                               |
| ---------- | ------ | ---------------------------------- |
| 变量取值   | ${...} | 获取请求域、session域、对象等值    |
| 变量选择   | *{...} | 获取上下文对象值                   |
| 消息       | #{...} | 获取国际化等值                     |
| 链接       | @{...} | 生成链接                           |
| 片段表达式 | ~{...} | jsp:include 作用，引入公共页面片段 |

### 2.3 测试demo

一、引入依赖

```xml
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-thymeleaf</artifactId>
        </dependency>
```

二、写 html 文件并使用 controller 来替换变量

success.html

```html
<!DOCTYPE html>
<!--thymeleaf 的名称空间-->
<html lang="en" xmlns:th="http://www.thymeleaf.org">
<head>
    <meta charset="UTF-8">
    <title >Success</title>
</head>
<body>
<h1 th:text="${msg}">hello!</h1>
<h2>
    <a href="www.google.com" th:href="${link}">去谷歌</a>
</h2>
</body>
</html>
```

controller.java

```java
@Controller
public class viewTestController {


    @GetMapping(path = "/get")
    public String test(Model model){
        model.addAttribute("msg", "hello!");
        model.addAttribute("link", "http://www.baidu.com");
        return "success";
    }
}
```

替换效果：

![image-20220114165155372](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202201141652487.png)

### 2.4 后端系统

1. 从 Model 中取出提示信息展示：

```html
<label style="color: red" th:text="${msg}">错误消息</label>
```

2. 用于指定表单 action

```html
<form class="form-signin" method="post" th:action="@{/login}">
```

3. 用于取出变量中的文字放在 html 的文字中

```html
[[${session.user.name}]]
```

4. 模板使用

> 声明模板：**footer.html**
>
> `th:fragment`  / `id`

```html
<body>
    <div th:fragment="copy">
        &copy; 2011 The Good Thymes Virtual Grocery
    </div>
</body>

<div id="copy">
  &copy; 2011 The Good Thymes Virtual Grocery
</div>
```

> 引用模板：`th:insert` / `th:replace`
>
> `"~{footer :: copy}"` 和 `"footer :: copy"`作用一样

```html
<body>
  ...
  <div th:insert="footer :: copy"></div>
  <div th:replace="footer :: copy"></div>
  <div th:include="footer :: copy"></div>
</body>
```

替换结果：

```html
<body>
  ...
  <div>
    <footer>
      &copy; 2011 The Good Thymes Virtual Grocery
    </footer>
  </div>
  <footer>
    &copy; 2011 The Good Thymes Virtual Grocery
  </footer>
  <div>
    &copy; 2011 The Good Thymes Virtual Grocery
  </div>
</body>
```

![image-20220210172214647](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202202101722924.png)

5. 表格填充

```html
        <tbody> <!-- 表数据 -->
        <tr class="gradeX" th:each="user, stats:${users}">
            <td th:text="${stats.count}"></td>  <!-- 计数 -->
                <!--对 users 遍历取出每个 user-->
            <td th:text="${user.getUsername()}"></td>
            <td th:text="${user.getPassword()}"></td>
        </tr>
```

![image-20220210183815846](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202202101838925.png)



# 六、拦截器

拦截器作用：用于拦截请求，判断是否符合要求，若符合要求则放行请求，否则会拦截请求

1.实现`HandlerInterceptor`接口，其中有三种方法

* `preHandle`：目标方法执行之前进行拦截器检查
* `postHandle`：目标方法执行之后进行拦截器检查
* `afterCompletion`：页面渲染以后检查

```java
/**
 *  登录检查
 */
public class LoginInterceptor implements HandlerInterceptor {

    /**
     * 目标方法执行之前进行拦截器检查
     */
    @Override
    public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler) throws Exception {
        // 登录检查逻辑
        HttpSession session = request.getSession();
        Object loginUser = session.getAttribute("loginUser");
        if(loginUser != null) {
            return true;
        }
        return false;
    }
}
```

2.配置拦截器请求

* `/**`：表示所有的请求，包括静态资源请求

```java
@Configuration
public class AdminWebConfig implements WebMvcConfigurer {
    /**
     * 将拦截器注册进容器中
     * @param registry
     */
    @Override
    public void addInterceptors(InterceptorRegistry registry) {
        // 1. 将拦截器注册
        InterceptorRegistration interceptorRegistration = registry.addInterceptor(new LoginInterceptor());
        // 2. 设置拦截哪些请求，放行哪些请求
        interceptorRegistration.addPathPatterns("/**").
                excludePathPatterns("/", "/login", "/css/**", "/js/**","/fonts/**","/images/**");
    }
}
```

# 七、文件上传

`HTML`

```html
<div class="panel-body">
    <form role="form" th:action="@{/upload}" method="post" enctype="multipart/form-data">
        <div class="form-group">
            <label for="InputEmail">邮箱</label>
            <input type="text" name="email" class="form-control" id="InputEmail" placeholder="电子邮箱">
        </div>
        <div class="form-group">
            <label for="InputUsername">名字</label>
            <input type="text" name="username" class="form-control" id="InputUsername" placeholder="姓名">
        </div>
        <div class="form-group"> <!--单文件上传-->
            <label for="InputImg">头像</label>
            <input type="file" name="headImg" id="InputImg">
            <p class="help-block">从这上传一张头像</p>
        </div>
        <div class="form-group"> <!--多文件上传-->
            <label for="InputImg">生活照片</label>
            <input type="file" name="lifeImgs" multiple>
            <p class="help-block">从这上传若干张生活照片</p>
        </div>
        <button type="submit" class="btn btn-primary">提交</button>
    </form>
</div>
```

`Java Controller`

```java
    @PostMapping(value = "/upload")
    public String upload(@RequestParam("email") String email,
                         @RequestParam("username") String username,
                         @RequestPart("headImg") MultipartFile headImg,
                         @RequestPart("lifeImgs") MultipartFile[] lifeImgs) throws IOException {
        if(!headImg.isEmpty()) {
            String originalFilename = headImg.getOriginalFilename();
            headImg.transferTo(new File("D:\\SpringBootDemo\\imgs\\" + originalFilename));
        }
        if(lifeImgs.length > 0) {
            for(MultipartFile img : lifeImgs) {
                if(!img.isEmpty()) {
                    String originalFilename = img.getOriginalFilename();
                    img.transferTo(new File("D:\\SpringBootDemo\\imgs\\" + originalFilename));
                }
            }
        }
        return "main";
    }
```



# 八、原生组件注入

前置设置：增加扫描注解`@ServletComponentScan`

## 1. servlet

编写`servlet/MyServlet`

```java
@WebServlet(urlPatterns = "/servletTest")
public class MyServlet extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        resp.getWriter().write("hello");
    }
}
```



