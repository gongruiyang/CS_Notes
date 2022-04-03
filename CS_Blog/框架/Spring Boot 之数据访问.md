# 一、配置依赖

`pom.xml`

```xml
<!--mysql-->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-data-jdbc</artifactId>
        </dependency>
        <dependency>
            <groupId>mysql</groupId>
            <artifactId>mysql-connector-java</artifactId>
            <version>5.1.49</version>
        </dependency>
```

`application.yaml`

```yaml
# 配置访问的数据库名称 用户名 密码
spring:
  datasource:
    url: jdbc:mysql://localhost:3306/test?characterEncoding=UTF-8&useSSL=false
    username: root
    password: root
    driver-class-name: com.mysql.jdbc.Driver

```



# 二、编写接口

1.编写ORM中的O：`bean/City`

```java
@Data
public class City {
    private Long id;
    private String name;
    private String state;
    private String country;
}
```

2.编写ORM中的M：`mapper/CityMapper`，这是一个 interface

```java
@Mapper
public interface CityMapper {

    @Select("select * from city where id=#{id}")
    public City getByID(Long id);

    @Insert("insert into city(`name`, `state`, `country`) values(#{name}, #{state}, #{country})")
    @Options(useGeneratedKeys = true, keyProperty = "id")
    public boolean addCity(City city);
}

```

3.编写 service：`service/CityService`

```java
@Service
public class CityService {
    @Autowired
    CityMapper cityMapper;

    public City getCityById(Long id) {
        return cityMapper.getByID(id);
    }

    public boolean addCity(City city) {
        return cityMapper.addCity(city);
    }
}
```

4.编写 controller：`controller/CityController`

```java
@Controller
public class CityController {

    @Autowired
    CityService cityService;

    @ResponseBody
    @GetMapping(value = "/getCity")
    public City getCityById(@RequestParam Long id) {
        return cityService.getCityById(id);
    }

    @ResponseBody
    @PostMapping(value = "/addCity")
    public boolean addCity(City city) {
        return cityService.addCity(city);
    }
}
```

5.使用xml进行复杂SQL操作

`resources/mybatis/mapper/CityMapper.xml`

```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE mapper
        PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"
        "http://mybatis.org/dtd/mybatis-3-mapper.dtd">
<mapper namespace="com.xpu.webdev.mapper.CityMapper">
    <!-- public boolean addCity(City city); -->
    <insert id="addCity">
        insert into city(`name`, `state`, `country`) values(#{name}, #{state}, #{country})
    </insert>
</mapper>
```

`application.yaml`

```yaml
# 配置mybatis规则
mybatis:
  mapper-locations: classpath:mybatis/mapper/*.xml
```

