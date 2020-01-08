#include "bmp.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <direct.h>
#include <fstream>
#include <iostream>
#include <map>
#include <set>

int train_cnt = 0;
int test_cnt = 0;
using namespace std;
extern "C" {
typedef struct _data_set {
  int cnt;
  char *datas;
  char *labels;

} * DATA_SET;
typedef struct _sub_data_set {
  int cnt;
  int *num;
} * SUB_DATA_SET;
DATA_SET train_set;
DATA_SET test_set;
SUB_DATA_SET SUB_TRAIN_SET;
typedef struct Tree_node {
  int Node_type; //节点类型0内部，1叶子
  int feature;
  char TYpe;
  Tree_node *t0, *t1;
} * TREE;

void AddTree(Tree_node *root, char fe, Tree_node *t) {
  if (fe == '0')
    root->t0 = t;
  else
    root->t1 = t;
}

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

double get_Gain(SUB_DATA_SET data, int feature) {
  double a = get_class_entropy(data);
  double b = get_label_entropy(data, feature);

  return a - b;
}

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

int ccc(int c) {
  train_cnt = c;
  test_cnt = 42000 - train_cnt;
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
    if (ans == (test_set->labels[i]))
      yes++;
  }

  return (float)yes * 100 / all;
}
}