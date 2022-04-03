# 零：快速搭建一个SpringBoot项目

## 1. pom.xml 配置

> 必须配置

```xml
<parent>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-parent</artifactId>
    <version>2.6.2</version>
</parent>

<dependencies>
    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-web</artifactId>
    </dependency>
</dependencies>

<build>
    <plugins>
        <plugin>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-maven-plugin</artifactId>
        </plugin>
    </plugins>
</build>
```

* `spring-boot-starter-parent`：Spring Boot的父级依赖管理，
* `spring-boot-starter-web`：默认为提供一些SpringMVC必要的组件

* `spring-boot-maven-plugin`：将Springboot应用打包为可执行的jar或war文件，进行相应部署后即可启动Springboot应用



## 2. 主程序

```java
package com.xpu.boot;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

// 主程序类
@SpringBootApplication  // 用来告诉 SpringBoot 这是一个 SB 应用
public class MainApplication {
    public static void main(String[] args) {
        SpringApplication.run(MainApplication.class, args);
    }
}
```

* `@SpringBootApplication`：用来告诉 SpringBoot 这是一个 SB 应用

直接运行该 main 方法即可启动项目

## 3. Controller

```java
package com.xpu.boot.controller;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.bind.annotation.RestController;

//@ResponseBody   // 表示直接返回数据给浏览器 而不是 返回某个页面
//@Controller
@RestController     // @ResponseBody + @Controller
public class HelloController {

    @RequestMapping("/hello")
    public String handle01(){
        return "Hello, Spring Boot !";
    }
}
```

* `@RestController`：作用相当于 **@ResponseBody + @Controller**
* `@ResponseBody`：作用是将controller的方法返回的对象通过适当的转换器转换为指定的格式之后，写入到response对象的body区，通常用来返回JSON数据或者是XML数据。
* `@Controller`：用于标记在一个类上，使用它标记的类就是一个SpringMvc Controller对象，分发处理器会扫描使用该注解的类的方法

## 4. 配置 application.properties

