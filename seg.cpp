#include <iostream>

#include <python2.7/Python.h>
#include <boost/python.hpp>
#include <vtkImageData.h>

using namespace std;
using namespace boost::python;

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

int main()
{
      Py_Initialize();
      if(!Py_IsInitialized())
      {
          std::cout <<"1" <<std::endl;
          return -1;
      }
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
      return 0;
}
