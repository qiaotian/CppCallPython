Linux 平台配置说明 （Windows类似）

Prerequisites:
1. VTK
2. boost  
`sudo apt install libboost-all-dev`

需求：在C++环境中调用python脚本，支持传入C++数据结构（vtkImageData），python脚本自定解析参数，计算结果，并重新将结果构造成C++数据结构，最后返回（vtkImageData）；

