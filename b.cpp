#include "bmp.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <direct.h>
#include <fstream>
#include <iostream>
#include <map>
#include <set>

using namespace std;
// 训练集数据
// 不间断的01
// 本程序每张图由28*28个元素组成
typedef struct _data_set {
  int cnt;
  char *datas;
  char *labels;

} * DATA_SET;
// 子数据集
// 只记录下该子数据集中每张图在训练集中的序号
typedef struct _sub_data_set {
  int cnt;
  int *num;
} * SUB_DATA_SET;
DATA_SET train_set;
DATA_SET test_set;
SUB_DATA_SET SUB_TRAIN_SET;
// 树节点
typedef struct Tree_node {
  int Node_type; //节点类型0内部，1叶子
  int feature;
  char TYpe;
  Tree_node *t0, *t1;
} * TREE;

// 树节点插入子节点
void AddTree(Tree_node *root, char fe, Tree_node *t) {
  if (fe == '0')
    root->t0 = t;
  else
    root->t1 = t;
}

// 装载数据集，同时把第一个子数据集装载好
void LoadTrainData(DATA_SET &t, int c) {
  t = (DATA_SET)malloc(sizeof(struct _data_set));
  t->cnt = c;
  t->datas = (char *)malloc(sizeof(char) * 28 * 28 * t->cnt);
  t->labels = (char *)malloc(sizeof(int) * t->cnt);
  ifstream f1("train_features.txt");
  ifstream f2("train_labels.txt");
  f1.getline(t->datas, 28 * 28 * t->cnt);
  f2.getline(t->labels, t->cnt);
  SUB_TRAIN_SET = (SUB_DATA_SET)malloc(sizeof(struct _sub_data_set));
  SUB_TRAIN_SET->cnt = c;
  SUB_TRAIN_SET->num = (int *)malloc(sizeof(int) * c);
  for (int i = 0; i < c; i++) {
    SUB_TRAIN_SET->num[i] = i;
  }
}

// 装载训练数据集
void LoadTestData(DATA_SET &t, int c) {
  t = (DATA_SET)malloc(sizeof(struct _data_set));
  t->cnt = c;
  t->datas = (char *)malloc(sizeof(char) * 28 * 28 * t->cnt);
  t->labels = (char *)malloc(sizeof(int) * t->cnt);
  ifstream f1("test_features.txt");
  ifstream f2("test_labels.txt");
  f1.getline(t->datas, 28 * 28 * t->cnt);
  f2.getline(t->labels, t->cnt);
}

// 计算类别信息熵
double get_class_entropy(SUB_DATA_SET data) {
  map<char, int> num;
  double info = 0.0;
  for (int i = 0; i < data->cnt; i++) {
    num[train_set->labels[data->num[i]]]++;
  }

  for (char j = '0'; j <= '9'; j++) {
    double p = (double)num[j] / data->cnt;
    if (p > 0)
      info += -p * log2(p);
  }

  return info;
}

// 计算标签信息熵
double get_label_entropy(SUB_DATA_SET data, int id) {
  map<char, int> num;
  double info0 = 0.0;
  double info1 = 0.0;
  int num_0 = 0, num_1 = 0;

  for (int i = 0; i < data->cnt; i++) {
    if (train_set->datas[data->num[i] * 784 + id] == '0') {
      num[train_set->labels[data->num[i]]]++;
      num_0++;
    }
  }

  for (int j = '0'; j <= '9'; j++) {
    double p = (double)num[j] / data->cnt;
    if (p > 0)
      info0 += -p * log2(p);
  }

  num.clear();

  for (int i = 0; i < data->cnt; i++) {
    if (train_set->datas[data->num[i] * 784 + id] == '1') {
      num[train_set->labels[data->num[i]]]++;
      num_1++;
    }
  }

  for (int j = '0'; j <= '9'; j++) {
    double p = (double)num[j] / data->cnt;
    if (p > 0)
      info1 += -p * log2(p);
  }

  return (double)num_0 / data->cnt * info0 + (double)num_1 / data->cnt * info1;
}

// 计算信息增益率
double get_Gain(SUB_DATA_SET data, int feature) {
  double a = get_class_entropy(data);
  double b = get_label_entropy(data, feature);

  return a - b;
}

// 计算标签分裂信息熵
double get_label_spilt_entropy(SUB_DATA_SET data, int id) {
  int zero = 0, one = 0;
  int step = 784;
  double info = 0.0;
  for (int i = 0; i < data->cnt; i++) {
    if (train_set->datas[id + data->num[i] * step] == '1')
      one++;
    else
      zero++;
  }

  double p0 = (double)(zero) / data->cnt;
  double p1 = (double)(one) / data->cnt;

  if (p0 > 0)
    info += -p0 * log2(p0);
  if (p1 > 0)
    info += -p1 * log2(p1);

  return info;
}

// 将数据集按照树节点所选的标签分为两个
void spilt_data(SUB_DATA_SET data, int id, SUB_DATA_SET &zero,
                SUB_DATA_SET &one) {
  zero = (SUB_DATA_SET)malloc(sizeof(struct _sub_data_set));
  one = (SUB_DATA_SET)malloc(sizeof(struct _sub_data_set));
  int size0 = 0, size1 = 0;
  for (int i = 0; i < data->cnt; i++) {
    if (train_set->datas[data->num[i] * 784 + id] == '0')
      size0++;
    else
      size1++;
  }
  zero->cnt = size0;
  one->cnt = size1;
  int p1 = 0;
  int p0 = 0;
  zero->num = (int *)malloc(sizeof(int) * size0);
  one->num = (int *)malloc(sizeof(int) * size1);

  for (int i = 0; i < data->cnt; i++) {
    if (train_set->datas[data->num[i] * 784 + id] == '0') {
      zero->num[p0] = data->num[i];
      (p0)++;
    } else {
      one->num[p1] = data->num[i];
      (p1)++;
    }
  }
}

