#include <iostream>

//#include <python2.7/Python.h>
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

#include <vtkImageData.h>
#include <vtkDICOMImageReader.h>
#include <vtkNIFTIImageWriter.h>
#include <vtkDataSetMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkActor.h>
#include <vtkRenderWindowInteractor.h>

//using namespace std;

//// first construct input and output 
int process(vtkImageData* input, vtkImageData* output, std::string script_path) {
    namespace p = boost::python;
    namespace np = boost::python::numpy;

    // 初始化python解释器
    Py_Initialize();
    if(!Py_IsInitialized()) {
        std::cout <<"python initialized failed" <<std::endl;
        return -1;
    }

    PyRun_SimpleString("import sys");

    // 添加自定义脚本路径，否则无法导入自定义模块
    PyRun_SimpleString( (std::string("sys.path.append(\"") + script_path + std::string("\")")).c_str() );
    //PyRun_SimpleString( "print(sys.path)" );
    // 导入python分割模块
    p::object pmodule;
    try {
        pmodule = p::import("seg_script");
    } catch (p::error_already_set) {
        PyErr_Print();
    }

    // 导入python函数（主函数）
    p::object seg_func = pmodule.attr("seg_volume");

    // 初始化numpy
    np::initialize();
    // 生成ndarray类实例
    int dims[3] = {0, 0, 0};
    double spacing[3] = {0., 0., 0.};
    double origin[3] = {0., 0., 0.};
    output->GetDimensions(dims);
    output->GetSpacing(spacing);
    output->GetOrigin(origin);
    std::cout << dims[0] << " " << dims[1] << " " << dims[2] << std::endl;
    // 
    //uint8_t data_in_c[] = {1,2,3,4,5,6,7,8,1,3,5,7}; // use output buffer instead
    uint8_t* data_in_c = static_cast<uint8_t*>(output->GetScalarPointer(0,0,0));
    double spacing_in_c[3] = {spacing[0], spacing[1], spacing[2]};
    double origin_in_c[3] = {origin[0], origin[1], origin[2]};

    //std::cout << spacing_in_c[0] << " " << spacing_in_c[1] << " " << spacing_in_c[2] << std::endl;
    //std::cout << origin_in_c[0] << " " << origin_in_c[1] << " " << origin_in_c[2] << std::endl;

    p::tuple data_shape = p::make_tuple(dims[0], dims[1], dims[2]);
    p::tuple data_stride = p::make_tuple(dims[1], dims[2], sizeof(uint8_t));
    p::tuple spacing_shape = p::make_tuple(3);
    p::tuple spacing_stride = p::make_tuple(sizeof(double));
    p::tuple origin_shape = p::make_tuple(3);
    p::tuple origin_stride = p::make_tuple(sizeof(double));

    np::ndarray data_from_c = np::from_data(data_in_c, np::dtype::get_builtin<uint8_t>(), data_shape, data_stride, p::object());
    np::ndarray spacing_from_c = np::from_data(spacing_in_c, np::dtype::get_builtin<double>(), spacing_shape, spacing_stride, p::object());
    np::ndarray origin_from_c = np::from_data(origin_in_c, np::dtype::get_builtin<double>(), origin_shape, origin_stride, p::object());

    // 打印参数
    //std::cout << "C++ ndarray:" << std::endl;
    //std::cout << p::extract<char const *>(p::str(data_from_c)) << std::endl;
    //std::cout << p::extract<char const *>(p::str(spacing_from_c)) << std::endl;
    //std::cout << p::extract<char const *>(p::str(origin_from_c)) << std::endl;
    //std::cout << std::endl;

    // 执行分割
    p::object data_obj_from_python = seg_func(data_from_c, origin_from_c, spacing_from_c);
    np::ndarray data = np::array(data_obj_from_python);
    uint8_t *pp = reinterpret_cast<uint8_t*> (data.get_data()); //data.get_data()得到的数据指针是char *类型，需要转换为对应的数据类型

    // 打印返回值
    std::cout << "data from python:" << std::endl;
    std::cout << p::extract<char const *>(p::str(data)) << std::endl;
    std::cout << std::endl;

    std::cout << "pointer:" << std::endl;
    for (int i = 0; i < 9; i++) {
    	std::cout << *(pp+i)  << std::endl;
    }

    //Py_DECREF(seg_func);
    //Py_Finalize(); // 段错误
    return 0;
}

int main()
{
    // 读取测试体数据
    std::string folder = "/home/qt/CppCallPython/test_dicom_volume";
    std::string script_path = "/home/qt/CppCallPython/script/";
    vtkDICOMImageReader* reader = vtkDICOMImageReader::New();
    reader->SetDirectoryName(folder.c_str());
    reader->Update();
    vtkImageData* raw = reader->GetOutput();
    
    // 更新参数并打印（for testing）
    int dims[3] = {0, 0, 0};
    double spacing[3] = {0, 0, 0};
    double origin[3] = {0, 0, 0};
    raw->GetDimensions(dims);
    raw->GetSpacing(spacing);
    raw->GetOrigin(origin);

    // 构造空的分割数据并初始化
    vtkImageData* seg = vtkImageData::New();
    seg->SetDimensions(raw->GetDimensions());
    seg->SetSpacing(raw->GetSpacing());
    seg->SetOrigin(raw->GetOrigin());
    seg->AllocateScalars(VTK_UNSIGNED_CHAR, 1);
    uint8_t *ptr = (uint8_t *)seg->GetScalarPointer();
    for(int i=0; i<dims[0]*dims[1]*dims[2]; i++) {
        *ptr++ = 100;
    }

    // 分割数据
    process(raw, seg, script_path);

    // 显示 segmented data for validation
    vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
    mapper->SetInputData(raw);
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);
    renderer->AddActor(actor);
    renderer->ResetCamera();

    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);
    renderWindow->Render();
    renderWindowInteractor->Start();

    return 0;
}
