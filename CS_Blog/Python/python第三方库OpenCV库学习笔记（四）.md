# 1. ROI

* ROI(**region of interest**)，也就是感兴趣区域，如果你设置了图像了ROI，那么在使用OpenCV的函数的时候，会只对ROI区域操作，其他区域忽略。

```python
import cv2 as cv
import numpy

image = cv.resize(cv.imread("star.jpg"), (400, 300))
cv.imshow("image", image)
# 选取出目标区域
region = image[50:200, 100:250]
# 将目标区域变成灰色
gray = cv.cvtColor(region, cv.COLOR_BGR2GRAY)
# 再将目标区域放回原图
image[50:200, 100:250] = cv.cvtColor(gray, cv.COLOR_GRAY2BGR)
cv.imshow("image", image)
cv.waitKey(0)
cv.destroyAllWindows()
```

![image-20210117203841689](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210117203841689.png)

# 2. 泛洪填充

```python
def floodFill(image, mask, seedPoint, newVal, loDiff=None, upDiff=None, flags=None)
```

* image表示原图
* mask表示遮罩图
* seedPoint表示填充起始坐标
* newVal表示需要填充的颜色
* loDiff表示需要填充颜色的像素值的下限
* upDiff表示需要填充颜色的像素值的上限
* flags表示填充方法，彩色图像填充必须为FLOODFILL_FIXED_RANGE，二值图像使用

> 假设seedPoint的值为（R，G，B），遍历整张图片，将所有像素点的值位于（R-loDiff，G-loDiff，B-loDiff）~（R+upDiff，GR+upDiff，BR+upDiff）的点全部变成newVal颜色



**（1）填充彩色图像**

```python
import cv2 as cv
import numpy


def flood_fill_demo(image):
    copy_image = image.copy()   # 拷贝图片
    h, w = image.shape[:2]    # 获取图片宽和高
    mask = numpy.zeros([h + 2, w + 2], numpy.uint8)  # 获取一张mask，其大小必须比原图长和宽大2
    cv.floodFill(copy_image, mask, (30, 30), (0, 255, 255), (100, 100, 100), (50, 50, 50), cv.FLOODFILL_FIXED_RANGE)
    cv.imshow("flood_fill_image", copy_image)

image = cv.resize(cv.imread("star.jpg"), (400, 300))
cv.imshow("image", image)
flood_fill_demo(image)
cv.waitKey(0)
cv.destroyAllWindows()
```

![image-20210117205256795](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210117205256795.png)



（2）填充二值图像

```python
import cv2 as cv
import numpy

def fill_binary_demo():
    # 创建一张中间白周围黑的图片
    image = numpy.zeros([400, 400, 3], numpy.uint8)
    image[100:300, 100:300, :] = 255
    cv.imshow("image", image)

    # 创建遮罩图
    mask = numpy.ones([402, 402, 1], numpy.uint8)
    mask[101:301, 101:301] = 0
    # 从（200,200）点向周围填充(0, 0, 255)这个颜色
    cv.floodFill(image, mask, (200, 200), (0, 0, 255), cv.FLOODFILL_MASK_ONLY)
    cv.imshow("filled image", image)

fill_binary_demo()
cv.waitKey(0)
cv.destroyAllWindows()
```

![image-20210117211331468](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210117211331468.png)



# 3. 模糊操作

> 模糊操作原理：
>
> * 基于离散卷积
> * 定义好每个卷积核
> * 不同卷积核得到不同的卷积效果
> * 模糊是卷积的一种表象

卷积计算原理：

![image-20210117213219621](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210117213219621.png)



## 3.1 均值模糊

* 可以用于去除

```python
import cv2 as cv
import numpy


def blur_demo(image):
    dst = cv.blur(image, (20, 1))
    cv.imshow("dst", dst)

image = cv.imread("01.jpg")
cv.namedWindow("input image", cv.WINDOW_AUTOSIZE)
cv.imshow("input image", image)
blur_demo(image)
cv.waitKey(0)
cv.destroyAllWindows()
```

![image-20210117213911982](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210117213911982.png)

## 3.2 中值模糊

* 可以用于去除`椒盐噪声`

> 原图中有许许多多的小黑点，使用中值模糊可以将小黑点给去除，这就叫`去除椒盐噪声`

