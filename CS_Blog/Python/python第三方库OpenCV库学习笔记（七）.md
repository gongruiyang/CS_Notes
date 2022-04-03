# 1. 图像梯度

## 1.1 Soble算子

* 求x,y方向图像梯度

```python
import cv2 as cv

def sobel_demo(image):
    grad_x = cv.Sobel(image, cv.CV_32F, 1, 0)	# 求x方向的梯度，对x求一阶导数
    grad_y = cv.Sobel(image, cv.CV_32F, 0, 1)	# 求x方向的梯度，对y求一阶导数
    gradx = cv.convertScaleAbs(grad_x)
    grady = cv.convertScaleAbs(grad_y)
    cv.imshow("grandient-x", gradx)
    cv.imshow("grandient-y", grady)

image = cv.resize(cv.imread("01.jpg"), (300,400))
cv.imshow("image", image)
sobel_demo(image)
cv.waitKey(0)
cv.destroyAllWindows()
```

![image-20210122103815831](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210122103815831.png)

* 求整体梯度

```python
import cv2 as cv

def sobel_demo(image):
    grad_x = cv.Sobel(image, cv.CV_32F, 1, 0)
    grad_y = cv.Sobel(image, cv.CV_32F, 0, 1)
    gradx = cv.convertScaleAbs(grad_x)
    grady = cv.convertScaleAbs(grad_y)
    gradxy = cv.addWeighted(gradx, 0.5, grady, 0.5, 0)
    cv.imshow("gradxy",gradxy)

image = cv.resize(cv.imread("01.jpg"), (300,400))
cv.imshow("image", image)
sobel_demo(image)
cv.waitKey(0)
cv.destroyAllWindows()
```

![image-20210122104459240](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210122104459240.png)

## 1.2 拉普拉斯算子

* 拉普拉斯算子

```python
import cv2 as cv

def lapalian_demo(image):
    dst = cv.Laplacian(image, cv.CV_32F)
    lpls = cv.convertScaleAbs(dst)
    cv.imshow("lapalian_demo", lpls)

image = cv.resize(cv.imread("01.jpg"), (300,400))
cv.imshow("image", image)
lapalian_demo(image)
cv.waitKey(0)
cv.destroyAllWindows()
```



![image-20210122104832539](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210122104832539.png)

* 自定义拉普拉斯算子

```python
import cv2 as cv
import numpy

def lapalian_demo(image):
    kernel = numpy.array([[0, 1, 0], [1, -4, 1], [0, 1, 0]])
    lpls = cv.filter2D(image, cv.CV_32F, kernel=kernel)
    cv.imshow("lapalian_demo", lpls)

image = cv.resize(cv.imread("01.jpg"), (300,400))
cv.imshow("image", image)
lapalian_demo(image)
cv.waitKey(0)
cv.destroyAllWindows()
```

![image-20210122105156402](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210122105156402.png)

# 2. Canny边缘提取

## 2.1 Canny算法介绍

* `Canny`是边缘检测算法，是1986年提出的

> 步骤：对彩色图像进行高斯模糊，去除噪声，再进行灰度转换，对图像求取梯度，根据图像角度实现非最大信号压制，进行高低阈值过滤
>
> 1. 高斯模糊：GaussianBlur
> 2. 灰度转换：cvtColor
> 3. 计算梯度：Sobel/Scharr
> 4. 非最大信号抑制
> 5. 高低阈值输出二值图像

## 2.2 代码演示

```python
import cv2 as cv
import numpy

def edge_demo(image):
    blurred = cv.GaussianBlur(image, (3, 3), 0)
    gray = cv.cvtColor(blurred, cv.COLOR_BGR2GRAY)
    # x gradient
    gradx = cv.Sobel(gray, cv.CV_16SC1, 1, 0)
    grady = cv.Sobel(gray, cv.CV_16SC1, 0, 1)
    # edge 高阈值为150 低阈值为50
    edge_output = cv.Canny(gradx, grady, 50, 150)
    cv.imshow("Canny Edge", edge_output)

    dst = cv.bitwise_and(image, image, mask=edge_output)
    cv.imshow("Color Edge", dst)

image = cv.resize(cv.imread("01.jpg"), (300,400))
cv.imshow("image", image)
edge_demo(image)
cv.waitKey(0)
cv.destroyAllWindows()
```

![image-20210122111401813](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210122111401813.png)



# 3. 直线检测

## 3.1 霍夫直线变换介绍

* Hough Line Transform用来做直线检测
* 前提条件：边缘检测已经完成
* 需要理解：平面空间到极坐标空间转换

## 3.2 代码演示

```python
import cv2 as cv
import numpy

def line_detection(image):
    gray = cv.cvtColor(image, cv.COLOR_BGR2GRAY)
    edges = cv.Canny(gray, 50, 150, apertureSize=3)
    lines = cv.HoughLines(edges, 1, numpy.pi/180, 200)
    for line in lines:
        rho, theta = line[0]
        a = numpy.cos(theta)
        b = numpy.sin(theta)
        x0 = a * rho
        y0 = b * rho
        x1 = int(x0 + 1000 * (-b))
        y1 = int(y0 + 1000 * (a))
        x2 = int(x0 - 1000 * (-b))
        y2 = int(y0 - 1000 * (a))
        cv.line(image, (x1, y1), (x2, y2), (0, 0, 255), 2)
    cv.imshow("image lines", image)


image = cv.resize(cv.imread("star.jpg"), (300,400))
cv.imshow("image", image)
line_detection(image)
cv.waitKey(0)
cv.destroyAllWindows()
```

![image-20210122113528509](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210122113528509.png)

