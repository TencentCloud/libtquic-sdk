# libtquic-sdk

跨平台通用QUIC网络库
![image](https://github.com/TencentCloud/libtquic-sdk/blob/main/doc/image/tquic_arch.png)


#### 支持版本
Q043,Q046,Q050,draft_29,RFC_V1


#### 如何在linux下编译使用
需要用到lib/linux_x64/libtquic.so, include/tnet_quic_request.h，demo/tquic_demo_bin.cc

clang编译命令(假设头文件、lib、cc代码均在同一目录下)
clang++ tquic_demo_bin.cc -I./ -ltquic -L./ -L/lib64/ -o tquic_demo_bin -std=c++11

g++编译命令
g++ tquic_demo_bin.cc -I./ -ltquic -L./ -L/lib64/ -o tquic_demo_bin -std=c++11


#### 如何嵌入android工程
1. C++调用，将android动态库libquic.so 及 头文件 tnet_quic_request.h放入工程
2. java调用，将android动态库libtquic_jni.so 及 android/java下的代码导入工程


#### 如何嵌入iOS工程
将lib/ios下解压后的Tquic.xcframework导入工程即可


#### API及使用
API：参考include/tnet_quic_request.h
1. C++使用，参见tquic_demo_bin.cc
2. Android使用，参见demo/android 下Example代码及apk
3. iOS使用，参见demo/iOS;src/ios/;
备注：所有java、oc接口均可自己灵活定制或重新，底层最终都是一套简单的C++接口


#### 功能宣讲
参见：https://gmtc.infoq.cn/2021/shenzhen/presentation/4148
