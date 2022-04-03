# 1. 图像二值化

## 1.1 二值图像（Binary Image）

（1）二值图像的概念

* 一张图像只有`黑白`两个颜色
* 下面图像是就是一个`二值图像`，其中的`1`代表`黑色`，其中的`0`代表`白`色

![image-20210119202036294](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210119202036294.png)

（2）二值图像的存储

* 使用`四叉树`存储平面图像

![image-20210119202440422](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210119202440422.png)

## 1.2 图像二值化方法

> 图像二值化的方法：
>
> 1. 全局阈值
> 2. 局部阈值

![image-20210119202749666](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210119202749666.png)

## 1.3 OpenCV相关API

### 1.3.1 OTSU

```python
import cv2 as cv
import numpy

def threshold_demo(image):
    gray = cv.cvtColor(image, cv.COLOR_BGR2GRAY)
    ret, binary = cv.threshold(gray, 0, 255, cv.THRESH_BINARY | cv.THRESH_OTSU)
    print("threshold value %s" % ret)
    cv.imshow("Binary IMage", binary)

image = cv.resize(cv.imread("01.jpg"), (300,400))
cv.imshow("image", image)
threshold_demo(image)
cv.waitKey(0)
cv.destroyAllWindows()
```

```python
threshold value 171.0
```

![image-20210119203521740](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210119203521740.png)



### 1.3.2 Triangle

```python
import cv2 as cv
import numpy


def threshold_demo(image):
    gray = cv.cvtColor(image, cv.COLOR_BGR2GRAY)
    ret, binary = cv.threshold(gray, 0, 255, cv.THRESH_BINARY | cv.THRESH_TRIANGLE)
    print("threshold value %s" % ret)
    cv.imshow("Binary IMage", binary)


image = cv.resize(cv.imread("01.jpg"), (300,400))
cv.imshow("image", image)
threshold_demo(image)
cv.waitKey(0)
cv.destroyAllWindows()
```

```python
threshold value 189.0
```

![image-20210119203757152](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210119203757152.png)

### 1.3.3 自动与手动

```python
import cv2 as cv
import numpy


def threshold_demo(image):
    gray = cv.cvtColor(image, cv.COLOR_BGR2GRAY)
    ret, binary = cv.threshold(gray, 150, 255, cv.THRESH_BINARY)
    print("threshold value %s" % ret)
    cv.imshow("Binary IMage", binary)


image = cv.resize(cv.imread("01.jpg"), (300,400))
cv.imshow("image", image)
threshold_demo(image)
cv.waitKey(0)
cv.destroyAllWindows()
```

```python
threshold value 150.0
```

![image-20210119203859288](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210119203859288.png)



### 1.3.4 局部阈值

* 使用`ADAPTIVE_THRESH_MEAN_C`

```python
import cv2 as cv
import numpy

def local_threshold_demo(image):
    gray = cv.cvtColor(image, cv.COLOR_BGR2GRAY)
    binary = cv.adaptiveThreshold(gray, 255, cv.ADAPTIVE_THRESH_MEAN_C, cv.THRESH_BINARY, 25, 10)
    cv.imshow("Binary IMage", binary)

image = cv.resize(cv.imread("01.jpg"), (300,400))
cv.imshow("image", image)
local_threshold_demo(image)
cv.waitKey(0)
cv.destroyAllWindows()
```



![image-20210119204543876](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210119204543876.png)

* 使用`ADAPTIVE_THRESH_GAUSSIAN_C`

```python
import cv2 as cv
import numpy

def local_threshold_demo(image):
    gray = cv.cvtColor(image, cv.COLOR_BGR2GRAY)
    binary = cv.adaptiveThreshold(gray, 255, cv.ADAPTIVE_THRESH_GAUSSIAN_C, cv.THRESH_BINARY, 25, 10)
    cv.imshow("Binary IMage", binary)

image = cv.resize(cv.imread("01.jpg"), (300,400))
cv.imshow("image", image)
local_threshold_demo(image)
cv.waitKey(0)
cv.destroyAllWindows()
```



![image-20210119204749350](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210119204749350.png)

# 2. 图像金字塔

## 图像金字塔原理

* 图像金字塔是图像多尺度表达的一种，是一种以多分辨率来解释图像的有效但概念简单的结构。
* 一幅图像的金字塔是一系列以金字塔形状排列的分辨率逐步降低，且来源于同一张原始图的图像集合。其通过梯次向下采样获得，直到达到某个终止条件才停止采样。
* 我们将一层一层的图像比喻成金字塔，层级越高，则图像越小，分辨率越低。

![image-20210119211953881](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210119211953881.png)

## 高斯金字塔和拉普拉斯金字塔

* Reduce：先对图像进行高斯模糊，再取出所有行和列中的偶数行和偶数列进行降采样

![image-20210119211746496](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210119211746496.png)

* Expand：先对图像进行扩大，再对扩大的图像进行卷积

 ```python
import cv2 as cv


def pyramid_demo(image):
    level = 3
    temp = image.copy()
    pyramid_images = []
    for i in range(level):
        dst = cv.pyrDown(temp)
        pyramid_images.append(dst)
        cv.imshow("pyramid_down_" + str(i), dst)
        temp = dst.copy()
    return pyramid_images


def ladpalian_demo(image):
    pyramid_images = pyramid_demo(image)
    level = len(pyramid_images)
    for i in range(level - 1, -1, -1):
        if (i - 1) < 0:
            expand = cv.pyrUp(pyramid_images[i], dstsize=image.shape[:2])
            lpls = cv.subtract(image, expand)
            cv.imshow("laplalian_down_" + str(i), lpls)
        else:
            expand = cv.pyrUp(pyramid_images[i], dstsize=pyramid_images[i - 1].shape[:2])
            lpls = cv.subtract(pyramid_images[i - 1], expand)
            cv.imshow("laplalian_down_" + str(i), lpls)

image = cv.imread("01.jpg")
pyramid_demo(image)
cv.waitKey(0)
cv.destroyAllWindows()
 ```

* pyramid_demo

![image-20210119214059717](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210119214059717.png)

* ladpalian_demo

![image-20210119214233845](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210119214233845.png)