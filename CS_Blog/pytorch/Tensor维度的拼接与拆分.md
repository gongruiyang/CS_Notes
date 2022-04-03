# 1. 拼接与拆分常用API

* cat函数
* stack函数
* split函数
* chunk函数

# 2. 按照维度合并Tensor

### 2.1 cat函数

```python
def cat(tensors, dim) -> Tensor
```

* tensors：需要合并的Tensor
* dim：按照维度dim进行合并
* 注意：想要拼接的维度上的值可以不同，但是其它维度上的值必须相同，并且两个 Tensor 的维度最大值必须相同

```python
a = torch.rand(4, 32, 8)  # 含义： 4个班级，每个班级32个人，每个人8门课成绩
b = torch.rand(5, 32, 8)  # 含义： 5个班级，每个班级32个人，每个人8门课成绩

# 合并成绩单
c = torch.cat([a,b], dim=0)
print(c.shape)     #  torch.Size([9, 32, 8])	-> 9个班级，每个班级32个人，每个人8门课成绩
```

> 解释：从班级维度( 0D )将成绩进行合并，两个 Tensor 中的 0D 中的值可以不同，但是其他维度上的值必须相同

`二维矩阵理解 cat 函数`

按行拼接：Tensor1 是 3 行 4 列，Tensor2 是 2 行 4列，将 Tensor1 和 Tensor2 按行拼接，变成 Tensor3 是 5 行 4 列

按列拼接：Tensor1 是 3 行 4 列，Tensor2 是 3 行 5 列，将 Tensor1 和 Tensor2 按行拼接，变成 Tensor3 是 3 行 9 列

### 2.2 stack函数

```python
def stack(tensors, dim) -> Tensor
```

* tensors：需要合并的Tensor
* dim：将新产生的维度放在 dim 维度
* 注意：需要合并的这些个Tensor，维度个数必须相等，维度中的值也必须相等

```python
a = torch.rand(32, 8)  # 含义： A班级32个人，每个人8门课成绩
b = torch.rand(32, 8)  # 含义： B班级32个人，每个人8门课成绩
c = torch.rand(32, 8)  # 含义： C班级32个人，每个人8门课成绩

# 将三个班级用一个Tensor表示：增加一个维度表示班级
d = torch.stack([a, b, c], dim=0)
print(d.shape)  # torch.Size([3, 32, 8])  -> 含义：3个班级，每个班级32个人，每个人8门课成绩
```

# 3. 按照维度拆分Tensor

### 3.1 split函数

```python
def split(split_size, dim=0)
```

* dim：表示需要拆分的维度
* split_size
  * 如果是一个数字num，表示将维度为dim中的值按照num进行平均拆分成多个Tensor；
  * 如果是一个[num1, num2, num3, .....]，表示将该维度中的值按照num进行分配生成指定个数的Tensor
* 功能：按照某维度的长度来拆分

```python
d = torch.rand(3, 32, 8)  # 含义：3个班级，每个班级32个人，每个人8门课成绩
# 将这个Tensor按照班级维度进行拆分成三个班级Tensor
a, b, c = d.split([1, 1, 1], dim=0)
print(a.shape)  # torch.Size([1, 32, 8]) -> 含义： A班级32个人，每个人8门课成绩
print(b.shape)  # torch.Size([1, 32, 8]) -> 含义： B班级32个人，每个人8门课成绩
print(c.shape)  # torch.Size([1, 32, 8]) -> 含义： C班级32个人，每个人8门课成绩

c = torch.rand(4, 32, 8)  # 含义：4个班级，每个班级32个人，每个人8门课成绩
# 将这个Tensor按照班级维度拆分成两个班级为一个的Tensor
a, b = c.split(2, dim=0)
print(a.shape)  # torch.Size([2, 32, 8]) -> 含义： 该Tensor有2个班级，每个班级32个人，每个人8门课成绩
print(b.shape)  # torch.Size([2, 32, 8]) -> 含义： 该Tensor有2个班级，每个班级32个人，每个人8门课成绩
```

> 可能的报错：拆分的值 与 接收Tensor的变量的个数 不合适时
>
> ValueError: not enough values to unpack (expected 3, got 2)
>
> ValueError: too many values to unpack (expected 3)

### 3.2 chunk函数

```python
def chunk(chunks, dim=0) -> List of Tensors
```

* chunks：要产生Tensor的个数
* dim：拆分的维度
* 功能：将维度为 dim 中的值平均分给chunks个Tensor

* 按照某维度的数量来拆分

```python
d = torch.rand(6, 32, 8)  # 含义：6个班级，每个班级32个人，每个人8门课成绩
a, b, c = d.chunk(3, dim=0)
print(a.shape)  # torch.Size([2, 32, 8]) -> 含义： 该Tensor有2个班级，每个班级32个人，每个人8门课成绩
print(b.shape)  # torch.Size([2, 32, 8]) -> 含义： 该Tensor有2个班级，每个班级32个人，每个人8门课成绩
print(c.shape)  # torch.Size([2, 32, 8]) -> 含义： 该Tensor有2个班级，每个班级32个人，每个人8门课成绩

```

> 解释：将 d 中的 dim 为 0 的维度中的值平均分给 3 个Tensor
>
> 故产生的 3 个 Tensor 中 dim 为 0 的值为 6 / 3 = 2