# 数字图像的基本概念

## 1. 图像的RGB色彩模式

RGB三个颜色通道的变化和叠加得到各种颜色，其中

* R 红色，取值范围[0,255]，大小为1字节
* G 绿色，取值范围[0,255]，大小为1字节
* B 蓝色，取值范围[0,255]，大小为1字节

这三种颜色是三原色，是指色彩中不能再进行分解的三种基本颜色

常见的几种颜色的叠加：

* 红色的RGB表示（255，0，0）
* 绿色的RGB表示（0，255，0）
* 蓝色的RGB表示（0，0，255）
* 黑色的RGB表示（0，0，0）
* 白色的RGB表示（255，255，255）



## 2. 像素阵列

数字图像可以看成一个整数阵列，阵列中的元素称为像素（Pixel）

![image-20210111154043930](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210111154043930.png)

* 每个像素点代表1个像素，一点包含RGB三种颜色的组合，一个像素点包含3个字节的信息

* 假如这个点是红色，则包含的信息为（255, 0, 0）

* 理论上，我们只需要操作每个点上的这三个数字，就能得到任何图形

* 一幅图像上的所有像素点的信息，完全可以使用整数矩阵来表示

* 通过矩阵的运算实现复杂的图像操作



# Image模块

## 1. 打开图像和新建图像

### (1) 打开已有图像

> 使用open函数打开一个已经存在的图像

```python
def open(fp, mode="r", formats=None)
```

* 参数

fp表示要打开文件的路径（相对路径或者绝对路径）

mode表示打开文件的形式，通常使用默认值r，即不需要传入

* 返回值：一个图像的实例



> 打开图像函数程序演示

```python
# 导入包
from PIL import Image
# 以 默认方式 打开图片
img1 = Image.open("picture/杯子.png")
# 显示图片
img1.show()
```

> 程序运行结果

<img src="C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210111160030114.png" alt="image-20210111160030114" style="zoom:33%;" />

### (2) 新建一个图像

> 使用new函数新建一个图像

```python
def new(mode, size, color=0)
```

* 参数

mode表示新建图像的形式

size表示图像的大小

color表示图像的颜色



> new函数演示

```python
from PIL import Image
img1 = Image.new("RGB", (300,300),(255,255,255))
img1.show()
```

> 运行结果

![image-20210111162035687](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210111162035687.png)

## 2. 查看图像的信息

| 属性   | 含义                              |
| ------ | --------------------------------- |
| format | 图片格式（后缀名）                |
| size   | 图片尺寸（宽度和高度组成的tuple） |
| width  | 图片宽度                          |
| height | 图片高度                          |

> 获取某个点的RPG像素值函数

```python
def getpixel(self, xy)
```

* 参数：点坐标
* 返回值：RPG三个值组成的tuple



> 查看图像信息程序演示

```python
from PIL import Image
img1 = Image.open("picture/杯子.png")
# 查看图片信息
print('图像格式：',img1.format)
print('图像大小（宽度，高度）：',img1.size)
print('图片的高度：',img1.height,'图片的宽度：',img1.width)
print('获取(100,100)处像素值：',img1.getpixel((100,100)))
```

> 程序输出结果

```
图像格式： JPEG
图像大小（宽度，高度）： (2000, 1333)
图片的高度： 1333 图片的宽度： 2000
获取(100,100)处像素值： (26, 49, 33)
```

## 3. 混合处理图像

### (1) 透明度混合处理图像

> 函数blend实现透明度混合处理

```python
def blend(img1,img2,alpha)
```

* 其中img1和img2指参与混合的图片1和图片2，他们的尺寸必须相同
* alpha指混合透明度，取值为[0,1]
* blend函数可以将两个图片以一定的透明度进行混合

* 混合原理：img1 * (1 - alpha) + img2 * alpha
* 当alpha为0时，混合出来的图片为img2；当alpha为1时，混合出来的图片为img1



> blend函数演示

