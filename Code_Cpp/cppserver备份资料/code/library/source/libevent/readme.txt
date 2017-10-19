win编译方法:
1 使用vs工具控制台
2 到libevent目录下执行
nmake /f Makefile.nmake

linux使用时需要增加库
rt（<time.h>中clock_gettime()等函数）