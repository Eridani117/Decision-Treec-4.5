import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split

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

train_features, test_features, train_labels, test_labels = train_test_split(imgs, labels, test_size=0.33,
                                                                            random_state=0)
np.savetxt('train_features.txt', train_features, delimiter='', fmt='%s')
np.savetxt('test_features.txt', test_features, delimiter='', fmt='%s')
np.savetxt('train_labels.txt', train_labels, delimiter='', fmt='%s')
np.savetxt('test_labels.txt', test_labels, delimiter='', fmt='%s')


fh = open(r'C:\Users\Phoenix\Desktop\数据集\test_labels.txt', 'w')
for line in open(r'D:\PycharmProjects\qt\test_labels.txt', 'r'):  # 打开文件
    rs = line.replace('\n', '')  # 替换换行符
    fh.write(rs)  # 显示替换后的行
fh.close()
fh = open(r'C:\Users\Phoenix\Desktop\数据集\test_features.txt', 'w')
for line in open(r'D:\PycharmProjects\qt\test_features.txt', 'r'):  # 打开文件
    rs = line.replace('\n', '')  # 替换换行符
    fh.write(rs)  # 显示替换后的行
fh.close()
fh = open(r'C:\Users\Phoenix\Desktop\数据集\train_features.txt', 'w')
for line in open(r'D:\PycharmProjects\qt\train_features.txt', 'r'):  # 打开文件
    rs = line.replace('\n', '')  # 替换换行符
    fh.write(rs)  # 显示替换后的行
fh.close()
fh = open(r'C:\Users\Phoenix\Desktop\数据集\train_labels.txt', 'w')
for line in open(r'D:\PycharmProjects\qt\train_labels.txt', 'r'):  # 打开文件
    rs = line.replace('\n', '')  # 替换换行符
    fh.write(rs)  # 显示替换后的行
fh.close()