```python
from PIL import Image
img1 = Image.open("picture/cup.jpg").convert("RGB")
img2 = Image.new("RGB", img1.size, "red")
img3 = Image.blend(img1, img2, alpha=0.5)
img3.show()
```



> 运行结果

<img src="C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210111162644327.png" alt="image-20210111162644327" style="zoom:33%;" />

### (2) 遮罩混合处理图像

> 函数composite实现遮罩混合处理

```python
def composite(image1, image2, mask)
```

* image1 、 image2 、mask 分别是三张图片
* 三张图片尺寸必须相同
* 用mask来混合image1 和image2



> composite函数演示

```python
from PIL import Image
img1 = Image.open("picture\cup.jpg")
img2 = Image.open("picture\杯子.png").resize(img1.size)
r,g,b = img2.split()	# 将img2分割成3个通道
Image.composite(img1,img2,b).show() # 使用img2的b通道遮罩img1
```

> 程序输出结果

<img src="C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210111164016563.png" alt="image-20210111164016563" style="zoom:33%;" />

## 4. 复制和缩放图像

### (1) 复制图像

> 函数copy复制指定的图片

```python
def copy(self)
```



> 函数copy演示

```pyth
from PIL import Image
img1 = Image.open("picture\cup.jpg")
img2 = img1.copy()
img2.show()
print(id(img1))
print(id(img2))
```

> 输出结果

```python
1706724532872
1706724069000
```

<img src="C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210111165235150.png" alt="image-20210111165235150" style="zoom:33%;" />



### (2) 缩放像素值的RPG

> 使用eval函数缩放图像

```python
def eval(image, *args)
```

* 将image图片的每一个像素点的RPG值按照函数指针args中定义的方式进行缩放



> eval函数演示

```python
from PIL import Image
img1 = Image.open("picture\cup.jpg")
img1_bigger = Image.eval(img1, lambda x: x / 10)
print(img1.getpixel((100,100)))
print(img1_bigger.getpixel((100,100)))
img1_bigger.show()
```

> 输出结果：缩放图的点(100,100)的RPG值是原图的十分之一

```python
(26, 49, 33)
(2, 4, 3)
```





<img src="C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210111170424551.png" alt="image-20210111170424551" style="zoom:33%;" />



### (3) 原生缩放图像大小

> thumbnail函数

```
def thumbnail(self, size, resample=BICUBIC, reducing_gap=2.0)
```

* 按照size缩放按比例缩放原图



> thumbna函数演示

```python
from PIL import Image
img1 = Image.open("picture\cup.jpg")
print(img1.size)
img1.thumbnail((200, 100))
print(img1.size)
```

> 运行结果

```python
(2000, 1333)
(150, 100)
```



## 5. 剪切和黏贴图片

### (1) 剪切图片函数

> 剪切图片函数crop

```python
def crop(self, box=None)
```

* 参数box是一个四元组，分别定义了剪切区域的左、上、右、下四个坐标

### (2) 黏贴图片函数

> 粘贴图片函数paste

```python
def paste(self, im, box=None, mask=None)
```

* im是要黏贴的目标图片
* box是黏贴的区域，分为三种情况

| box           | 含义                             |
| ------------- | -------------------------------- |
| (x1,y1)       | 将源图像左上角对齐(x1,y1)点      |
| (x1,y1,x2,y2) | 源图像与该区域必须大小一致       |
| None          | 源图像与被黏贴的图像大小必须一致 |



> 剪切黏贴函数演示程序

```python
from PIL import Image
img1 = Image.open("picture\cup.jpg")
# 将img1拷贝两份
img2 = img1.copy()
img3 = img1.copy().resize((500,400))
# 将img3整张图片剪切出来
img_region = img3.crop((0,0,img3.width,img3.height))
# 将剪切出来的img_region左上角的点定位放在img2的(30,30)处
img2.paste(img_region,(30,30))
img2.show()
```

> 运行结果

<img src="C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210111172951640.png" alt="image-20210111172951640" style="zoom:33%;" />

## 6. 图像旋转

> rotate函数实现图像旋转

