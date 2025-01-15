# README
## 说明
1. 程序使用到硅基流动所提供的api key，因为避免产生费用问题，会在2025.01.06后删除该api key（位于src/generate.c中）。
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
> make
## 运行方法
> huffman
