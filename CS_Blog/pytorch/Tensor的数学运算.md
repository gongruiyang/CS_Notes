# 1. Tensor的加减乘除

```python
import torch

a = torch.ones(2, 2)
print(a)    # tensor([[1., 1.],[1., 1.]])
b = torch.ones(2)
print(b)    # tensor([1., 1.])

c = a + b
print(c)    # tensor([[2., 2.],[2., 2.]])
c = torch.add(a, b)
print(c)    # tensor([[2., 2.],[2., 2.]])

c = a - b;
print(c)    # tensor([[0., 0.],[0., 0.]])
c = torch.sub(a, b)
print(c)    # tensor([[0., 0.],[0., 0.]])

c = a * b
print(c)    # tensor([[1., 1.],[1., 1.]])
c = torch.mul(a, b)
print(c)    # tensor([[1., 1.],[1., 1.]])

c = a / b
print(c)    # tensor([[1., 1.],[1., 1.]])
c = torch.div(a, b)
print(c)    # tensor([[1., 1.],[1., 1.]])
```

> \+ 号相当于add函数
>
> \- 号相当于sub函数
>
> \* 号相当于mul函数
>
> / 号相当于div函数



# 2. matmul 函数计算矩阵相乘

> 矩阵相乘的三种计算方法：
>
> 1. @ 符号：是matmul函数的重载形式
> 2. matmul函数：可以计算高维度乘法
> 3. mm函数：只能计算二维矩阵相乘

```python
a = 3 * torch.ones(2, 2)
print(a)    # tensor([[3., 3.],[3., 3.]])
b = torch.ones(2, 2)
print(b)    # tensor([[1., 1.],[1., 1.]])

c = a @ b
print(c)    # tensor([[6., 6.],[6., 6.]])

c = torch.mm(a, b)
print(c)    # tensor([[6., 6.],[6., 6.]])

c = torch.matmul(a, b)
print(c)    # tensor([[6., 6.],[6., 6.]])
```

## 2.1 @ 运用案例：线性层计算

```python
a = torch.rand(4, 784)
b = torch.rand(512, 784)
c = a @ b.t()
print(c.shape)	# torch.Size([4, 512])
```

> 神经网络 -> 矩阵运算 -> tensor flow

## 2.2 多维Tensor的matmul

* 对于多维 Tensor 使用 matmul 函数，只会对最后两维度进行运算

```python
a = torch.rand(4, 3, 28, 64)
b = torch.rand(4, 3, 64, 32)
c = torch.matmul(a, b)
print(c.shape)  # torch.Size([4, 3, 28, 32])
```

> 如果使用 mm 函数，则会报错：
>
> RuntimeError: self must be a matrix



# 3. 幂运算

> 幂运算的计算方法：
>
> 1. pow 函数：计算 n 次方时使用，例如pow(a, 2)
> 2. \** 运算符：计算 n 次方时使用，例如 a \** 2
> 3. sqrt 函数：square root 计算平方根
> 4. rsqrt 函数：计算平方根的倒数

```python
a = torch.full([2, 2], 9)
print(a)    # tensor([[9, 9],[9, 9]])

b = a ** 2
print(b)    # tensor([[81, 81],[81, 81]])

b = a ** 0.5
print(b)    # tensor([[3., 3.],[3., 3.]])

aa = pow(a, 2)
print(b)    # tensor([[3., 3.],[3., 3.]])
```

`sqrt`和`rsqrt`只支持`torch.FloatTensor`类型

```python
a = torch.Tensor([
    [9, 9],
    [9, 9]
])

b = a.sqrt()
print(b)	# tensor([[3., 3.],[3., 3.]])

b = a.rsqrt()
print(b)	# tensor([[0.3333, 0.3333],[0.3333, 0.3333]])
```

# 4. e 与 log 运算

> 相关计算函数：
>
> 1. exp 函数：Tensor 中的值都乘以 e 
> 2. log 函数：log 以 e 为底对数运算
> 3. log2 函数：log 以 2 为底对数运算
> 4. log10 函数：log 以 10 为底对数运算

```python
a = torch.ones(2, 2)

b = torch.exp(a)
print(b)    # tensor([[2.7183, 2.7183],[2.7183, 2.7183]])

b = torch.log(a)
print(b)    # tensor([[1., 1.],[1., 1.]])

b = torch.log2(a)
print(b)    # tensor([[1.4427, 1.4427],[1.4427, 1.4427]])

b = torch.log10(a)
print(b)    # tensor([[0.4343, 0.4343],[0.4343, 0.4343]])
```

# 5. 近似值计算

> 相关计算函数：
>
> 1. floor 函数：向下取整数
> 2. ceil 函数：向上取整数
> 3. round 函数：四舍五入取整
> 4. trunc 函数：去除小数部分
> 5. frac 函数：取出小数部分

```python
a = torch.exp(torch.ones(1))
print(a)    # tensor([2.7183])

b = a.floor()
print(b)    # tensor([2.])

b = a.ceil()
print(b)    # tensor([3.])

b = a.round()   
print(b)    # tensor([3.])

b = a.trunc()
print(b)    # tensor([2.])

b = a.frac()
print(b)    # tensor([0.7183])
```

# 6. 梯度裁剪 clamp 函数

```python
def clamp(min, max) -> Tensor
```

* 只填入一个参数(min) ：小于 min 的都变为 min
* 填入两个参数(min, max)：小于 min 的值全部变为 min，大于 max 的值全部变为 max，位于 min 和 max 之间的值不变

```python
grad = torch.rand(2, 2) * 15
print(grad)     # tensor([[ 8.6183, 11.2805],[ 4.5788,  0.1278]])

b = grad.max()
print(b)        # tensor(11.2805)

b = grad.median()
print(b)        # tensor(4.5788)

b = grad.clamp(10)
print(b)        # tensor([[10.0000, 11.2805],[10.0000, 10.0000]])

b = grad.clamp(0, 10)
print(b)        # tensor([[ 8.6183, 10.0000],[ 4.5788,  0.1278]])
```