```python
rotate(
        self,
        angle,
        resample=NEAREST,
        expand=0,
        center=None,
        translate=None,
        fillcolor=None,
    )
```

* 将图片旋转angle度,其他参数可不填



> rotate函数演示

```python
from PIL import Image
img1 = Image.open("picture\cup.jpg")
img_rotate = img1.rotate(90)
img_rotate.show()
```

> 运行结果

<img src="C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210111173937045.png" alt="image-20210111173937045" style="zoom:33%;" />





## 7. 图像通道的分割与合并

### (1) 图像分割为通道函数

> split函数将图片分割为多个**通道列表**

```python
def split(self)
```



### (2) 通道合并入图像函数

> merge函数可以将**一个通道的图像**合并到**更多通道的图像**当中

```python
def merge(mode, bands)
```



> 对指定图片进行合并和分离操作

```python
from PIL import Image
img1 = Image.open("picture\cup.jpg")
img2 = Image.open("picture\杯子.png").resize(img1.size)
# 分离
r1,g1,b1 = img1.split()
r2,g2,b2 = img2.split()
# 合并
tmp = [r1,g2,b1]
img = Image.merge("RGB",tmp)
img.show()
```

> 运行结果

<img src="C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210111175445372.png" alt="image-20210111175445372" style="zoom:33%;" />



## 8.  滤镜效果

>在ImageFilter模块中，函数filter可以使用特定滤镜过滤指定的图像

```python
def filter(self, filter)
```

* filter表示滤镜内核



> 函数filter对图像进行**高斯模糊**

```python
from PIL import Image,ImageFilter
img = Image.open("picture\cup.jpg")
# 创建一个 与img同高但两倍宽的空白图 
img_output = Image.new("RGB", (2 * img.width, img.height))
# 将img放在空白图左侧
img_output.paste(img, (0, 0))
# 将img进行高斯模糊
img_filter = img.filter(ImageFilter.GaussianBlur)
# 将高斯模糊后的img放在空白图的右侧
img_output.paste(img_filter,(img.width, 0))
# 将拼接好的原图和高斯模糊的图进行对比展示
img_output.show()
```

> 输出结果



<img src="C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210111185105229.png" alt="image-20210111185105229" style="zoom:33%;" />



> 其他滤镜内核演示

* 四个一起对比

```python
img = Image.open("picture\cup.jpg")
img_output = Image.new("RGB", (img.width * 4, img.height))

img_GaussianBlur = img.filter(ImageFilter.GaussianBlur)     # 高斯模糊
img_EDGE_ENHANCE = img.filter(ImageFilter.EDGE_ENHANCE)     # 边缘增强滤镜
img_FIND_EDGES   = img.filter(ImageFilter.FIND_EDGES)       # 寻找边缘滤镜

# 将原图与另外三个图拼接在一起
img_output.paste(img, (0, 0))
img_output.paste(img_GaussianBlur, (img.width, 0))
img_output.paste(img_EDGE_ENHANCE, (img.width * 2, 0))
img_output.paste(img_FIND_EDGES, (img.width * 3, 0))

img_output.show()
```

* 两两对比

```python
from PIL import Image,ImageFilter

img = Image.open("picture\cup.jpg")

img_GaussianBlur = img.filter(ImageFilter.GaussianBlur)     # 高斯模糊
img_EDGE_ENHANCE = img.filter(ImageFilter.EDGE_ENHANCE)     # 边缘增强滤镜
img_FIND_EDGES   = img.filter(ImageFilter.FIND_EDGES)       # 寻找边缘滤镜

filters = []
filters.append(img_GaussianBlur)
filters.append(img_EDGE_ENHANCE)
filters.append(img_FIND_EDGES)

img_output  = Image.new("RGB", (img.width * 2, img.height))
img_output.paste(img, (0, 0))
for _filter in filters:
    img_output.paste(_filter, (img.width, 0))
    img_output.show()

```



> 四个一起对比输出结果

<img src="C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210111190018688.png" alt="image-20210111190018688" style="zoom:33%;" />



