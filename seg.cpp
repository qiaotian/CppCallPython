#include <iostream>

//#include <python2.7/Python.h>
#include <boost/python.hpp>
#include <boost/python/numpy.hap>

#include <vtkImageData.h>

using namespace std;
//using namespace boost::python;

namespace p = boost::python;
namespace np = boost::python::numpy;

struct ImageData {
    //int height = 0;
    //int width = 0;
    double spacing[3];
    double origin[3];
    int dims[3];
    int scalar_size;
    void* data = NULL;
} ImageData;

ImageData* parse_vtk_image_data(vtkImageData* input) {
    double dims[3];
    input->GetDimensions(dims);

    ImageData* output = new ImageData();
    input->GetDimensions(output->dims);
    input->GetOrigin(output->origin);
    input->GetSpacing(output->spacing);
    output->scalar_size = input->GetScalarSize();
    //output->data = input->GetScalarPointer(0, 0, 0);
    output->data = new float[dims[0]*dims[1]*dims[2]];

    return output;
}

typedef boost::shared_ptr<vtkImageData> shared_image_ptr;

BOOST_PYTHON_MODULE(hello)
{
    class_<vtkImageData>("vtkImageData")
    . def("", &vtkImageData::)
    . def("", &vtkImageData::)
    ;
    register_ptr_to_python<shared_image_ptr>();
}

void process(vtkImageData* input, vtkImageData* output) {
    // 初始化python解释器
    Py_Initialize();
    if(!Py_IsInitialized()) {
        std::cout <<"1" <<std::endl;
        return -1;
    }

    // 导入python分割模块
    p::object module = p::import("seg_script");
    // 导入python函数（主函数）
    p::object segfunc = module.attr("seg_volume");

    // 初始化numpy
    np::initialize();
    // 生成ndarray类实例
    uint8_t data_in_c[] = {1,2,3,4,5,6,7,8,1,3,5,7};
    p::tuple shape = p::make_tuple(3, 4);
    p::tuple stride = p::make_tuple(4, 1);


    PyRun_SimpleString("import sys");
    int argc = 1;
    char *argv[1];
    argv[0] = "/home/hunkyu/xxxxx/test.py";
    PySys_SetArgv(argc, argv);

    if(PyRun_SimpleString("execfile('/home/hunkyu/xxxxx/test.py')") == NULL)
    {
        std::cout <<"2" <<std::endl;
        return -1;
    }
    Py_Finalize();
}

// TODO
void save(vtkImageData* volume) {
    //vtkNIFTIImageWriter* writer = vtkNIFTIImageWriter::New();
    //writer->
}

int main()
{
    // 读取测试体数据
    string folder = "/home/qt/CppCallPython/test_dicom_volume";
    vtkDIDOMImageReader* reader = vtkDIDOMImageReader::New();
    reader->SetDirectoryName(folder.c_str());
    reader->update();
    vtkImageData* raw = reader->GetOutput();

    // 构造空的分割数据并初始化
    vtkImageData* seg = vtkImageData::New();
    seg->SetDimensions(512, 512, 100);
    seg->AllocateScalars(VTK_UNSIGNED_CHAR, 1);
    unsigned char *ptr = (unsigned char*)seg->GetScalarPointer();
    for(int i=0; i<512*512*100; i++) {
	    //*ptr ++ =i%256;
        *ptr++ = 1;
    }

    // 
    process(raw, seg);

    // 保存raw和seg成nifti格式，方便检查运行结果
    save(raw);
    save(seg);

    return 0;
}