// 建立决策树
void buildTree(TREE &root, SUB_DATA_SET data) {
  root = (TREE)malloc(sizeof(struct Tree_node));
  root->t0 = NULL;
  root->t1 = NULL;
  set<char> cnt;
  //cout << data->cnt << endl;
  for (int i = 0; i < data->cnt; i++)
    cnt.insert(train_set->labels[data->num[i]]);
  cnt.erase('\0');

  if (cnt.size() == 1) {
    root->Node_type = 1;
    root->TYpe = *cnt.begin();
    root->feature = -1;
    return;
  }

  root->Node_type = 0;
  int max_IGR_f = 0;
  double max_IGR = -99999999;

  for (int i = 0; i < 784; i++) {

    double gain = get_Gain(data, i);

    double sp = get_label_spilt_entropy(data, i);

    if (sp != 0) {
      if ((gain / sp) > max_IGR) {
        max_IGR_f = i;
        max_IGR = (gain / sp);
      }
    }
  }

  root->feature = max_IGR_f;

  SUB_DATA_SET zero, one;

  spilt_data(data, max_IGR_f, zero, one);

  if (zero->cnt != 0)
    buildTree(root->t0, zero);

  if (one->cnt != 0)
    buildTree(root->t1, one);
}

// 图像预测
char img_ident(char *img, TREE rt) {
  if (rt == NULL)
    return '?';
  if (rt->Node_type == 1)
    return rt->TYpe;
  //
  int ident = rt->feature;
  if (img[ident] == '0')
    return img_ident(img, rt->t0);
  else {
    return img_ident(img, rt->t1);
  }
}

// 导出到图片
void write(int num, int id, char img[]) {
  //用于存储一张手写体图片的位图信息
  char image[28 * 28];
  for (int i = 0; i < 28; i++)
    for (int j = 0; j < 28; j++) {
      if (img[i * 28 + j] == '0')
        image[(27 - i) * 28 + j] = 0;
      else
        image[(27 - i) * 28 + j] = 255;
    }

  //用于存储产生的图片的名字

  //读取字符集的幻数、数量、宽、高

  fileHeader fh;
  fileInfo fi;
  //编写信息头fi
  fi.biSize = 40;
  fi.biWidth = 28;
  fi.biHeight = 28;
  fi.biPlanes = 1;
  fi.biBitCount = 8;
  fi.biCompression = 0;
  fi.biSizeImage = 28 * 28;
  fi.biXPixPerMeter = 0;
  fi.biYPixPerMeter = 0;
  fi.biClrUsed = 0;
  fi.biClrImporant = 0;

  //编写文件头fh
  fh.bfType[0] = 0x42;
  fh.bfType[1] = 0x4D;
  fh.bfOffBits = sizeof(fileHeader) + sizeof(fileInfo) + 256 * sizeof(rgbq);
  fh.bfSize = fh.bfOffBits + fi.biSizeImage;
  fh.bfReserved1 = 0;
  fh.bfReserved2 = 0;

  //创建调色板
  rgbq *fq = (rgbq *)malloc(256 * sizeof(rgbq));
  for (int i = 0; i < 256; i++) {
    fq[i].rgbBlue = fq[i].rgbGreen = fq[i].rgbRed = i;
  }

  char name[30];
  sprintf(name, "img/%d/%d.bmp", num, id);
  char dir[10];
  sprintf(dir, "img/%d", num);
  if (NULL == fopen(dir, "r"))
    _mkdir(dir);
  //创建一个BMP图片

  FILE *ftmp = fopen(name, "wb");
  if (ftmp == NULL)
    printf("Create  failure\n");
  else {
    //读取一张图片的位图信息
    // fread(image, sizeof(char), 28 * 28, fp1);
    //写文件头
    fwrite(&fh, sizeof(fileHeader), 1, ftmp);
    //写信息头
    fwrite(&fi, sizeof(fileInfo), 1, ftmp);
    //写调色板
    fwrite(fq, sizeof(rgbq), 256, ftmp);
    //写位图信息
    fwrite(image, fi.biSizeImage, 1, ftmp);
    fclose(ftmp);
  }
}

int main() {
  int train_cnt = 28140;
  int test_cnt = 13860;
  LoadTrainData(train_set, train_cnt);
  LoadTestData(test_set, test_cnt);
  cout << "loads over" << endl;
  TREE t;
  buildTree(t, SUB_TRAIN_SET);
  cout << "bulid over" << endl;
  int all = 0, yes = 0;
  for (int i = 0; i < test_cnt; i++) {
    all++;
    char img[784];
    for (int j = 0; j < 784; j++) {
      img[j] = test_set->datas[i * 784 + j];
    }
    char ans = img_ident(img, t);
    write(ans, i, img);

    if (ans == (test_set->labels[i]))
      yes++;
  }
  printf("rate :%.2f\n", (float)yes * 100 / all);

  system("pause");
}