## 9.像素点计算

* 对像素点进行计算，从而改变图片的亮度

```python
def point(self, lut, mode=None)
```

> 测试程序

```python
from PIL import Image

img = Image.open("picture\cup.jpg")

img_high = img.point(lambda x: x * 1.5)
img_low  = img.point(lambda x: x * 0.5)

img_output = Image.new("RGB",(img.width * 3, img.height))
img_output.paste(img, (0, 0))
img_output.paste(img_high, (img.width, 0))
img_output.paste(img_low, (img.width * 2, 0))
img_output.show()
```

<img src="C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210111210206599.png" alt="image-20210111210206599" style="zoom:33%;" />



# ImageChops模块

* PIL库中的内置模块ImageChops中包含了多个用于实现图片合成的函数
* 这些合成功能的=是通过计算通道中像素值的方式实现的
* 主要用于制作特效、合成图片等操作

内置函数：

**（1）add函数**

* 相加函数add，功能是对两张图片进行算术加法运算
* 合成后的图像中的每个像素值，按照以下公式计算： out = (image1 + image2) / scale + offset)

```python
def add(image1, image2, scale=1.0, offset=0)
```

> 函数演示

```python
from PIL import Image,ImageChops
img1 = Image.open("picture\cup.jpg")
img2 = Image.open("picture\杯子.png").resize(img1.size)

# add函数
ImageChops.add(img1, img2).show()
```

> 运行结果

<img src="C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210111201806065.png" alt="image-20210111201806065" style="zoom:33%;" />

**（2）subtract函数**

* 减法函数subtract，功能是对两张图片进行算术减法运算
* 合成后的图像中的每个像素值，按照以下公式计算： out = (image1 - image2) / scale + offset)

```python
def subtract(image1, image2, scale=1.0, offset=0)
```

> 函数演示

```python
from PIL import Image,ImageChops
img1 = Image.open("picture\cup.jpg")
img2 = Image.open("picture\杯子.png").resize(img1.size)

# subtract函数
ImageChops.subtract(img1,img2).show()
```

> 运行结果

<img src="C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210111202123242.png" alt="image-20210111202123242" style="zoom:33%;" />

**（3）darker函数**

* 变量函数darker，功能是比较连个图片（逐像素比较）将两张图片较暗的部分叠加得到一张新的图片返回
* 某个像素点的取值为两张图片中较小（暗）的那个值

```python
def darker(image1, image2)
```



> 函数演示

```python
from PIL import Image,ImageChops
img1 = Image.open("picture\cup.jpg")
img2 = Image.open("picture\杯子.png").resize(img1.size)

# darker函数
ImageChops.darker(img1,img2).show()
```

> 运行结果

<img src="C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210111202144982.png" alt="image-20210111202144982" style="zoom:33%;" />

**（4）lighter函数**

* 变量函数lighter，功能是比较连个图片（逐像素比较）将两张图片较亮的部分叠加得到一张新的图片返回
* 某个像素点的取值为两张图片中较大（亮）的那个值

```python
def lighter(image1, image2)
```



> 函数演示

```python
from PIL import Image,ImageChops
img1 = Image.open("picture\cup.jpg")
img2 = Image.open("picture\杯子.png").resize(img1.size)

# lighter函数
ImageChops.lighter(img1,img2).show()
```

> 运行结果

<img src="C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210111202204830.png" alt="image-20210111202204830" style="zoom:33%;" />

**（5）multiply函数**

* 叠加函数multiply，功能室将两张图片相互叠加。若果用纯色与某图片进行叠加操作，就会得到一幅纯黑色的图片，如果用纯白色图片，则图片不受影响
* 像素的计算公式为： out = image1 * image / MAX

```python
def multiply(image1, image2)
```

> 函数演示

```python
from PIL import Image,ImageChops
img1 = Image.open("picture\cup.jpg")
img2 = Image.open("picture\杯子.png").resize(img1.size)

# multiply函数
ImageChops.multiply(img1,img2).show()
```

