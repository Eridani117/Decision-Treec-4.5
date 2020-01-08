# encoding=utf-8

import os
import numpy as np
from scipy import log
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt
import math
from sklearn.metrics import r2_score

# 字体


# 拟合函数
def func(x, a, b):
    #    y = a * log(x) + b
    y = x / (a * x + b)
    return y


# 拟合的坐标点

x0 = [28140, 21840, 17640, 13860, 10080, 6720, 3360, 2100, 1260]
#y0 = [82,  80,  78, 76, 73, 70, 66, 64, 59]
y0 = [36.06756138801575, 25.256444931030273, 18.83265995979309, 14.102001905441284, 9.232271671295166, 5.199124097824097, 2.3686649799346924, 1.3992562294006348, 0.9305112361907959]

# 拟合，可选择不同的method
result = curve_fit(func, x0, y0, method='trf')
a, b = result[0]

# 绘制拟合曲线用
x1 = np.arange(0, 28140, 0.1)
# y1 = a * log(x1) + b
y1 = x1 / (a * x1 + b)

x0 = np.array(x0)
y0 = np.array(y0)
# 计算r2
y2 = x0 / (a * x0 + b)
# y2 = a * log(x0) + b
r2 = r2_score(y0, y2)

# plt.figure(figsize=(7.5, 5))
# 坐标字体大小
plt.tick_params(labelsize=11)
# 原数据散点
plt.scatter(x0, y0, s=30, marker='o')

# 横纵坐标起止
plt.xlim((0, 28140))
plt.ylim((40, 82))

# 拟合曲线
plt.plot(x1, y1, "blue")


# True 显示网格  
# linestyle 设置线显示的类型(一共四种)  
# color 设置网格的颜色  
# linewidth 设置网格的宽度   
plt.grid(True, linestyle="--", color="g", linewidth="0.5")
plt.show()
