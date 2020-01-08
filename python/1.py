import os
from PIL import Image


def eachFile(filepath):
    width = 28
    height = 28
    fh = open(r'D:\PycharmProjects\qt\a.txt', 'w')
    j = 0
    pathDir = os.listdir(filepath)  # 获取当前路径下的文件名，返回list
    cnt = 0
    for s in pathDir:
        newDir = os.path.join(filepath, s)  # 将文件名写入到当前文件路径后面
        NewPathDir = os.listdir(newDir)
        for t in NewPathDir:
            nDir = os.path.join(filepath, s)
            nDir2 = os.path.join(nDir, t)
            if os.path.isfile(nDir2):  # 如果是文件
                if os.path.splitext(nDir2)[1] == ".png":  # 判断是否是png
                    img = Image.open(nDir2)
                    out = img.resize((28, 28))
                    out.save(nDir2)

                    fh.write(chr(ord('0') + cnt // 55))
                    fh.write(',')
                    print(chr(ord('0') + cnt // 55))
                    # print(cnt//55)
                    print(cnt)
                    for i in range(height):
                        for j in range(width):
                            # 获取像素点颜色
                            color = img.getpixel((j, i))
                            colorsum = color[0] + color[1] + color[2]
                            if (colorsum == 0):
                                fh.write('1')
                                if i != 27 or j != 27:
                                    fh.write(',')
                            else:
                                fh.write('0')
                                if i != 27 or j != 27:
                                    fh.write(',')
                    cnt = cnt + 1
                    fh.write('\n')
                    pass
                else:
                    break

    fh.close()


def main():
    fp = r'D:\PycharmProjects\qt\qq'  # 存放数据的目录
    os.chdir(fp)
    eachFile(fp)


if __name__ == '__main__':
    main()
