from ctypes import *
import cv2
import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score
import time
import os


def spl(slp):
    d = pd.read_csv('train.csv', sep=',', header=None, low_memory=False)
    data = d.values
    imgs = data[::, 1::]
    labels = data[::, 0]
    imgs = np.delete(imgs, 0, 0)
    labels = np.delete(labels, 0, 0)

    for i in range(42000):
        for j in range(784):
            if imgs[i][j] >= '125':
                imgs[i][j] = '1'
            else:
                imgs[i][j] = '0'

    train_features, test_features, train_labels, test_labels = train_test_split(imgs, labels, test_size=slp,
                                                                                random_state=0)

    if (os.path.exists("trf.txt")):
        os.remove("trf.txt")

    if (os.path.exists("trf.txt")):
        os.remove("tef.txt")
    if (os.path.exists("trf.txt")):
        os.remove("trl.txt")
    if (os.path.exists("trf.txt")):
        os.remove("tel.txt")
    if (os.path.exists("trf.txt")):
        os.remove("train_labels.txt")
    if (os.path.exists("trf.txt")):
        os.remove("test_labels.txt")
    if (os.path.exists("trf.txt")):
        os.remove("test_features.txt")
    if (os.path.exists("trf.txt")):
        os.remove("train_features.txt")

    np.savetxt('trf.txt', train_features, delimiter='', fmt='%s')
    np.savetxt('tef.txt', test_features, delimiter='', fmt='%s')
    np.savetxt('trl.txt', train_labels, delimiter='', fmt='%s')
    np.savetxt('tel.txt', test_labels, delimiter='', fmt='%s')

    fh = open(r'train_features.txt', 'w')
    for line in open(r'trf.txt', 'r'):  # 打开文件
        rs = line.replace('\n', '')  # 替换换行符
        fh.write(rs)  # 显示替换后的行
    fh.close()
    fh = open(r'train_labels.txt', 'w')
    for line in open(r'trl.txt', 'r'):  # 打开文件
        rs = line.replace('\n', '')  # 替换换行符
        fh.write(rs)  # 显示替换后的行
    fh.close()
    fh = open(r'test_features.txt', 'w')
    for line in open(r'tef.txt', 'r'):  # 打开文件
        rs = line.replace('\n', '')  # 替换换行符
        fh.write(rs)  # 显示替换后的行
    fh.close()
    fh = open(r'test_labels.txt', 'w')
    for line in open(r'tel.txt', 'r'):  # 打开文件
        rs = line.replace('\n', '')  # 替换换行符
        fh.write(rs)  # 显示替换后的行
    fh.close()
    return train_labels.shape[0]


def main():
    a = list()
    a.append(0.33)
    a.append(0.48)
    a.append(0.58)
    a.append(0.67)
    a.append(0.76)
    a.append(0.84)
    a.append(0.92)
    a.append(0.95)
    a.append(0.97)
    a.append(0.98)
    a.append(0.99)
    result = cdll.LoadLibrary("b.so")
    p = list()
    t = list()
    c = list()
    for i in a:
        for j in range(5):
            cnt = spl(i)
            c.append(cnt)
            time_1 = time.time()
            p.append(result.ccc(cnt))
            time_2 = time.time()
            t.append(time_2 - time_1)

    fh = open(r'zzzzzzzz.txt', 'w')
    fh.write(p.__str__())
    fh.write("\n")
    fh.write(t.__str__())
    fh.write("\n")
    fh.write(c.__str__())
    print(p)
    print(t)
    print(c)
    fh.close()


if __name__ == "__main__":
    main()