用来修改服务器的一些配置信息：[Application Properties](https://docs.spring.io/spring-boot/docs/current/reference/html/application-properties.html#application-properties.core)

```properties
server.port=8088
server.address=localhost
```



# 一、Spring Boot 特点

## 1. 依赖管理

pom.xml

```xml
<parent>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-parent</artifactId>
    <version>2.6.2</version>
</parent>

<dependencies>
    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-web</artifactId>
    </dependency>
</dependencies>
```



* spring-boot-starter-parent：该父级依赖的父级依赖为**spring-boot-dependencies**，常用的 jar 包依赖的版本号都在该 pom 文件里

* spring-boot-starter-web： **spring-boot-starter-***是Spring boot 官方提供用于引入对应场景的所有常用依赖，关注[Starters](https://docs.spring.io/spring-boot/docs/current/reference/html/using.html#using.build-systems.starters)

所有增加的 dependency 都不需要写版本号，所有常见依赖都在**祖父级依赖spring-boot-dependencies**中已经配置过了，直接在 dependencies 中加即可

> 如果默认包含的依赖版本号并不是当前所需要的，如何进行调换至需要的版本号？
>
> 去[mvn repository](https://mvnrepository.com/)中寻找到所需的版本号，例如：mysql-connector-java : 5.1.43
>
> ```xml
>     <properties>
>         <mysql.version>5.1.43</mysql.version>
>     </properties>
> ```

即优先使用当前 pom 文件中的配置，如果当前文件没有配置，则用父级依赖里面的配置

## 2. 自动配置

* 自动引入依赖 + 自动配置

我们配置的 pom 文件中只引入了一个 spring-boot-starter-web 依赖，Spring Boot 自动把很多模块的依赖给自动引入与配置，例如：tomcat 、 springMVC 等

![image-20220109141559597](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202201091416758.png)

查看自动配置了哪些 BeanNames

```java
// 1. 获取 IOC 容器
ConfigurableApplicationContext run = SpringApplication.run(MainApplication.class, args);
// 2. 查看容器中的所有组件名称
String[] names = run.getBeanDefinitionNames();
for (String name : names) {
    System.out.println(name);
}
```

* 默认的包结构

主程序所在的包及所有子包里面的所有组件都会被默认扫描进来，无需以前的包扫描配置

想要改变默认扫描路径方法：在主程序类使用以下其中一个注解即可

1. `@SpringBootApplication(scanBasePackages = "com.xpu")`
2. `@ComponentScan("com.xpu")`

其实第一个注解相当于以下三个注解：

```java
@SpringBootConfiguration
@EnableAutoConfiguration
@ComponentScan("com.xpu")
```

* 配置都有默认值

配置文件中的值都会绑定到某个对象

例如：server 与 ServerProperties 类绑定

* 自动配置按需加载

# 二、容器功能

## 1. 组件添加

### 1.1 old method

> 使用 Spring 配置文件加入组件，例如beans.xml
>
> ```xml
>     <bean id="user01" class="com.xpu.boot.bean.User">
>         <property name="name" value="zhangsan"></property>
>         <property name="age" value="18"></property>
>     </bean>
> 
>     <bean id="pet01" class="com.xpu.boot.bean.Pet">
>         <property name="name" value="mike"></property>
>         <property name="age" value="2"></property>
>     </bean>
> ```

### 1.2 @Configuration / @Bean

`@Configuration`：作用是告诉 Spring Boot 这个类是一个配置类，本身也是一个组件

`@Bean`：用于给 IOC 容器添加组件，方法名是组件id，返回类型为组件类型，返回值为容器中的单实例组件，组件获取默认为**单例模式**

MyConfig 类

```java
//@Configuration
public class MyConfig {

    //@Bean("tom")
    public User user01(){
        return new User("tom", 18);
    }
    //@Bean("mike")
    public Pet pet01(){
        return new Pet("mike", 6);
    }
}
```

Main 类

```java
// 主程序类
@SpringBootApplication
public class MainApplication {
    public static void main(String[] args) {
        // 1. 获取 IOC 容器
        ConfigurableApplicationContext run = SpringApplication.run(MainApplication.class, args);

        // 2. 获取指定单实例组件：tom 和 mike
        User tom = run.getBean("tom", User.class);
        Pet mike = run.getBean("mike", Pet.class);
        System.out.println(tom);
        System.out.println(mike);
    }
}
```

组件单实例控制：`@SpringBootApplication(proxyBeanMethods = true)`

true表示保持组件**单例模式** 会检查组件是否为单例，Spring Boot 启动加载慢，Full mode

false表示**非单例模式** 则不会检查是否为单例，Spring Boot 启动加载快，Lite mode

单例模式可以保证组件依赖：

User 中有一个 Pet 属性，proxyBeanMethods 为 true 能保证 User 中的 Pet 必须是容器中的组件，反之，则不是容器中的



### 1.3 @Bean / @Component / @ Controller / @Service / @Repository



### 1.4 @ComponentScan / @ Import

`@Import`：以数组的方式导入组件

```java
// 主程序类
@Import({User.class, Pet.class})
@SpringBootApplication()
public class MainApplication {
    public static void main(String[] args) {
        // 1. 获取 IOC 容器
        ConfigurableApplicationContext run = SpringApplication.run(MainApplication.class, args);

        // 2. 获取注解 Import 导入的无参构造的组件
        User tom = run.getBean(User.class);
        Pet mike = run.getBean(Pet.class);
        MyConfig conf = run.getBean(MyConfig.class);
        System.out.println(tom);    // User{name='null', age=0}
        System.out.println(mike);   // Pet{name='null', age=0}
    }
}

```



### 1.5 @Conditional

`@Conditional`：满足 Conditional 指定的条件，则进行组件注入

Conditional 注解继承树：

![image-20220109154103758](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202201091541838.png)

```java
@ConditionalOnBean(name = "cat")
@Configuration
public class MyConfig {

    @Bean("tom")
    public User user01(){
        return new User("tom", 18);
    }
    @Bean("mike")
    public Pet pet01(){
        return new Pet("mike", 6);
    }
}
```

当容器中存在组件名称为 “cat” 的组件时，以下的配置才生效



## 2. 原生 Spring 配置文件引入

### 2.1 @ImportResource

`@ImportResource`：表示使用原生 Spring 配置文件进行组件注入，指定配置文件路径即可

```java
// 主程序类
@ImportResource("classpath:beans.xml")
@SpringBootApplication()
public class MainApplication {
    public static void main(String[] args) {
        // 1. 获取 IOC 容器
        ConfigurableApplicationContext run = SpringApplication.run(MainApplication.class, args);

        // 2. 获取注解 Import 导入的无参构造的组件
        User tom = run.getBean("user01", User.class);
        Pet mike = run.getBean("pet01", Pet.class);
        MyConfig conf = run.getBean(MyConfig.class);
        System.out.println(tom);    // User{name='zhangsan', age=18}
        System.out.println(mike);   // Pet{name='mike', age=2}
    }
}

```



## 3. 配置文件绑定对象

### 3.1 @ConfigurationProperties

使用以下两个注解将实体类与配置文件中的以特定前缀 prefix 开始的配置项建立起联系

Pet.java

```java
@Component
@ConfigurationProperties(prefix = "mypet")
public class Pet {}
```

`@Component`可以被替换为`@EnableConfigurationProperties`，其中 @Component 必须放在实体类前，而 @EnableConfigurationProperties 可以放在任何地方，作用就是保证 @ConfigurationProperties 生效

application.properties

```properties
mypet.name="mike"
mypet.age=11
```

将组件注入后使用 controller 来通过页面获取 javabean

```java
package com.xpu.boot.controller;

import com.xpu.boot.bean.Pet;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import javax.annotation.Resource;

@RestController
public class petController {

    @Resource
    Pet pet;

    @RequestMapping("/pet")
    public Pet handle(){
        return pet;
    }
}
```

结果如图所示：

![image-20220109162843077](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202201091628149.png)





# 三、自动配置原理探究

## 加载所有自动配置类

探究 SpringBootApplication 注解组成

```java
@SpringBootConfiguration
@EnableAutoConfiguration
@ComponentScan(excludeFilters = {
    @Filter(type = FilterType.CUSTOM,classes = {TypeExcludeFilter.class}), 
    @Filter(type = FilterType.CUSTOM,classes = {AutoConfigurationExcludeFilter.class})})
public @interface SpringBootApplication {}
```



### 1. @SpringBootConfiguration

```java
@Configuration
public @interface SpringBootConfiguration {
    @AliasFor(
        annotation = Configuration.class
    )
    boolean proxyBeanMethods() default true;
}
```

由源代码可以看出`@SpringBootConfiguration`就是一个`@Configuration`，表示当前是一个配置类，所以使用`@SpringBootApplication`注解的类是 Spring Boot 核心配置类



### 2. @ComponentScan

用于指定扫描哪些包，默认情况下扫描的是下级包，如果使用 `scanBasePackages  = `可以指定扫描上级包



### 3. @EnableAutoConfiguration

由两个`@AutoConfigurationPackage`和`@Import`两个注解合成的

```java
@AutoConfigurationPackage
@Import({AutoConfigurationImportSelector.class})
public @interface EnableAutoConfiguration {}
```

#### 3.1 @AutoConfigurationPackage

实际上是一个`@Import`注解，利用 Registrar 类来向容器中导入一系列组件

```java
@Import({Registrar.class})
public @interface AutoConfigurationPackage {}
```

Register 类：函数 registerBeanDefinitions 主要作用是将**指定包名注册**

```java
static class Registrar implements ImportBeanDefinitionRegistrar, DeterminableImports {
    Registrar() {}
    
    public void registerBeanDefinitions(AnnotationMetadata metadata, BeanDefinitionRegistry registry) {
        AutoConfigurationPackages.register(registry, (String[])(new AutoConfigurationPackages.PackageImports(metadata)).getPackageNames().toArray(new String[0]));
    }
}
```

#### 3.2 @Import

1. 利用 AutoConfigurationImportSelector 类中的函数 getAutoConfigurationEntry 来获取并过滤产生所需的配置类

![image-20220109172232177](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202201091722281.png)

2. 探索 getAutoConfigurationEntry 函数发现调用链路如下：

getAutoConfigurationEntry -> SpringFactoriesLoader类loadFactoryNames方法 -> SpringFactoriesLoader类loadSpringFactories方法

3. 探究 loadSpringFactories 函数后发现，这些配置类是从`META-INF/spring.factories`中读取来的

![image-20220109173449313](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202201091734392.png)

4. 我们打开 `spring-boot-autoconfigure-2.6.2.jar!\META-INF\spring.factories`发现正好有（158 - 26 + 1 = 133）个 configurations

![image-20220109173738771](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202201091737856.png)

虽然说一股脑把 133 个场景的 configuration 都加载了进去，但是并非是所有的都会生效，而是按需开启自动配置项



## 按需开启自动装配选项

随便打开一个配置类，我们可以看到，该配置类是否生效取决于`@ConditionalOnClass`注解，说明只有当容器中有 指定类 的时候才装配以下的组件

![image-20220109175054135](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/202201091750231.png)



# 四、开发工具包

## 1. Lombok

实体类上注解

`@Data`：编译时自动生成 getter/setter 方法

`@ToString`：编译时自动生成 toString 方法

`@NoArgsConstructor`：编译时自动生成 无参构造 方法

`@AllArgsConstructor`：编译时自动生成 全参构造 方法

`@EqualsAndHashCode`：编译时自动生成 equals 和 hashcode 方法



普通注解

`@Slf4j`：包含日志模块



# 五、yaml 的用法

## 1. 简介

用来做以数据为中心的配置文件

## 2. 基本语法

1. 格式：`K: V`
2. 缩进表示层级关系，缩进只允许使用空格
3. 大小写敏感
4. \# 表示注释
5. 单双引号表示字符串内容

## 3. 实例

```yaml
person:
  userName: zhangsan
  boss: true
  birth: 2019/12/9
  age: 10
  interests:
    - basketbll
    - football
  animal: [dog, cat]
  score: {english:80, math:90}
  salary:
    - 20000
    - 10000
  pet:
    name: dog
    weight: 100.9
  allPets:
    sick:
      - {name: cat, weight: 99.9}
      - name: dog
        weight: 199
    health:
      - {name: pig, weight: 9}
      - name: dog
        weight: 1

#spring:
#  banner:
#    image:
#      location: a
```

