# README
本项目为XJTU-2024年数据结构与程序设计专题实验课程的作业，主要实现以下功能。
## 功能
1. Huffman编码-单字符与汉字整体
2. Huffman解码-单字符与汉字整体
3. Huffman编码-单词与汉字整体
4. Huffman解码-单词与汉字整体
5. 单词统计
6. 字符与汉字统计
7. 字符串查找
8. 字符串模糊匹配
9. ai文生图
10. ai文生视频
11. ai多轮对话
## 说明
1. 程序使用到硅基流动所提供的api key，现已删除失效，可自行替换。
2. 程序使用到curl库以及json-c库，通过MSYS2进行安装，安装命令如下。
    a) curl
    pacman -Sy mingw-w64-x86_64-curl
    b) json-c
    pacman -Sy mingw-w64-x86_64-json-c  
3. 程序在进行ai多轮对话功能中，由于cmd字符编码问题，对于中文输出可能会输出乱码，可以采用以下命令进行调整。
调整成GBK
chcp 936
调整成UTF-8
chcp 65001
## 编译方法
> cd {path}
> make
## 运行方法
> huffman
