# PyTorch基本数据类型

`PyTorch 分别为 CPU tensor 和 GPU tensor 定义的张量基本类型`

![image-20210223122103460](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210223122110.png)

## 1. Tensor（张量）概念

* Pytorch里面处理的最基本的操作对象就是Tensor（张量），它表示的其实就是一个**多维矩阵**，并有矩阵相关的运算操作。
* 我们将标量称为 0D 张量（0维张量），向量称为 1D 张量（1维张量），矩阵称为 2D 张量（2维张量），依次类推。

* 在使用上和 numpy 是对应的，它和 numpy 唯一的不同就：pytorch 可以在 GPU 上运行，而 numpy 不可以。所以，我们也可以使用 Tensor 来代替 numpy 的使用。

| 数据类型             | CPU Tensor                                 |
| -------------------- | :----------------------------------------- |
| 8位整数（unsigned）  | torch.ByteTensor                           |
| 8位整数（signed）    | torch.CharTensor                           |
| 16位整型（signed）   | torch.ShortTensor                          |
| 32位整型（signed）   | torch.IntTensor                            |
| 64位整型（signed）   | torch.LongTensor                           |
| 32位浮点型（signed） | torch.FloatTensor / torch.Tensor的默认类型 |
| 64位浮点型（signed） | torch.DoubleTensor                         |



## 2. Tensor创建并初始化

```python
import torch
v_int_tensor = torch.IntTensor([	# 定义一个 2 * 3的int型矩阵，从list中导入数据
    [1, 2, 3],
    [4, 5, 6]
])
v_float_tensor = torch.FloatTensor([ # 定义一个 1 * 3的float型矩阵，从list中导入数据
    [3.13, 3.33, 7.21]
])
print(v_int_tensor)
print(v_float_tensor)
```

> 输出：
>
> ```
> tensor([[1, 2, 3],
>         [4, 5, 6]], dtype=torch.int32)
> tensor([[3.1300, 3.3300, 7.2100]])
> ```

## 3. Tensor类型推断

* type(tensor)：Python内置函数，只能给出是Tensor类型，无法给出具体类型
* tensor.type()：PyTorch内置函数，能给出Tensor的具体类型
* torch.Tensor默认数据类型为torch.FloatTensor
* isinstance(tensor, torch.XXXTensor)：tensor 类型的合法化检验，可以检查 tensor 的数据类型是否为torch.XXXTensor

```python
import torch
v_default_tensor = torch.Tensor([
    [1, 2, 3],
    [4, 5, 6]
])
v_int_tensor = torch.IntTensor([
    [1, 2, 3],
    [4, 5, 6]
])
v_float_tensor = torch.FloatTensor([
    [3.13, 3.33, 7.21]
])
print(type(v_int_tensor))		# <class 'torch.Tensor'>
print(v_float_tensor.type())	# torch.FloatTensor
print(v_default_tensor.type())	# torch.FloatTensor
```

instance函数检验：

* torch.FloatTensor：放置在CPU中的数据类型
* torch.cuda.FloatTensor：放置在GPU中的数据类型
* 同一个数据，放在不同的地方，类型就不一样了

```python
import torch
v_default_tensor = torch.Tensor([
    [1, 2, 3],
    [4, 5, 6]
])
v_GPU_default_tensor = v_default_tensor.cuda();	# 将数据从CPU数据转换为GPU类型数据
print(isinstance(v_default_tensor,torch.FloatTensor))	# True
print(isinstance(v_default_tensor,torch.cuda.FloatTensor))	# False
print(isinstance(v_GPU_default_tensor,torch.FloatTensor))	# False
print(isinstance(v_GPU_default_tensor,torch.cuda.FloatTensor))	# True
```

## 4. Tensor维度与形状

* 查看 tensor 的形状
  * tensor.shape：shape 为 tensor 的属性
  * tensor.size()：size 为 tensor 的函数
* 查看 tensor 的维度
  * len(tensor.shape)
  * tensor.dim()

![image-20210223140254083](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210223140254.png)

