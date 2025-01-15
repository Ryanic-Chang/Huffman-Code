# README
## 文件组织结构
> 26组_数据结构与程序设计专题实验2024
│  huffman.exe // 可执行文件
│  Makefile // 编译配置文件
│  README.md // 说明文件
│  README.pdf // 说明文件
│
├─data // 存放数据
│      Graph.txt // 图片提示词
│      prompt_list.txt // 视频提示词列表
│      SourceDoc.txt // 源文档
│      Video.txt // 视频提示词
│      
├─include // 存放头文件
│      decode.h
│      encode.h 
│      frequency.h
│      fuzzy_match.h
│      generate.h
│      huffman.h
│      linked_list.h
│      match.h
│      read.h
│      verify.h
│      word_count.h
│      
├─output // 存放输出文件
│      Encode.dat
│      Encode_Word.dat
│      generated_image.jpeg
│      generated_video.mp4
│      HuffmanCode.txt
│      HuffmanCode_Word.txt
│      Statistic.txt
│      Target.txt
│      Target_Word.txt
│      text_summary.md
│      WordCount.txt
│      
├─src // 存放源文件
│      decode.c
│      decode.o
│      encode.c
│      encode.o
│      file_io.o
│      frequency.c
│      frequency.o
│      fuzzy_match.o
│      generate.c
│      generate.o
│      huffman.c
│      huffman.o
│      linked_list.c
│      linked_list.o
│      main.c
│      main.o
│      match.c
│      match.o
│      read.c
│      read.o
│      verify.c
│      verify.o
│      word_count.c
│      word_count.o
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