```python
import cv2 as cv
import numpy

def mid_blur(image):
    dst = cv.medianBlur(image, 5)
    cv.imshow("dst", dst)

image = cv.imread("test1.png")
cv.namedWindow("input image", cv.WINDOW_AUTOSIZE)
cv.imshow("input image", image)
mid_blur(image)
cv.waitKey(0)
cv.destroyAllWindows()
```

![image-20210117214522388](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210117214522388.png)

## 3.2 自定义模糊

* 主要自定义卷积核

```python
import cv2 as cv
import numpy

def cus_blur(image):
    # 自定义初始化卷积核
    kernel = numpy.ones([17, 17], numpy.float32) / 200
    dst = cv.filter2D(image, -1, kernel=kernel)
    cv.imshow("dst", dst)

image = cv.imread("test1.png")
cv.namedWindow("input image", cv.WINDOW_AUTOSIZE)
cv.imshow("input image", image)
cus_blur(image)
cv.waitKey(0)
cv.destroyAllWindows()
```

![image-20210117215529861](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210117215529861.png)

* 还可以用于锐化（增强立体感）

```python
import cv2 as cv
import numpy

def cus_blur(image):
    kernel = numpy.array([[0, -1, 0], [-1, 5, -1], [0, -1, 0]], numpy.float32)
    dst = cv.filter2D(image, -1, kernel=kernel)
    cv.imshow("dst", dst)

image = cv.imread("01.jpg")
cv.namedWindow("input image", cv.WINDOW_AUTOSIZE)
cv.imshow("input image", image)
cus_blur(image)
cv.waitKey(0)
cv.destroyAllWindows()
```



![image-20210117215814104](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210117215814104.png)



# 4. 高斯模糊

> 二维高斯分布

![image-20210117221011483](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210117221011483.png)

> 立体高斯分布

![image-20210117221028895](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210117221028895.png)



* 代码层面

```python
import cv2 as cv
import numpy


def clamp(pv):
    if pv > 255:
        return 255
    if pv < 0:
        return 0
    return pv

def gaussion_noise(image):
    h, w, c = image.shape
    for row in range(w):
        for col in range(h):
            s = numpy.random.normal(0, 20, 3)
            B = image[row, col, 0]
            G = image[row, col, 1]
            R = image[row, col, 2]
            image[row, col, 0] = clamp(B + s[0])
            image[row, col, 1] = clamp(G + s[1])
            image[row, col, 2] = clamp(R + s[2])
    cv.imshow("noise image", image)

image = cv.imread("01.jpg")
cv.namedWindow("input image", cv.WINDOW_AUTOSIZE)
cv.imshow("input image", image)
gaussion_noise(image)
cv.waitKey(0)
cv.destroyAllWindows()
```

![image-20210117222721298](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210117222721298.png)



# 5. 边缘保留滤波

## 5.1 高斯双边

```python
def bilateralFilter(src, d, sigmaColor, sigmaSpace, dst=None, borderType=None)
```

* src表示图像
* d表示distance，可以设置为0
* sigmaColor取大一点，为了将小的噪声给模糊掉
* sigmaSpace取小一点

```python
import cv2 as cv
import numpy


def demo(image):
    dst = cv.bilateralFilter(image, 0, 100, 15)
    cv.imshow("dst",dst)

image = cv.imread("01.jpg")
cv.namedWindow("input image", cv.WINDOW_AUTOSIZE)
cv.imshow("input image", image)
demo(image)
cv.waitKey(0)
cv.destroyAllWindows()
```

![image-20210117230424756](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210117230424756.png)

## 5.2 均值迁移

```python
def pyrMeanShiftFiltering(src, sp, sr, dst=None, maxLevel=None, termcrit=None)
```

```python
import cv2 as cv
import numpy

def demo(image):
    dst = cv.pyrMeanShiftFiltering(image, 10, 50)
    cv.imshow("dst",dst)

image = cv.imread("star.jpg")
cv.namedWindow("input image", cv.WINDOW_AUTOSIZE)
cv.imshow("input image", image)
demo(image)
cv.waitKey(0)
cv.destroyAllWindows()
```

​	![image-20210117231108504](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210117231108504.png)