> * `0D 张量`：0D 张量仅包含一个数字，我们也可以称 0D 张量为标量，深度学习中loss函数的值通常为 0D 张量
>
> ```python
> a = torch.tensor(1.)	# 
> print(a)	# tensor(1.)
> print(a.size())	# torch.Size([]) --> 空的类型
> print(a.shape)  # torch.Size([])
> print(a.dim())	# 0
> 
> b = torch.tensor(1.3)
> print(b)	# tensor(1.3000)
> print(b.size())	# torch.Size([])
> print(b.shape)  # torch.Size([])
> print(b.dim())	# 0
> 
> c = torch.tensor(3.3)
> print(c)    # tensor(3.3000)
> print(c.size()) # torch.Size([])
> print(c.shape)  # torch.Size([])
> print(len(c.shape)) # 0
> ```
>
> PyTorch 提供了一个非常方便的**内置函数item函数**能够将 0D 张量转换为 Python 的基本数据类型
>
> ```python
> a = torch.tensor([1.0])
> print(a.type()) 	# torch.FloatTensor
> 
> b = a.item()
> print(type(b))	# <class 'float'>
> ```
>
> * `1D 张量`：1D 张量称为**向量**，在深度学习中阈值通常为向量的形式
>   * torch.tensor 接受的只能是数据的内容；
>   * torch.Tensor 接受的可以是数据的内容，也可以是数据的形状；
>
> ```python
> a = torch.tensor([1.1]) # []中是具体数据
> print(a)    # tensor([1.1000])
> print(a.size())     # torch.Size([1])
> print(a.dim())  # 1
> 
> b = torch.Tensor(1)	# 随机生成dim为1 size为1的数据
> print(b)    # tensor([8.4490e-39])
> print(b.size())  # torch.Size([1])
> print(b.dim())  # 1
> 
> b2 = torch.Tensor(2) # 随机生成dim为1 size为2的数据
> print(b2)   # tensor([6.1290e+28, 1.8037e+28])
> print(b2.size())    # torch.Size([2])
> print(b2.dim())     # 1
> ```
>
> * `2D 张量`：2D 张量称为**矩阵**，在深度学习中常用于向量数据。在前面介绍的手写数字识别问题中，我们将 (28 x 28) 的像素矩阵打平成 (784,) 的向量特征。为了方便矩阵计算以及提高效率，我们使用批梯度下降算法，即每次训练小批量的样本数据，因此我们通常会为 (784,) 的向量特征添加一个批量维batch_size，784 就是 features 特征，即 (batch_size, features)。
>
> ```python
> a = torch.randn(2, 3)
> print(a.size())     # torch.Size([2, 3])
> print(a.dim())      # 2
> 
> # 可以为size函数传入指定索引,来获取对应维度上的元素个数
> print(a.size(0))    # 2 -> torch.randn(2, 3)中第一个元素2
> print(a.size(1))    # 3 -> torch.randn(2, 3)中第二个元素3
> 
> v_default_tensor = torch.Tensor([
>     [1, 2, 3],
>     [4, 5, 6]
> ])
> print(v_default_tensor.dim()) 		# 2
> print(v_default_tensor.size(0))   	# 2
> print(v_default_tensor.size(1))    	# 3
> ```
>
> * `3D 张量`：3D 张量通常用于时间序列的数据或者文本序列的数据，比如对于文本序列的数据，通常形状为 (batch_size, timesteps, features)：
>   * batch_size：处理的文档数；
>   * timesteps：处理每篇文档的长度，由于每篇文档长度不一，需要人为指定一个长度阈值，超过长度阈值的文档进行裁剪，少于长度阈值的文档进行填充；
>   * features：词的表示，如果使用 one-hot 编码，则 features 为构成词典的大小。如果使用 Embedding 词嵌入，则 features 为设置词嵌入的维度；
>
> ```python
> a = torch.rand(1, 2, 3)
> 
> print(a.size())     # torch.Size([1, 2, 3])
> print(a.dim())      # 3
> ```
>
> * `4D 张量`：4D 张量通常用于图像数据，形状为 (batch_size, height, width, channels) 或 (batch_size, channels, height, width)，channel 的位置和具体使用的深度学习框架有关，在 TensorFlow 2.X 中图像形状为 (batch_size, height, width, channels)，而在 PyTorch 中图像的形状为 (batch_size, channels, height, width)，这点注意即可。
>
> ```python
> a = torch.rand(2, 3, 28, 28) # 对应到现实意义：[照片个数，RGB三个通道，长，宽]
> 
> print(a.size()) # torch.Size([2, 3, 28, 28])
> print(a.dim())  # 4
> ```
>
> * `5D 张量`：5D 张量通常用于视频数据，形状为 (batch_size, frames, height, width, channels) 或 (batch_size, frames, channels, height, width)，channels 通道的位置和在图像中的一致，不同框架中可能表示 channels 通道维度不同，视频和图像数据相比仅仅是增加了 frames 帧数这一个维度。5D 以上的张量在深度学习中并不常见这里不再赘述。



