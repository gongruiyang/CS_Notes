# 1. shape函数的返回值

```python
import cv2 as cv

def get_image_info(image):
    print(image.shape)
    height = image.shape[0]
    width = image.shape[1]
    channels = image.shape[2]
    print("宽度：%d,高度：%d,通道：%d" % (width, height, channels))
img = cv.imread("01.jpg")
get_image_info(img)
```

```python
(640, 640, 3)
宽度：640,高度：640,通道：3
```

# 2. 像素值取反方法

（1）每一个像素取反

> 该方法是一个一个像素值进行修改，会很慢！

```python
import cv2 as cv

def access_pexels(image):
    height = image.shape[0]
    width = image.shape[1]
    channels = image.shape[2]
    for row in range(width):
        for col in range(height):
            for c in range(channels):
                pv = image[row, col, c]
                image[row, col, c] = 255 - pv;
    cv.imshow("image", image)

img = cv.imread("01.jpg")
cv.imshow("image_", img)
access_pexels(img)
cv.waitKey(0)
cv.destroyAllWindows();
```

<img src="C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210114214043720.png"/>



（2）bitwise_not接口

> 这个函数会比上一个方法运行的更快！

```python
import cv2 as cv


def func(image):
    cv.imshow("image", image)
    image_ = cv.bitwise_not(image)
    cv.imshow("image_", image_)
image = cv.imread("01.jpg")
func(image)
cv.waitKey(0)
cv.destroyAllWindows()
```

![image-20210114232218857](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210114232218857.png)

# 3. getTickCount函数和getTickFrequency函数计算函数运行时间

```python
cv.getTickCount()
cv.getTickFrequency()
```

* getTickCount获取从操作系统启动到调用该函数的地方所经历的计时周期数，可以理解为次数
* getTickFrequency获取CPU的频率，单位：次/秒

* 所以：（次数2 - 次数1）/ 频率 = 秒

> 样例

```python
import cv2 as cv

def access_pexels(image):
    height = image.shape[0]
    width = image.shape[1]
    channels = image.shape[2]
    for row in range(width):
        for col in range(height):
            for c in range(channels):
                pv = image[row, col, c]
                image[row, col, c] = 255 - pv;

img = cv.imread("01.jpg")
t1 = cv.getTickCount();
access_pexels(img)
t2 = cv.getTickCount();
running_time = 1000 * (t2 - t1) / cv.getTickFrequency()
print('running time: %s (ms)' % running_time)
```

```python
running time: 2526.2581 (ms)
```

# 4. NumPy模块中zeros函数和ones函数详解

```python
import numpy
def zeros(shape, dtype=None, order='C')	# 将值全部初始化为 0
def ones(shape, dtype=None, order='C')  # 将值全部初始化为 1，还可进行乘积运算，修改成不同的值
```

* shape表示数组的类型，可以是一维、二维、三维的

* dtype表示其中数据的类型，默认为float64（64位的float类型），也可以指定为np.uint8（无符号8位整型，数据范围在0~255，正好适合表示图片的像素值）

* order表示在内存中存储多维数组的风格，默认为'C'（以行为主顺序），也可以填 'F' （以列为主顺序）

```python
import numpy as np
import cv2 as cv

def create_image():
    img = np.zeros([300,300,3], np.uint8)   # 创建一个400*400的3通道的图片，dtype为np.uint8
    cv.imshow("img", img)
    img[:, :, 0] = np.ones([300, 300]) * 255	# 将0号通道的值变成255
    cv.imshow("img_", img)

create_image()
cv.waitKey(0)
cv.destroyAllWindows();
```

![image-20210114224240024](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210114224240024.png)

> 代码中使用zero函数创建了一个三维数组，使用这个数组表示图片，其中图片大小为300 * 300，有3个通道，由于数组中的默认值为0，所以创建出来的图片是黑色
>
> 通过ones函数修改某通道的值，以达到修改像素值，使图片颜色改变
>
> 代码中将0号通道值全部变为255，所以每一个像素点的值都是(0,0,255)，所以显示出来是蓝色

**图解zero函数和ones函数在上述代码中的作用**

![image-20210114222948345](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210114222948345.png)

> 单通道样例

```python
import numpy as np
import cv2 as cv

def create_image():
    img = np.zeros([300,300,1], np.uint8)
    cv.imshow("img", img)
    img[:, :, 0] = np.ones([300, 300]) * 127
    cv.imshow("img_", img)
 
create_image()
cv.waitKey(0)
cv.destroyAllWindows();
```

> 由于只有一个通道，这个通道的值的取值为[0,255]，若是0则代表黑色，若是255则代表白色，中间的是灰色

![image-20210114225108005](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210114225108005.png)



# 5. NumPy模块中常用方法

（1）reshape方法和fill方法

```python
import numpy

array1 = numpy.zeros([3, 3], numpy.uint8) # 创建一个3 * 3的二维数组，其中数据类型为8位的无符号整型，
print(array1)
array2 = array1.reshape([1, 9]) # 该函数可以将原有的数据个数（9个），变成其他维度的，数据的个数不能改变哦！
print(array2)
array2.fill(256) # 将数组中填入某个值，如果这个值 大于该类型的最大值，会出现高位截断
print(array2)
```

```
[[0 0 0]
 [0 0 0]
 [0 0 0]]
[[0 0 0 0 0 0 0 0 0]]
[[0 0 0 0 0 0 0 0 0]]
```

