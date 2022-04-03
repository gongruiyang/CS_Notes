# Tensor维度变换

## 1. view / reshape

* 在 Pytorch 0.3 时，使用的默认 API 是 view
* 在 Pytorch 0.4 时，为了与numpy一致，增加了 reshape 方法
* 保证其**元素个数不变**的前提下，任意改变其维度
* 若改变了元素个数，就会报错

### 1.1 view 函数

```python
a = torch.rand(2, 1, 2, 2)	# 共有 2 * 1 * 2 * 2 = 8 个元素
print(a.shape)  # torch.Size([2, 1, 2, 2])
print(a.numel())	# 8
print(a)
'''
tensor([
        [[[0.6904, 0.6917],[0.1554, 0.4077]]],
        [[[0.7704, 0.3776],[0.5143, 0.8417]]]
        ])
'''

b = a.view(2, 2 * 2)	# 表示把 1 2 3三个维度合并成 1 个维度，第 0 维度不动
print(b.shape)  # torch.Size([2, 4])
print(b.numel())	# 8
print(b)
'''
tensor([[0.6904, 0.6917, 0.1554, 0.4077],
        [0.7704, 0.3776, 0.5143, 0.8417]])
'''

c = b.view(8)
print(c.shape) 		# torch.Size([8])
print(c.numel())	# 8
print(c)
'''
tensor([0.6904, 0.6917, 0.1554, 0.4077, 0.7704, 0.3776, 0.5143, 0.8417])
'''

d = c.view(8, 1, 1, 1, 1, 1, 1, 1, 1, 1)
print(d.shape)  # torch.Size([8, 1, 1, 1, 1, 1, 1, 1, 1, 1])
print(d)
'''
tensor([[[[[[[[[[0.6904]]]]]]]]],
        [[[[[[[[[0.6917]]]]]]]]],
        [[[[[[[[[0.1554]]]]]]]]],
        [[[[[[[[[0.4077]]]]]]]]],
        [[[[[[[[[0.7704]]]]]]]]],
        [[[[[[[[[0.3776]]]]]]]]],
        [[[[[[[[[0.5143]]]]]]]]],
        [[[[[[[[[0.8417]]]]]]]]]])
'''

print(a.dim())		# 4维度
print(b.dim())		# 2维度
print(c.dim())		# 1维度
print(d.dim())  	# 10w维度
```

### 1.2 reshape 函数

* 与 view 函数用法一致

```python
a = torch.ones(2, 1,  2)
print(a.shape)      # torch.Size([2, 1, 2, 2])
print(a.dim())      # 3
print(a.numel())    # 4
print(a)
'''
tensor([
        [[1., 1.]],
        [[1., 1.]]
       ])
'''


b = a.reshape(4)
print(b.shape)  # torch.Size([2, 4])
print(b.dim())  # 1
print(b.numel())    # 4
print(b)
'''
tensor([1., 1., 1., 1.])
'''
```



## 2. squeeze / unsqueeze

### 2.1 unsqueeze 函数

```python
def unsqueeze(dim) -> Tensor
```

* unsqueeze：展开，增加一个维度，但是不改变数据个数
* dim ： 参数范围 dim ∈ [ -(原dimMax + 1), (原dimMax + 1) )，能取左边界，无法取右边界
* 功能：在 dim 维度处插入一个维度，原 dim 维度及后续维度后移

参数dim正数范围： [ 0,  原 dimMax ]，下面测试代码为 dim∈ [0, 4]  ，如果在第 5 维度新增维度会报错

```python
import torch

a = torch.ones(4, 3, 28, 28)
print(a.dim())  # 4
print(a.shape)  # torch.Size([4, 3, 28, 28])

b = a.unsqueeze(0)  # 在 0 维度处插入一个维度
print(b.dim())  # 5
print(b.shape)  # torch.Size([1, 4, 3, 28, 28])

b = a.unsqueeze(1)  # 在 1 维度处插入一个维度
print(b.dim())  # 5
print(b.shape)  # torch.Size([4, 1, 3, 28, 28])

b = a.unsqueeze(2)  # 在 2 维度处插入一个维度
print(b.dim())  # 5
print(b.shape)  # torch.Size([4, 3, 1, 28, 28])

b = a.unsqueeze(3)  # 在 3 维度处插入一个维度
print(b.dim())  # 5
print(b.shape)  # torch.Size([4, 3, 28, 1, 28])

b = a.unsqueeze(4)  # 在 4 维度处插入一个维度
print(b.dim())  # 5
print(b.shape)  # torch.Size([4, 3, 28, 28, 1])

b = a.unsqueeze(5)  # 在 5 维度处插入一个维度
print(b.dim())  
print(b.shape)  
# IndexError: Dimension out of range (expected to be in range of [-5, 4], but got 5)
```

