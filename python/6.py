import numpy as np
import pandas as pd
from pandas import DataFrame

# 显示所有列
from sklearn.model_selection import train_test_split

d = pd.read_csv('a.txt', sep=',', header=None)
data = d.values
imgs = data[::, 1::]
labels = data[::, 0]
train_features, test_features, train_labels, test_labels = train_test_split(imgs, labels, test_size=0.33,
                                                                            random_state=0)
np.savetxt('train_features.txt', train_features, delimiter='', fmt='%s')
np.savetxt('test_features.txt', test_features, delimiter='', fmt='%s')
np.savetxt('train_labels.txt', train_labels, delimiter='', fmt='%s')
np.savetxt('test_labels.txt', test_labels, delimiter='', fmt='%s')