![image-20210114231632734](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210114231632734.png)

（2）array方法

```python
import numpy

one_dimension = numpy.array([1,2,3], numpy.int32)	# 创建一维数组
two_dimensions = numpy.array([[1,2,3],[4,5,6]], numpy.int32)	# 创建二维数组
three_dimensions = numpy.array([[1,2,3],[4,5,6],[7,8,9]], numpy.int32)	# 创建三维数组
print(one_dimension)
print(two_dimensions)
print(three_dimensions)
```

```
[1 2 3]
[[1 2 3]
 [4 5 6]]
[[1 2 3]
 [4 5 6]
 [7 8 9]]
```



# 6. 色彩空间

## 6.1 色彩空间的概念



## 6.2 常见的色彩空间

(1) RGB

> 电脑成像利用了图像的三原色的原理。RGB颜色模型即代表了红绿蓝三原色，每种颜色利用两位十六进制数（八位二进制数）表示该颜色的强度。其中在颜色通道大小为0时色强最弱，大小为255时色强最强。由于在所有图像通道都为0时图像呈白色，所有图像通道值为255时图像呈黑色，所以RGB图像也被称为灰度图像。
>

* RGB 色彩空间模型示意图

![image-20210114234344035](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210114234344035.png)

![image-20210114234543633](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210114234543633.png)

(2) HSV

> HSV颜色模型，通过色调（H）饱和度（S）明度（V）来表现。色调H由角度表示，取值为0-360°。从红色开始按逆时针方向计算，红色为0°，绿色为120°,蓝色为240°。它们的补色是：黄色为60°，青色为180°,品红为300°。饱和度S表示颜色接近光谱色的程度。一种颜色，可以看成是某种光谱色与白色混合的结果。其中光谱色所占的比例愈大，颜色接近光谱色的程度就愈高，颜色的饱和度也就愈高。饱和度高，颜色则深而艳。光谱色的白光成分为0，饱和度达到最高。通常取值范围为0%～100%，值越大，颜色越饱和。明度表示颜色明亮的程度，对于光源色，明度值与发光体的光亮度有关；对于物体色，此值和物体的透射比或反射比有关。通常取值范围为0%（黑）到100%（白）

![image-20210114234205399](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210114234205399.png)

(3) HIS

> HIS色彩空间模型示意图

![image-20210114234408498](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210114234408498.png)

（4）YUV (YCrCb)

> 在YUV颜色模型中，“Y”表示明亮度（Luminance或Luma），也就是灰阶值；而“U”和“V” 表示的则是色度（Chrominance或Chroma），作用是描述影像色彩及饱和度，用于指定像素的颜色。“亮度”是透过RGB输入信号来建立的，方法是将RGB信号的特定部分叠加到一起。“色度”则定义了颜色的两个方面─色调与饱和度，分别用Cr和Cb来表示。其中，Cr反映了RGB输入信号红色部分与RGB信号亮度值之间的差异。而Cb反映的是RGB输入信号蓝色部分与RGB信号亮度值之间的差异。
>  YUV颜色模型解决了彩色图像和黑白图像的兼容问题，如果只有Y信号而没有U何V信号，那么一幅彩色图像就可以背转换为黑白图像。
>  YUV颜色模型可以和RGB颜色模型相互转换。公式如下：
>  Y = 0.299R + 0.587G + 0.114B
>  U = -0.147R - 0.289G + 0.436B
>  V = 0.615R - 0.515G - 0.100B

## 6.3 不同的色彩空间之间的相互转换

```python
def cvtColor(src, code, dst=None, dstCn=None)
```

* src代表需要转换的图像
* code代表颜色映射码

### 6.3.1 HSV与RGB相互转换

| 颜色映射码（HSV < - - > RGB） |
| ----------------------------- |
| COLOR_RGB2HSV                 |
| COLOR_HSV2RGB                 |



```python
import cv2 as cv

def color_space_change(image):
    RGB2HSV_img = cv.cvtColor(image, cv.COLOR_RGB2HSV)
    cv.imshow("RGB2HSV_img", RGB2HSV_img)
    HSV2RGB_img = cv.cvtColor(RGB2HSV_img, cv.COLOR_HSV2RGB)
    cv.imshow("HSV2RGB_img", HSV2RGB_img)


img = cv.imread("01.jpg")
color_space_change(img)
cv.waitKey(0)
cv.destroyAllWindows()
```

![image-20210115000118825](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210115000118825.png)

### 6.3.2 YUV与RGB相互转换

| 颜色映射码（YUV < - - > RGB） |
| ----------------------------- |
| COLOR_RGB2YUV                 |
| COLOR_YUV2RGB                 |

```python
import cv2 as cv

def color_space_change(image):
    RGB2YUV_img = cv.cvtColor(image, cv.COLOR_RGB2YUV)
    cv.imshow("RGB2YUV_img", RGB2YUV_img)
    
    YUV2RGB_img = cv.cvtColor(RGB2YUV_img, cv.COLOR_YUV2RGB)
    cv.imshow("YUV2RGB_img", YUV2RGB_img)


img = cv.imread("01.jpg")
color_space_change(img)
cv.waitKey(0)
cv.destroyAllWindows()
```

![image-20210115000300525](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210115000300525.png)

