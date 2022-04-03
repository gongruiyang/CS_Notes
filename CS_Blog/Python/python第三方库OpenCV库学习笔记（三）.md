# 1. inRange函数实现HSV图片物体追踪

* 将图像转化为HSV后通过inRange函数可以定位颜色块

```python
def inRange(src, lowerb, upperb, dst=None)
```

* src表示图片
* lowerb是一个数组，用于存放`要追踪的颜色`hmin、smin、vmin三个值组成的数组
* upperb是一个数组，用于存放`要追踪的颜色`hmax、smax、vmax三个值组成的数组

* 返回值：返回二值化图像



> 示例：提取图像中绿色颜色

```python
import cv2 as cv
import numpy


def extrace_object_demo():
    capture = cv.VideoCapture("")   # 填入路径
    while(True):
        ret, frame = capture.read() # 读取视频的每一帧图片放进frame中
        if ret == False:    # ret是False表示视频读取完毕
            break;
        hsv = cv.cvtColor(frame, cv.COLOR_RGB2HSV) # 将每一帧变成HSV图片
        # 将要过滤出来的绿色的hsv三个值的最高值和最低值分别放入数组中
        lower_green_hsv = numpy.array([35, 43, 46])
        upper_green_hsv = numpy.array([77, 255, 255])
        # 返回二值化图像，其中绿色的物体全部变成白色，其他颜色全部变成黑色
        mask = cv.inRange(hsv, lower_green_hsv, upper_green_hsv)
        cv.imshow("track green", mask)   # 展示图片
        c = cv.waitKey(40)
        if c == 27:     # 27代表esc(escape)
            break;
```

![image-20210115230856626](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210115230856626.png)

> 将视频**每一帧的图片中的绿色全部提取**出来，在左边的**二值化图像**中用**白色**表示，其他颜色全部变成黑色



# 2. 通道分离与合并

* 使用`split函数`将图片的三个`通道分离`出来

```python
import cv2 as cv
import numpy

image = cv.imread("01.jpg")
B, G, R = cv.split(image)	# 将图片分解成RGB三个通道
cv.imshow("image", image)
cv.imshow("R", R)
cv.imshow("G", G)
cv.imshow("B", B)
cv.waitKey(0)
cv.destroyAllWindows()
```

![image-20210115231540252](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210115231540252.png)



* 使用merge函数将三个通道合并

```python
import cv2 as cv
import numpy

image = cv.imread("01.jpg")
B, G, R = cv.split(image)
cv.imshow("image", image)
change_image = cv.merge([B, G, R])	# 将RGB三个通道合并
cv.imshow("change_image", change_image)
cv.waitKey(0)
cv.destroyAllWindows()
```

![image-20210115232042989](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210115232042989.png)



# 3. 像素运算

## 3.1 算术运算

```python
import cv2 as cv
import numpy

# 像素 加 运算
def add_demo(image1, image2):
    final_image = cv.add(image1, image2)
    cv.imshow("add", final_image)

# 像素 减 运算
def subtract_demo(image1, image2):
    final_image = cv.subtract(image1, image2)
    cv.imshow("subtract", final_image)

# 像素 乘 运算
def divide_demo(image1, image2):
    final_image = cv.divide(image1, image2)
    cv.imshow("divide", final_image)

# 像素 除 运算
def multiply_demo(image1, image2):
    final_image = cv.multiply(image1, image2)
    cv.imshow("multiply", final_image)

image1 = cv.imread("01.jpg")
add_demo(image1, image1)
subtract_demo(image1, image1)
divide_demo(image1, image1)
multiply_demo(image1, image1)
cv.waitKey(0)
cv.destroyAllWindows()
```

![image-20210115235004906](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210115235004906.png)



## 3.2 逻辑运算

```python
import cv2 as cv
import numpy


def logic_demo(image1, image2):
    dst = cv.bitwise_and(image1, image2)	# 与运算
    cv.imshow("and", dst)
    dst = cv.bitwise_not(image1, image2)	# 非运算
    cv.imshow("not", dst)
    dst = cv.bitwise_or(image1, image2)		# 或运算
    cv.imshow("or", dst)
    dst = cv.bitwise_xor(image1, image2)	# 异或运算
    cv.imshow("xor", dst)



image = cv.imread("01.jpg")
logic_demo(image, image)
cv.waitKey(0)
cv.destroyAllWindows()
```

![image-20210117192236592](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210117192236592.png)

# 4. addWeighted函数讲解

```python
def addWeighted(src1, alpha, src2, beta, gamma, dst=None, dtype=None)
```

* 该函数可以将两张图片混合
* src1和src2是待混合的两张图片
* alpha和beta分别是两张图片混合权重
* gamma是图1与图2混合后添加的数值。不要太大，不然图片一片白。总和等于255以上就是纯白色了

> 样例

```python
import cv2 as cv
import numpy

# 读取图片并将图片尺寸修改一致
src1 = cv.imread("star.jpg")
src2 = cv.imread("hourse.jpg")
h, w, c = src2.shape
src1 = cv.resize(src1, (w, h))

# 混合图片
dst = cv.addWeighted(src1, 0.5, src2, 0.5, 5)

# 展示图片
cv.imshow("src1", src1)
cv.imshow("src2", src2)
cv.imshow("dst", dst)

cv.waitKey(0)
cv.destroyAllWindows()
```

![image-20210117201320123](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210117201320123.png)

# 5. 调整对比度与亮度

```python
import cv2 as cv
import numpy

def contract_brightness_demo(image, c, b):
    h, w, ch = image.shape
    blank = numpy.zeros([h, w, ch], image.dtype)
    dst = cv.addWeighted(image, c, blank, 1-c, b)
    cv.imshow("change", dst)

image = cv.imread("01.jpg")
cv.imshow("image",image)
contract_brightness_demo(image, 1.2, 10)
cv.waitKey(0)
cv.destroyAllWindows()
```

![image-20210117201839795](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210117201839795.png)