参数dim负数范围： [  - ( 原 dimMax + 1 ), 0 ]，下面测试代码为 dim∈ [ -5,  0 ]  

```python
import torch

a = torch.ones(4, 3, 28, 28)
print(a.dim())  # 4
print(a.shape)  # torch.Size([4, 3, 28, 28])

b = a.unsqueeze(0)  # 在 0 维度前面插入一个新维度
print(b.dim())  # 5
print(b.shape)  # torch.Size([1, 4, 3, 28, 28])

b = a.unsqueeze(-1)  # 在 4 维度处插入一个维度
print(b.dim())  # 5
print(b.shape)  # torch.Size([4, 3, 28, 28, 1])

b = a.unsqueeze(-2)  # 在 3 维度处插入一个维度
print(b.dim())  # 5
print(b.shape)  # torch.Size([4, 3, 28, 1, 28])

b = a.unsqueeze(-3)  # 在 2 维度处插入一个维度
print(b.dim())  # 5
print(b.shape)  # torch.Size([4, 3, 1, 28, 28])

b = a.unsqueeze(-4)  # 在 1 维度处插入一个维度
print(b.dim())  # 5
print(b.shape)  # torch.Size([4, 1, 3, 28, 28])

b = a.unsqueeze(-5)  # 在 0 维度处插入一个维度
print(b.dim())  # 5
print(b.shape)  # torch.Size([1, 4, 3, 28, 28])
```

#### 案例

* 将 b Tensor 的维度变成和 f Tensor 的维度一样

```python
b = torch.rand(32)
f = torch.rand(4, 32, 14, 14)
b = b.unsqueeze(1).unsqueeze(2).unsqueeze(0)
print(b.shape)	# torch.Size([1, 32, 1, 1])
```

### 2.2 squeeze 函数

```python
def squeeze(dim=None) -> Tensor
```

* 参数不填：将所有维度中值为1的维度删除
* 填入正dim：将dim维度删除，若dim超过原维度最大值，则报错
* 填入负dim：将倒数dim维度删除，若该dim中的值不为 1 则不删除，只有 dim 中的值为 1 才删除该维度

参数dim正数范围：[1, 原 dimMax]

```python
a = torch.ones(1, 1, 2, 2)
print(a.dim())  # 4
print(a.shape)  # torch.Size([1, 1, 2, 2])

b = a.squeeze()
print(b.dim())  # 2
print(b.shape)  # torch.Size([2, 2])

b = a.squeeze(0)
print(b.dim())  # 3
print(b.shape)  # torch.Size([1, 2, 2])

b = a.squeeze(1)
print(b.dim())  # 3
print(b.shape)  # torch.Size([1, 2, 2])

b = a.squeeze(2)
print(b.dim())  # 2
print(b.shape)  # torch.Size([2, 2])

b = a.squeeze(3)
print(b.dim())  # 4
print(b.shape)  # torch.Size([1, 1, 2, 2])

b = a.squeeze(4)
print(b.dim()) 
print(b.shape)  
# IndexError: Dimension out of range (expected to be in range of [-4, 3], but got 4)
```

参数dim负数范围：[- 原 dimMax,  -1]

```python
a = torch.ones(1, 1, 2, 2)
print(a.dim())  # 4
print(a.shape)  # torch.Size([4, 3, 28, 28])

b = a.squeeze()
print(b.dim())  # 2
print(b.shape)  # torch.Size([2, 2])

b = a.squeeze(0)
print(b.dim())  # 3
print(b.shape)  # torch.Size([1, 2, 2])

b = a.squeeze(-1)
print(b.dim())  # 4
print(b.shape)  # torch.Size([1, 1, 2, 2])

b = a.squeeze(-2)
print(b.dim())  # 4
print(b.shape)  # torch.Size([1, 1, 2, 2])

b = a.squeeze(-3)
print(b.dim())  # 3
print(b.shape)  # torch.Size([1, 2, 2])

b = a.squeeze(-4)
print(b.dim())  # 3
print(b.shape)  # torch.Size([1, 2, 2])
```



## 3. expand / repeat

### 3.1 expand 函数

```python
def expand(*sizes) -> Tensor
```