## 5. Tensor与NumPy之间的转换

* NumPy -> Tensor : 内置函数from_numpy函数

```python
data_numpy = numpy.ones(3)
print(data_numpy)   # [1. 1. 1.]
data_tensor = torch.from_numpy(data_numpy)
print(data_tensor)  # tensor([1., 1., 1.], dtype=torch.float64)
```

* Tensor -> NumPy：Tensor矩阵.numpy()

```python
data_tensor = torch.randn((3, 2))
print(data_tensor)
data_numpy = data_tensor.numpy()
print(data_numpy)

​```
输出：
tensor([[ 0.9307,  1.2856],
        [-1.1095, -0.1586],
        [-0.9531, -2.0985]])
[[ 0.93066823  1.2855971 ]
 [-1.1095397  -0.15860502]
 [-0.9531248  -2.0985377 ]]
​```
```

## 6. Tensor所占内存大小

* PyTorch内置函数：**numel函数**  ---> number of element

```python
a = torch.Tensor([
    [1, 2],
    [3, 4],
    [5, 6]
])
print(a.numel())    # 6
```

## 7. 设置torch.Tensor默认数据类型

* PyTorch内置函数set_default_tensor_type(torch.DoubleTensor)设置Tensor默认数据类型为DoubleTensor

```python
a = torch.Tensor([
    [1, 2],
])
print(a.type())	# torch.FloatTensor

torch.set_default_tensor_type(torch.DoubleTensor)
# tourch.IntTesnor会爆异常

b = torch.Tensor([
    [1, 2],
])
print(b.type()) # torch.DoubleTensor
```

## 8. 随机初始化

* rand函数：从0~1之间均匀随机产生数字

```python
a = torch.rand(3, 3)
print(a)  
'''
输出：
tensor([
    [0.3535, 0.0184, 0.3210],
        [0.8187, 0.0639, 0.1270],
        [0.0299, 0.7948, 0.0381]
])
'''
```

* rand_like函数：将Tensor的shape读取出来后传给rand函数来填充内容

```python
a = torch.Tensor(3,3)
torch.rand_like(a)
print(a)

'''
输出：
tensor([[9.6429e-39, 8.4490e-39, 9.6429e-39],
        [9.2755e-39, 1.0286e-38, 9.0919e-39],
        [8.9082e-39, 9.2755e-39, 8.4490e-39]])
'''
```

* randint(min,max)函数：从[min,max)范围内随机产生数字填充

```python
a = torch.randint(1,10,[3,3])
print(a)

'''
输出：
tensor([[8, 7, 2],
        [2, 9, 5],
        [5, 9, 3]])
'''
```

## 9. 范围顺序初始化

* range和arange函数

```python
a = torch.arange(1,10)
print(a)
b = torch.range(0, 10)
print(b)
c = torch.arange(1, 10, 3)
print(c)
'''
输出：
tensor([1, 2, 3, 4, 5, 6, 7, 8, 9])
tensor([ 0.,  1.,  2.,  3.,  4.,  5.,  6.,  7.,  8.,  9., 10.])
tensor([1, 4, 7])
UserWarning:  torch.range is deprecated and will be removed in a future release
'''
```

## 10. 范围数据切割形成Tensor

* linspace：将[min,max]范围内的数据等距切分成step个数字
* logspace：将[min,max]切割成

```python
a = torch.linspace(1,8, steps=4)
print(a)

'''
输出：
tensor([1.0000, 3.3333, 5.6667, 8.0000])
'''

a = torch.logspace(0,1,steps=10)
print(a)

'''
输出：
tensor([ 1.0000,  1.2915,  1.6681,  2.1544,  2.7826,  
		 3.5938,  4.6416,  5.9948, 7.7426, 10.0000])
'''
```

## 11. 生成特定数字

* ones：将参数形成的矩阵全部初始化为1
* ones_like：将参数中所有数据填成1
* zeros：将参数形成的矩阵全部初始化为0
* eye：将参数形成的矩阵全部初始化为单位矩阵

```python
a = torch.ones(3, 3)
b = torch.zeros(3, 3)
c = torch.eye(3)
print(a)
print(b)
print(c)

'''
输出：
tensor([[1., 1., 1.],
        [1., 1., 1.],
        [1., 1., 1.]])
tensor([[0., 0., 0.],
        [0., 0., 0.],
        [0., 0., 0.]])
tensor([[1., 0., 0.],
        [0., 1., 0.],
        [0., 0., 1.]])
'''
```

