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

using namespace std;
//using namespace boost::python;

namespace p = boost::python;
namespace np = boost::python::numpy;

/*
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

typedef boost::shared_ptr<vtkImageData> shared_image_ptr;*/

bool process(vtkImageData* input, vtkImageData* output) {
    // 初始化python解释器
    Py_Initialize();
    if(!Py_IsInitialized()) {
        std::cout <<"python initialized failed" <<std::endl;
        return false;
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
    cout << string("sys.path.append(\"") + script_path + string("\")") << endl;
    // 添加自定义脚本路径，否则无法导入
    PyRun_SimpleString( (string("sys.path.append(\"") + script_path + string("\")")).c_str() );
    
    int argc = 1;
    char *argv[1];
    argv[0] = "/home/hunkyu/xxxxx/test.py";
    PySys_SetArgv(argc, argv);

    if(PyRun_SimpleString("execfile('/home/hunkyu/xxxxx/test.py')") == NULL)
    {
        std::cout <<"script running failed" <<std::endl;
        return false;
    }
    Py_Finalize();
}

// TODO
void save(vtkImageData* volume, string opath) {
    vtkNIFTIImageWriter* writer = vtkNIFTIImageWriter::New();
    writer->SetInputData(volume);
    writer->SetFileName(opath.c_str());
}

int dims[3] = {0, 0, 0};
double spacing[3] = {0, 0, 0};
double origin[3] = {0, 0, 0};

int main()
{
    // 读取测试体数据
    string folder = "/home/qt/CppCallPython/test_dicom_volume";
    vtkDICOMImageReader* reader = vtkDICOMImageReader::New();
    reader->SetDirectoryName(folder.c_str());
    reader->Update();
    vtkImageData* raw = reader->GetOutput();
    
    // 更新参数并打印
    raw->GetDimensions(dims);
    raw->GetSpacing(spacing);
    raw->GetOrigin(origin);

    // 构造空的分割数据并初始化
    vtkImageData* seg = vtkImageData::New();
    seg->SetDimensions(raw->GetDimensions());
    seg->SetSpacing(raw->GetSpacing());
    seg->SetOrigin(raw->GetOrigin());
    seg->AllocateScalars(VTK_UNSIGNED_CHAR, 1);
    unsigned char *ptr = (unsigned char*)seg->GetScalarPointer();
    for(int i=0; i<dims[0]*dims[1]*dims[2]; i++) {
	    //*ptr ++ =i%256;
        *ptr++ = 100;
    }

    // 分割数据
    //process(raw, seg);

    // display segmented data for validation
    vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
    mapper->SetInputData(seg);
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



    // 保存raw和seg成nifti格式，方便检查运行结果
    //save(raw);
    //save(seg);

    return 0;
}
