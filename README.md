Linux 平台配置说明 （Windows类似）

任务需求：
- 在C++环境中调用python影像分割脚本；
- 支持传入C++数据结构（vtkImageData），python脚本自定解析参数，计算结果，并重新将结果构造成C++数据结构，最后返回（vtkImageData）；
- 示例传入空白三维影像，经过处理，返回三维影像中绘制有8个圆球；

Prerequisites:
1. VTK
2. cmake (3.10 or newer, don't use ccmake)
3. boost (1.61.1)
`sudo apt install libboost-all-dev`

运行脚本：
1. 检查并更新CMakeLists.txt文
    1. 库文件名和版本号与本机安装是否一致
    2. 更新VTK_DIR为本机安装位置
2. 更新seg.cpp中自定义python脚本路径
3. 检查python脚本，注意python脚本必须全部为ASCII码字符，否则需要转码
4. 进入工程根目录，并执行  
`mkdir build && cd build && cmake .. && make && ./seg`

警告：  
It may raise an error when cmake project that 'FindBoost.cmake uable to find the requested boost libraries including boost_thread boost_serialization boost_python'. The reason is attached as follows:  
- `The naming scheme of the Boost libraries changed with version 1.66.0. An additional architecture infix has been added which is not known to FindBoost.cmake of the CMake version 3.10 you are using. Try to use version 1.65.1 and set BOOST_ROOT to the path of your Boost installation.`  
- Boost 1.63 requires CMake 3.7 or newer.  
Boost 1.64 requires CMake 3.8 or newer.  
Boost 1.65 and 1.65.1 require CMake 3.9.3 or newer.  
Boost 1.66 requires CMake 3.11 or newer.  
Boost 1.67 requires CMake 3.12 or newer.  
Boost 1.68 is only support by the current master  
- The version of cmake bundled with ccmake is `not` compatable with Boost version

so you can not find boost libraries. To solve this you should only use `cmake` command rather than `ccmake gui`. 

