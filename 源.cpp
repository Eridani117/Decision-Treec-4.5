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

typedef struct _data_set {
	int cnt;
	char* datas;
	char* labels;

} *DATA_SET;
DATA_SET train_set;
DATA_SET test_set;
typedef struct Tree_node {
	int Node_type; //节点类型0内部，1叶子
	int feature;
	char TYpe;
	Tree_node* t0, * t1;
} *TREE;

void AddTree(Tree_node* root, char fe, Tree_node* t) {
	if (fe == '0')
		root->t0 = t;
	else
		root->t1 = t;
}

void LoadTrainData(DATA_SET& t, int c) {
	t = (DATA_SET)malloc(sizeof(struct _data_set));
	t->cnt = c;
	t->datas = (char*)malloc(sizeof(char) * 28 * 28 * t->cnt);
	t->labels = (char*)malloc(sizeof(int) * t->cnt);
	ifstream f1("train_features.txt");
	ifstream f2("train_labels.txt");
	f1.getline(t->datas, 28 * 28 * t->cnt);
	f2.getline(t->labels, t->cnt);
}

void LoadTestData(DATA_SET& t, int c) {
	t = (DATA_SET)malloc(sizeof(struct _data_set));
	t->cnt = c;
	t->datas = (char*)malloc(sizeof(char) * 28 * 28 * t->cnt);
	t->labels = (char*)malloc(sizeof(int) * t->cnt);
	ifstream f1("test_features.txt");
	ifstream f2("test_labels.txt");
	f1.getline(t->datas, 28 * 28 * t->cnt);
	f2.getline(t->labels, t->cnt);
}
double get_class_entropy(DATA_SET data) {
	map<char, int> num;
	double info = 0.0;
	for (int i = 0; i < data->cnt; i++) {
		num[data->labels[i]]++;
	}

	for (char j = '0'; j <= '9'; j++) {
		double p = (double)num[j] / data->cnt;
		if (p > 0)
			info += -p * log2(p);
	}

	return info;
}

double get_label_entropy(DATA_SET data, int id) {
	map<char, int> num;
	double info0 = 0.0;
	double info1 = 0.0;
	int num_0 = 0, num_1 = 0;

	for (int i = 0; i < data->cnt; i++) {
		if (data->datas[i * 784 + id] == '0') {
			num[data->labels[i]]++;
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
		if (data->datas[i * 784 + id] == '1') {
			num[data->labels[i]]++;
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

double get_Gain(DATA_SET data, int feature) {
	double a = get_class_entropy(data);
	double b = get_label_entropy(data, feature);

	return a - b;
}

double get_label_spilt_entropy(DATA_SET data, int id) {
	int zero = 0, one = 0;
	int step = 784;
	double info = 0.0;
	for (int i = 0; i < data->cnt; i++) {
		if ((data->datas[id + i * step]) == '1')
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
void spilt_data(DATA_SET data, int id, DATA_SET& zero, DATA_SET& one) {
	zero = (DATA_SET)malloc(sizeof(struct _data_set));
	one = (DATA_SET)malloc(sizeof(struct _data_set));
	int size0 = 0, size1 = 0;
	for (int i = 0; i < data->cnt; i++) {
		if (data->datas[i * 784 + id] == '0')
			size0++;
		else
			size1++;
	}
	zero->cnt = size0;
	one->cnt = size1;
	int p1 = 0;
	int p0 = 0;
	zero->datas = (char*)malloc(size0 * 784);
	one->datas = (char*)malloc(size1 * 784);
	zero->labels = (char*)malloc(size0);
	one->labels = (char*)malloc(size1);

	for (int i = 0; i < data->cnt; i++) {
		if (data->datas[i * 28 * 28 + id] == '0') {
			memcpy((zero->datas) + 28 * 28 * (p0), (data->datas) + i * 28 * 28, 784);
			zero->labels[p0] = data->labels[i];
			(p0)++;
		}
		else {
			memcpy((one->datas) + 28 * 28 * (p1), (data->datas) + i * 28 * 28, 784);
			(one->labels[p1]) = (data->labels[i]);
			(p1)++;
		}
	}
}

void buildTree(TREE& root, DATA_SET data) {
	root = (TREE)malloc(sizeof(struct Tree_node));
	root->t0 = NULL;
	root->t1 = NULL;
	set<char> cnt;
	cout << data->cnt << endl;
	for (int i = 0; i < data->cnt; i++)
		cnt.insert(data->labels[i]);
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

	DATA_SET zero, one;

	spilt_data(data, max_IGR_f, zero, one);

	if (zero->cnt != 0)
		buildTree(root->t0, zero);

	if (one->cnt != 0)
		buildTree(root->t1, one);
}

char img_ident(char* img, TREE rt) {
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
	rgbq* fq = (rgbq*)malloc(256 * sizeof(rgbq));
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

	FILE* ftmp = fopen(name, "wb");
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
	int train_cnt = 368;
	int test_cnt = 182;
	LoadTrainData(train_set, train_cnt);
	LoadTestData(test_set, test_cnt);
	cout << "loads over" << endl;
	TREE t;
	buildTree(t, train_set);
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
