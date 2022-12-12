import os
import sys

currentPath = os.getcwd().replace('\\','/')    # 获取当前路径
files = os.listdir(currentPath)
print('files', files)
for filename in files:
    portion = os.path.splitext(filename)
    # 如果后缀是.txt
    if portion[1] == ".txt":
        # 重新组合文件名和后缀名
        newname = portion[0] + ".cpp"
        os.rename(filename, newname)
