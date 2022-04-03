# 1. 什么是激活函数

> 1959年生物学家对青蛙的神经元细胞做了一个深入的研究，研究发现：青蛙的神经元细胞有多个输入，这些多个输入经过加权后相加产生一个输出，这个输出并不是一个线性的输出，也就是说，你这个输出的值低于某个响应值时，青蛙并不会做出任何反应，这个响应值称为阈值，细胞体中模拟出来的加权函数就是激活函数，
>
> 意为当函数值高于某个阈值时，该函数才会被激活产生一个有意义的值

`图解单一神经元模型`

![image-20210302210421272](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210302210428.png)

> x0、x1、x2都是神经元细胞的 输入
>
> w0、w1、w2都是输入的 权值
>
> 细胞体中的函数 f 就是 激活函数



![image-20210302211747720](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210302211747.png)

# 2. Sigmoid 激活函数及其loss函数

`Sigmoid函数`

Sigmoid函数是一个非线性的激活函数，其数学形式及函数图如下：

![image-20210302212039806](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210302212039.png)

`Sigmoid函数的loss函数`

loss函数推导过程及函数图如下：

![image-20210302212728768](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210302212728.png)

`Sigmoid 函数特点`

> 特点：
>
> * 将连续输入的值转化为 0 到 1 之间的输出
> * 如果输入极小，输出趋近于 0 
> * 如果输入极大，输出趋近于 1
>
> 缺点：
>
> * 易出现 gradient vanishing
> * 函数输出并不是zero-centered
> * 幂运算相对来讲比较耗时



# 3. Tanh 激活函数及其loss函数

`Tanh激活函数`

tanh 读作 Hyperbolic Tangent，其函数结构和函数图如下：

![image-20210302213359003](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210302213359.png)



`Tanh函数的Loss函数`

Tanh函数推导过程及函数图如下：

![image-20210302213537191](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210302213537.png)

`Tanh函数特点`

> 特点：
>
> * 解决了zero-centered问题
> * 仍有 gradient vanishing 问题



# 4. ReLU 激活函数及其loss函数

`ReLU 激活函数`

ReLU 是 Rectified Linear Unit 的简称，其函数结构和函数图如下：

![image-20210302214009465](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210302214009.png)



``ReLU 函数的loss函数`

ReLU 函数的loss函数及函数图如下：

![image-20210302215711862](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210302215711.png)



`ReLU函数特点`

> 特点：
>
> * 解决了 gradient vanishing 问题 (在正区间)
> * 计算速度非常快，只需要判断输入是否大于 0
> * 收敛速度远快于 sigmoid 和 tanh
> * 输出不是 zero-centered



# 5. loss函数的梯度

## 5.1 Mean Squared Error

loss 函数：

![image-20210302221504382](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210302221504.png)

loss函数的梯度：

![image-20210302221524043](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210302221524.png)



`torch.autograd.grad 函数`

```python
x = torch.ones(1)
w = torch.full([1], 2., requires_grad=True) 	# 如果参数传2而不是2.： RuntimeError: Only Tensors of floating point and complex dtype can require gradients
mse = torch.nn.functional.mse_loss(torch.ones(1), x * w)
print(mse)  # tensor(1., grad_fn=<MseLossBackward>)
loss_value = torch.autograd.grad(mse, [w])
print(loss_value)   # (tensor([2.]),)
```

`loss.backward 函数`

```python
x = torch.ones(1)
w = torch.full([1], 2., requires_grad=True)
mse = torch.nn.functional.mse_loss(torch.ones(1), x * w)
print(mse)  # tensor(1., grad_fn=<MseLossBackward>)
mse.backward()
print(w.grad)   # (tensor([2.]),)
```

