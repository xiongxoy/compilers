If you find the following lines alian to you, please mail me at
xiong.xoy@gmail.com

本项目采用C语言完成，只用到标准的C语言库
(另外在文本处理的时候用了少许的python）

在项目的目录结构如下
--
  |lex                  #词法分析部分 
  |compilercompiler     #LR（1）分析表生成部分
  |include              #公用文件头和公用库部分
  |syntax_and_semantics #语法和语义部分
    
在syntax_and_semantics目录里面，有simpile1.c文件，这个文件即为本编译器的输入文件

本项目采用Makefile构建，由于Makefile写的不太完善，需要一定的手动操作
可以首先尝试
cd <根目录>
cd syntax_and_semantics
make syntax_test
./syntax_test               #运行编译程序

如果有问题，则推荐的操作顺序如下
cd <根目录>
cd include
make
cd ../compilercompiler
make
cd ../lex
make
cd ../syntax_and_semantics
make syntax_test
./syntax_test

