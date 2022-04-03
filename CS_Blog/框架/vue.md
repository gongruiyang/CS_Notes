> 引入 vue  包

```html
<!-- 开发环境版本，包含了有帮助的命令行警告 -->
<script src="https://cdn.jsdelivr.net/npm/vue@2/dist/vue.js"></script>
```

# el 挂载点

> demo1

```html
<div id="app-1">
    {{ message }}     
</div>

<div id="app-2">
    <span v-bind:title="message"> 
        鼠标悬停几秒钟查看此处动态绑定的提示信息！
    </span>
</div>

<script>
    var app = new Vue({
        el: '#app-1',  // 表示管理的是 id 为 app-1 的元素
        data: {
            message: 'Hello Vue!'
        }
    })

    var app2 = new Vue({
        el: '#app-2',
        data: {
            message: '页面加载于 ' + new Date().toLocaleString()
        }
    })
</script>
```

* `el`：挂载点，挂载点代表的元素中包含的`{{ message }}`块中的`变量message`会被 `data`中的`同名变量message`中的值所替换，无法挂载到`<html>`和`<body>`上
* `{{ message }}`：差值表达式，其中 `message `是用来被替换的变量
* `选择器`：`#`表示以 `id` 为选择器，`.`表示以 `class` 为选择器
* `data`：数据对象，遵守`对象.语法`，`数组索引取值语法`



# vue指令

## 1. 内容绑定 / 事件绑定

* `v-text`：设置标签的文本值 (textContent)，与`{{}}`不同之处在于会将原值覆盖
* `v-html`：设置标签的`innerHTML`
* `v-on`：为元素绑定`事件`，指令可以简写为`@`，绑定的方法定义在`methods`中，方法内部可以通过`this`关键字来取出`data`中的数据

```html
<div id="app1">
    <h2 v-text="message"></h2>
</div>

<div id="app2">
    <h2 v-html="content"></h2>
</div>

<div id="app3">
    <input type="button" value="单击事件1" v-on:click="clickOnce">
    <input type="button" value="单击事件2" @click="clickOnce">
    <input type="button" value="双击事件1" v-on:dblclick="clickTwice">
    <input type="button" value="双击事件2" @dblclick="clickTwice">
</div>
<script src="https://cdn.jsdelivr.net/npm/vue@2/dist/vue.js"></script>
<script>
    var app3 = new Vue({
        el: "#app3",
        methods: {
            clickOnce:function() {
                alert("单击事件");
            },
            clickTwice(){
                alert("双击事件");
            }

        }
    })

    var app2 = new Vue({
        el:"#app2",
        data:{
            content:"<a href='https://www.baidu.com/' method='post'>Baidu</a>"
        }
    })

    var app1 = new Vue({
        el: "#app1",
        data:{
            message: "hello"
        }
    })
</script>
```



> demo - 计数器

```html
<div id="countNumber">
    <button  @click="minusOne">-</button>
    <span>{{number}}</span>
    <button @click="plusOne">+</button>
</div>
<script>
    var countNumber = new Vue({
        el: "#countNumber",
        methods:{
            minusOne(){
                this.number -= 1;
            },
            plusOne(){
                this.number += 1;
            }
        },
        data: {
            number: 0
        }
    })
</script>
```



## 2. 显示切换 / 属性绑定
* `v-show`：根据`表达式的真假`，切换元素的显示与隐藏，原理是修改元素的`display`属性，指令后面的表达式最终会被解析成布尔值
* `v-if`：根据`表达式的真假`，切换元素的显示与隐藏，原理是增加或删除`dom元素`
* `v-bind`：设置元素的属性，比如：src \ title \ class，`v-bind:class="isActive?'act':''"`可以改写为`:class="{act:isActive}"`，

注意：频繁切换使用`v-show`，反之使用`v-if`，原因是前者消耗较小

> v-if

```html

<div id="app">
    <button @click="changeVis" >{{vis}}</button>
    <p></p>
    <button v-if="vis">{{vis}}</button>
</div>
<script>
    var app = new Vue({
        el: "#app",
        data:{
            vis:"true"
        },
        methods:{
            changeVis(){
                this.vis = !this.vis;
            }
        }
    })
</script>
```

> v-bind

```html
<style>
    .act{
        border: 5px solid red;
    }
</style>

<div id="app">
    <img v-bind:src="imgSrc" v-bind:title="imgTit" v-bind:class="isActive?'act':''"
         @click="change">
</div>

<script>

    var app = new Vue({
        el: "#app",
        data:{
            imgSrc:"https://i.loli.net/2021/10/05/6VLhvQTgeEbUBlG.png",
            imgTit:"Flink部署",
            isActive:false
        },
        methods:{
            change(){
                this.isActive = !this.isActive;
            }
        }
    })
</script>
```




## 3. 列表循环 / 表单元素绑定
* `v-for`：
* `v-on`：
* `v-model`：