* 按照传入的参数，将原 dim 中的值个数按照参数的值进行修改，并将其中的值按原有的值进行初始化
* 原维度中的值必须 小于等于 改变后的维度中的值
* 将不想改变的维度中填入 -1 后，该维度中的值不会改变

```python
a = torch.rand(2, 1, 2, 2)
b = torch.rand(2, 1, 1, 1)
c = b.expand(a.shape)
print(c.shape)  # torch.Size([2, 1, 2, 2])
print(a)
print(b)
print(c)
'''
tensor([[[[0.4356, 0.0878],
          [0.3151, 0.1201]]],
        [[[0.3978, 0.2646],
          [0.0629, 0.9532]]]])
tensor([[[[0.9870]]],
        [[[0.1181]]]])
tensor([[[[0.9870, 0.9870],
          [0.9870, 0.9870]]],
        [[[0.1181, 0.1181],
          [0.1181, 0.1181]]]])
'''
```

### 3.2 repeat 函数

```python
def repeat(*sizes) -> Tensor
```

* 将原维度中的值进行复制sizes次

```python
b = torch.rand(2, 1, 1, 1)
c = b.repeat(1, 2, 1, 2)
print(c.shape)  # torch.Size([2, 2, 1, 2])
print(c)
'''
tensor([[[[0.2657, 0.2657]],
         [[0.2657, 0.2657]]],
        [[[0.8223, 0.8223]],
         [[0.8223, 0.8223]]]])
'''
```

> 代码解释：
>
> c = b.repeat(1, 2, 1, 2)
>
> 将 b 的 0 维度中的值复制 1 次，所以复制之后 0 维度中有 2 * 1 个值
>
> 将 b 的 1 维度中的值复制 2 次，所以复制之后 1 维度中有 1 * 2 个值
>
> 将 b 的 2 维度中的值复制 1 次，所以复制之后 2 维度中有 1 * 1 个值
>
> 将 b 的 3 维度中的值复制 2 次，所以复制之后 3 维度中有 1 * 2 个值

## 4.  矩阵转置

### 4.1 t 函数

```python
def t() -> Tensor
```

* 将矩阵转置
* 只适用于 2D 的矩阵，其他多维度都不适用

```python
a = torch.randn(2, 2)
print(a)
b = a.t()
print(b)
'''
tensor([[-0.6856,  0.7479],
        [ 0.7589,  0.6101]])
tensor([[-0.6856,  0.7589],
        [ 0.7479,  0.6101]])
'''
```

### 4.2 transpose 函数

```python
def transpose(dim0, dim1) -> Tensor
```

* 功能：将 dim0 维度与 dim1 维度进行交换

```python
a = torch.randn(4, 3, 28, 28)
print(a.shape)      # torch.Size([4, 3, 28, 28])
a1 = a.transpose(0, 3)
print(a1.shape)     # torch.Size([28, 3, 28, 4])
```

#### 案例：数据污染

* `注意`：view 函数运行之后需要记住原维度及size才能恢复原状，要不然可能会产生数据污染，如下代码所示

```python
a = torch.randn(4, 3, 32, 32)

a1 = a.transpose(1, 3).contiguous().view(4, 3 * 32 * 32).view(4, 3, 32, 32)
a2 = a.transpose(1, 3).contiguous().view(4, 3 * 32 * 32).view(4, 32, 32, 3).transpose(1, 3)

# 比较是否完全一样
print(torch.all(torch.eq(a, a1)))   # tensor(False)
print(torch.all(torch.eq(a, a2)))   # tensor(True)
```



### 4.3 permute 函数

```python
def permute(*dims) -> Tensor
```

* 对维度顺序进行指定

```python
a = torch.rand(4, 3, 28, 28)
b = a.permute(0, 2, 3, 1)
print(b.shape)  # torch.Size([4, 28, 28, 3])
```

> 解释：
>
> 将原来的 0 维度放置在 0 维度处
>
> 将原来的 2 维度放置在 1 维度处
>
> 将原来的 3 维度放置在 2 维度处
>
> 将原来的 1 维度放置在 3 维度处



## 5. Broadcasting

> 特点：
>
> * 自动扩展维度并改变维度中的值：自动调用 expand 函数
> * 没有数据拷贝：不需要拷贝数据
>
> 例如：Tensor A [4, 32, 14, 14]    ;   Tensor B [32, 1, 1]
>
> 将 B 转化为 A ：先扩展维度，再扩展维度中的值
>
> [32, 1, 1] -> [1, 32, 1, 1] -> [4, 32, 14, 14]