> 运行结果

<img src="C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210111202222501.png" alt="image-20210111202222501" style="zoom:33%;" />

**（6）screen函数**

* 屏幕函数screen，功能是先反色后叠加，实现合成图像的效果，就像将两张幻灯片用两台投影仪投影到一块屏幕上的效果
* 像素计算公式： out = MAX - ((MAX - image1) * (MAX - image2) / MAX)

```python
def screen(image1, image2)
```

> 函数演示

```python
from PIL import Image,ImageChops
img1 = Image.open("picture\cup.jpg")
img2 = Image.open("picture\杯子.png").resize(img1.size)

# screen函数
ImageChops.screen(img1,img2).show()
```

> 运行结果

<img src="C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210111202241739.png" alt="image-20210111202241739" style="zoom:33%;" />

**（7）invert函数**

* 反色函数invert，类似于集合操作中的求补集，最大值为MAX，用MAX减去每个像素值求出反色
* 像素计算公式：out = MAX - image

```python
def invert(image)
```

> 函数演示

```python
from PIL import Image,ImageChops
img1 = Image.open("picture\cup.jpg")
img2 = Image.open("picture\杯子.png").resize(img1.size)

# invert函数
ImageChops.invert(img1).show()
```

> 运行结果

<img src="C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210111202304544.png" alt="image-20210111202304544" style="zoom:33%;" />

**（8）difference函数**

* 比较函数difference，逐像素做减法再求绝对值
* 若两个像素值相同，比较后的像素值为黑色
* 像素计算公式：out = abs(image1 - image2)

```python
def difference(image1, image2)
```



> 函数演示

```python
from PIL import Image,ImageChops
img1 = Image.open("picture\cup.jpg")
img2 = Image.open("picture\杯子.png").resize(img1.size)

# difference函数
ImageChops.difference(img1,img2).show()
```

> 运行结果

<img src="C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210111202322125.png" alt="image-20210111202322125" style="zoom:33%;" />



# ImageEnhance模块

* ImageEnhance模块中包含了多个用于增强图像效果的函数，主要用来调整图像的色彩、对比度、亮度和清晰度
* 在ImageEnhance模块中，质保函一个方法enhance，该方法会返回一个增强的对象，参数是一个大于0的float数，1表示返回原始图片
* 使用enhance方法之前，需要获得对应的增强调整器，根据增强调制器调用enhance方法，根据增强系数（小于1表示减弱，大于1表示增强）进行调整，再返回调整后的图片

```python
ImageEnhance.Color(image)		# 调整图像 色彩
ImageEnhance.Contrast(image)	# 调整图像 对比度 
ImageEnhance.Brightness(image)	# 调整图像 亮度
ImageEnhance.Sharpness(image)	# 调整图像 清晰度
```

> 测试程序

```python
from PIL import Image,ImageEnhance

img = Image.open("picture\cup.jpg")
img_output = Image.new("RGB",(img.width * 3, img.height * 2))

# 获取色彩调整期
img_color = ImageEnhance.Color(img)
img_high = img_color.enhance(1.5)   # 色彩增加
img_low  = img_color.enhance(0.5)   # 色彩减弱
# 将原图复制到第一行最左边，另外两个图以此放在右边
img_output.paste(img, (0, 0))
img_output.paste(img_high, (img.width * 1, 0))
img_output.paste(img_low, (img.width * 2, 0))


# 获取亮度调整期
img_Brightness = ImageEnhance.Brightness(img)
img_high = img_Brightness.enhance(1.5)  # 亮度增强
img_low  = img_Brightness.enhance(0.5)  # 亮度减弱
# 将原图复制到第二行最左边，另外两个图以此放在右边
img_output.paste(img, (0, img.height))
img_output.paste(img_high, (img.width * 1, img.height))
img_output.paste(img_low, (img.width * 2, img.height))

img_output.show()
```

> 输出结果

<img src="C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210111205615940.png" alt="image-20210111205615940" style="zoom:33%;" />



