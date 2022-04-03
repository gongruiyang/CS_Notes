# 1. 图像直方图（histogram）

## 1.1 绘制图片直方图

```python
import cv2 as cv
import numpy
from matplotlib import pyplot


def plot_demo(image):
    pyplot.hist(image.ravel(), 256, [0, 256])
    pyplot.show()


image = cv.imread("01.jpg")
cv.namedWindow("image", cv.WINDOW_AUTOSIZE)
cv.imshow("image", image)
plot_demo(image)
cv.waitKey(0)
cv.destroyAllWindows()
```

![image-20210118132127971](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210118132127971.png)





## 1.2 RGB三通道的直方图

```python
import cv2 as cv
import numpy
from matplotlib import pyplot

def image_hist(image):
    color = ('blue', 'green', 'red')
    for i, color in  enumerate(color):
        hist = cv.calcHist([image], [i], None, [256], [0,256])
        pyplot.plot(hist, color=color)
        pyplot.xlim([0, 256])
    pyplot.show()

image = cv.imread("01.jpg")
cv.namedWindow("image", cv.WINDOW_AUTOSIZE)
cv.imshow("image", image)
image_hist(image)
cv.waitKey(0)
cv.destroyAllWindows()
```



![image-20210118132656511](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210118132656511.png)

## 1.3 直方图应用1

```python
import cv2 as cv
import numpy
from matplotlib import pyplot

def equalHist_demo(image):
    gray = cv.cvtColor(image, cv.COLOR_BGR2GRAY)
    dst = cv.equalizeHist(gray)
    cv.imshow("equalHist_demo", dst)hg

image = cv.imread("01.jpg")
cv.namedWindow("image", cv.WINDOW_AUTOSIZE)
cv.imshow("image", image)
equalHist_demo(image)
cv.waitKey(0)
cv.destroyAllWindows()
```

![image-20210118134056271](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210118134056271.png)

## 1.4 直方图应用2

```python
import cv2 as cv
import numpy
from matplotlib import pyplot


def Clahe_demo(image):
    gray = cv.cvtColor(image, cv.COLOR_BGR2GRAY)
    clahe = cv.createCLAHE(clipLimit=5.0, tileGridSize=(8, 8))
    dst = clahe.apply(gray)
    cv.imshow("Clahe_demo", dst)

image = cv.imread("01.jpg")
cv.namedWindow("image", cv.WINDOW_AUTOSIZE)
cv.imshow("image", image)
Clahe_demo(image)
cv.waitKey(0)
cv.destroyAllWindows()
```





![image-20210118144201824](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210118144201824.png)



# 2. 直方图反向投影

## 2.1 黑白2D直方图

```python
import cv2 as cv
import numpy



def hist2d_demo(image):
    hsv = cv.cvtColor(image, cv.COLOR_BGR2HSV)
    hist = cv.calcHist([image], [0, 1], None, [100, 250], [0, 100, 0, 256])
    cv.imshow("hist2D", hist)


image = cv.imread("01.jpg")
cv.namedWindow("input image", cv.WINDOW_AUTOSIZE)
cv.imshow("input image", image)
hist2d_demo(image)
cv.waitKey(0)
cv.destroyAllWindows()
```

![image-20210118200147040](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210118200147040.png)

## 2.2 彩色2D直方图

```python
import cv2 as cv
import numpy
from matplotlib import pyplot


def hist2d_demo(image):
    hsv = cv.cvtColor(image, cv.COLOR_BGR2HSV)
    hist = cv.calcHist([image], [0, 1], None, [100, 250], [0, 100, 0, 256])
    pyplot.imshow(hist, interpolation='nearest')
    pyplot.title("2D Histogram")
    pyplot.show()


image = cv.imread("01.jpg")
cv.namedWindow("input image", cv.WINDOW_AUTOSIZE)
cv.imshow("input image", image)
hist2d_demo(image)
cv.waitKey(0)
cv.destroyAllWindows()
```

![image-20210118200434066](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210118200434066.png)

## 2.3 直方图反向衍射

```python
import cv2 as cv
import numpy
from matplotlib import pyplot

def back_projection_demo():
    target = cv.imread("01.jpg")
    sample = cv.imread("sample.jpg")
    sam_hsv = cv.cvtColor(sample, cv.COLOR_BGR2HSV)
    tar_hsv = cv.cvtColor(target, cv.COLOR_BGR2HSV)

    # 展示图片
    cv.imshow("target", target)
    cv.imshow("sample", sample)

    sam_hist = cv.calcHist([sam_hsv], [0, 1], None, [36, 48], [0, 100, 0, 256])
    cv.normalize(sam_hsv, sam_hsv, 0, 255, cv.NORM_MINMAX)
    dst = cv.calcBackProject([tar_hsv], [0, 1], sam_hist, [0, 100, 0, 256], 1)
    cv.imshow("BackProjectionDemo", dst)

back_projection_demo()
cv.waitKey(0)
cv.destroyAllWindows()
```

![image-20210118203207405](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210118203207405.png)



