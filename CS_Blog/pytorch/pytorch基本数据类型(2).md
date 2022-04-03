# Pytorch基本数据类型及操作(2)

## 1. 索引选取

```python
a = torch.rand(4, 3, 28, 28)     # 定义a是一个 4张28*28的RGB图 的张量

# 单个选取
print(a.shape)          # torch.Size([4, 3, 28, 28])
print(a[0].shape)       # torch.Size([3, 28, 28])
print(a[0, 0].shape)    # torch.Size([28, 28])
print(a[0, 0, 0].shape) # torch.Size([28])
print(a[0, 0, 2, 4].shape)  # torch.Size([])
```

* a[0]：理解为取第 0 张图片，这张图片有 3 个通道，每个通道都是 28 * 28 的
* a[0, 0]：理解为取第 0 张图片的第 0 个通道，这个通道是 28 * 28 的
* a[0, 0, 0]：理解为取第 0 张图片的第 0 个通道的第 0 行像素点，这一行一共有 28 个像素点
* a[0, 0, 2, 4]：理解为取第 0 张图片的第 0 个通道的第 2 行第 4 列的像素点，它是一个点
* a[:2]：理解为取第 1 张和第 2 张和第 3 张图片，每张图片有 3 个通道，每个通道都是 28 * 28 的

## 2. 切片选取

```python
a = torch.rand(4, 3, 28, 28)     # 定义a是一个 4张28*28的RGB图 的张量

# 连续选取
print(a[:2].shape)              # torch.Size([2, 3, 28, 28])
print(a[:2, :1, :, :].shape)    # torch.Size([2, 1, 28, 28])
print(a[:2, 1:, :, :].shape)    # torch.Size([2, 2, 28, 28])
print(a[:2, -1:, :, :].shape)   # torch.Size([2, 1, 28, 28])
```

* a[:2, :1]：理解为取第 1 张和第 2 张和第 3 张图片，每张图片取第 0 个通道，每个通道都是 28 * 28 的

* a[:2, 1:, :, :]：理解为取第 1 张和第 2 张和第 3 张图片，每张图片取第 1 个和第 2 个通道，每个通道都是 28 * 28 的

* a[:2, -1:, :, :]：理解为取第 1 张和第 2 张和第 3 张图片，每张图片取第 2 个通道，每个通道都是 28 * 28 的

  

##  3. 步长选取

```python
a = torch.rand(4, 3, 28, 28)     # 定义a是一个 4张28*28的RGB图 的张量

# 间隔选取
print(a[:, :, 0:28:2, 0:28:2].shape)    # torch.Size([4, 3, 14, 14])
print(a[:, :, ::2, ::2].shape)          # torch.Size([4, 3, 14, 14])
```

* a[:, :, 0:28:2, 0:28:2]：理解为取第 1 张和第 2 张和第 3 张图片，每张图片有 3 个通道，对行和列像素点以步长为2从0至28取行和列
* a[:, :, ::2, ::2]：理解为取第 1 张和第 2 张和第 3 张图片，每张图片有 3 个通道，对行和列像素点以步长为2从0至28取行和列



```python
def index_select(dim, index) 
```

* dim：表示要操作的维度
* index：表示该维度下的哪些值，这里的index必须是一个tensor，不能直接是一个list

```python
a = torch.rand(4, 3, 28, 28)     # 定义a是一个 4张28*28的RGB图 的张量

print(a.index_select(0, torch.tensor([0, 2])).shape)    # torch.Size([2, 3, 28, 28])
print(a.index_select(1, torch.tensor([1, 2])).shape)    # torch.Size([4, 2, 28, 28])
print(a.index_select(2, torch.arange(28)).shape)        # torch.Size([4, 3, 28, 28])
print(a.index_select(2, torch.arange(8)).shape)         # torch.Size([4, 3, 8, 28])
```

* a.index_select(0, torch.tensor([0, 2]))：表示要操作第 0 维度，即图片张数维度，取第 0 张和第 2 张图片
* a.index_select(1, torch.tensor([1, 2]))：表示要操作第 1 维度，即图片通道维度，取第 1 个和第 2 个通道
* a.index_select(2, torch.arange(28))：表示要操作第 2 个维度，即图片高度像素点维度，取前 28 个行
* a.index_select(2, torch.arange(8))：表示要操作第 2 个维度，即图片高度像素点维度，取前 8 个行

## 4. 用...选取

* `...`可以取代`:`，增加方便性

```python
print(a[...].shape) 		# torch.Size([4, 3, 28, 28])
print(a[0, ...].shape) 		# torch.Size([3, 28, 28])
print(a[:, 1, ...].shape) 	# torch.Size([4, 28, 28])
print(a[..., :2].shape) 	# torch.Size([4, 3, 28, 2])
```

* a[...]：表示选取所有维度（即 4 个维度）上的所有值，相当于a[:, :, :, :]
* a[0, ...]：表示选取后3个维度的所有值
* a[:, 1, ...]：表示选取第 1 和维度图片张数中所有值，选取第二个维度RGB三个通道中第1个通道，选取三四维度所有值
* a[..., :2]：表示选取前三个维度所有值，第四个维度宽度像素点维度中取前2个值

## 5. 使用mask来索引

```python
x = torch.randn(3, 4)
print(x)
'''
tensor([[-0.4146, -0.1112, -0.6213, -0.3464],
        [-1.0482,  0.2925,  1.0796,  0.1143],
        [-0.7203,  0.5699,  1.3800, -0.3570]])
'''

mask = x.ge(0.5)	
print(mask)
'''
tensor([[False, False, False, False],
        [False, False,  True, False],
        [False,  True,  True, False]])
'''

c = torch.masked_select(x, mask)
print(c)         # tensor([1.0796, 0.5699, 1.3800])
print(c.shape)   # torch.Size([3])
print(c.dim())   # 1
```

* mask = x.ge(0.5)：表示将 x 中所有大于等于 0.5 的设置为 True ，反之设置为 False ，生成由 True 和 False 组成的Tensor
* masked_select 函数是从 mask 中取出来所有值为 True 的值，形成一个 1 维的Tensor

## 6.  使用 take 打成一维

```python
def take(input, index) -> Tensor
```

* input：是一个Tensor ，将input变成一维的Tensor
* index：是一个Tensor，表示从打平后的一维input中取出哪些下标的值
* 返回值：由index为下标的值组成的一维Tensor

```python
# src是一个二维的 2 * 3 的Tensor
src = torch.tensor([
    [1, 2, 3],
    [4, 5, 6]
])

dst = torch.take(src, torch.tensor([0, 2, 5]))  
print(dst)          # tensor([1, 3, 6])
print(dst.shape)    # torch.Size([3])
print(dst.dim())    # 1
```

* torch.take(src, torch.tensor([0, 2, 5])) ：表示将 src 打平成一维后，取出下标为 0、2、5的值组织成一个一维Tensor