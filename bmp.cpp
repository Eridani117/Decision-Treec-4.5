#include <stdio.h>
#include <stdlib.h>
#include <iostream>
# include<io.h>
# include<direct.h>
#include "bmp.h"

using namespace std;

#define SUM 10000

void write(int num,int id,int img[28][28])
{
    //用于存储一张手写体图片的位图信息
    char image[28 * 28];
	for(int i=0;i<784;i++)
		if(img==0)
			image[i]=0;
		else
			image[i]=255;
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
    for (int i = 0; i<256; i++)
    {
        fq[i].rgbBlue = fq[i].rgbGreen = fq[i].rgbRed = i;
    }

	
		char name[30];
		sprintf(name,"img/%d/%d.bmp",num,id);
		char dir[10];
		sprintf(dir,"img/%d",num);
		if(NULL==fopen(dir,"r"))
			_mkdir(dir);
        //创建一个BMP图片
       printf("%s\n",name);
        FILE *ftmp = fopen(name, "wb");
        if (ftmp == NULL)
            printf("Create  failure\n");
        else
        {
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

int main() 
{
    
	int img[28][28];
	write(4,32,img);
    return 0;
}