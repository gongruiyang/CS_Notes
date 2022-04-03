# 打开一张图片

```python
# 1.导入cv2模块
import cv2 as cv
# 2.使用imread()函数读入图片
src = cv.imread('01.jpg')
# 3.使用imshow()函数展示图片
cv.imshow("image", src)
# 4.使用waitKey()暂停程序
cv.waitKey(0)		# 单位是 milliseconds. 0 is the special value that means "forever".
# 5.销毁创建窗口
cv.destroyAllWindows()
```

![image-20210113215410701](C:\Users\1111\AppData\Roaming\Typora\typora-user-images\image-20210113215410701.png)

# 打印图片属性

```python
# 1.导入cv2模块
import cv2 as cv


def get_image_info(image):
    print(type(image))
    print(image.shape)
    print(image.size)
    print(image.dtype)


    # 使用imread()函数读入图片
    src = cv.imread('01.jpg')
    get_image_info(src)
```

```python
<class 'numpy.ndarray'>
(640, 640, 3)
1228800
uint8
```

# 打开摄像头

```python
import cv2 as cv


def open_camera():
    capture = cv.VideoCapture(0)
    while(True):
        ret,frame = capture.read()
        cv.imshow("camera",frame)
        c = cv.waitKey(50)
        if c == 27:
            break


open_camera()
```

