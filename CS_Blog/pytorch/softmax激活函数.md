# Softmax 激活函数

* 功能：不同的输入经过 softmax 激活函数后，所有的输出都在 （0, 1） 之间，其中较大的输入与其他的输入经过  softmax 函数后，他们之间的悬殊就更大了
* Softmax将多个神经元的输出，映射到（0,1）区间内，并且做了归一化，所有元素的和累加起来等于1。可以直接当作概率对待，选取概率最大的分类作为预测的目标。
* Softmax中使用了指数，这样可以让大的值更大，让小的更小，增加了区分对比度，学习效率更高

![image-20210303133259950](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210303133300.png)

>输入的 yi 中有一个 2.0 的输入，有一个 1.0 的输入，它们经过 softmax 函数后差距变得更大了



# Softmax 函数的偏导过程

`图解求偏导过程`

![image-20210303134307677](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210303134307.png)

`偏导数`

![image-20210303134437946](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210303134437.png)



# 代码模拟偏导过程

```python
import torch

a = torch.rand(3, requires_grad=True)
print(a)    # tensor([0.6645, 0.3585, 0.7826], requires_grad=True)

p = torch.nn.functional.softmax(a, dim=0)
print(p)    # tensor([0.3494, 0.2573, 0.3932], grad_fn=<SoftmaxBackward>)

ret = torch.autograd.grad(p[1], [a], retain_graph=True)
print(ret)  # (tensor([-0.0899,  0.1911, -0.1012]),)    i = 1 -> 第1个是正的
ret = torch.autograd.grad(p[2], [a])
print(ret)  # (tensor([-0.1374, -0.1012,  0.2386]),)    i = 2 -> 第2个是正的
```

