
#include "replaceitkimageplugininterface.h"
#include <QtPlugin>
#include <QFileDialog>
#include <QString>


#include "vtkTransform.h"
#include "vtkMatrix4x4.h"
#include "vtkImageData.h"
#include "vtkSmartPointer.h"
#include "ibisapi.h"
#include "imageobject.h"
#include "ibisitkvtkconverter.h"
#include "itkImageFileReader.h"

ReplaceITKImagePluginInterface::ReplaceITKImagePluginInterface()
{
}

ReplaceITKImagePluginInterface::~ReplaceITKImagePluginInterface()
{
}

QWidget * ReplaceITKImagePluginInterface::CreateFloatingWidget()
{
    IbisAPI *ibisAPI = GetIbisAPI();
    Q_ASSERT(ibisAPI);
    QString sceneDir = ibisAPI->GetSceneDirectory();
    ImageObject *ref = ibisAPI->GetReferenceDataObject();
    if( ! ref )
        return 0;
    IbisItkFloat3ImageType::Pointer refITKimg = ref->GetItkImage();
    IbisItkFloat3ImageType::DirectionType dir = refITKimg->GetDirection();
    IbisItkFloat3ImageType::PointType origin = refITKimg->GetOrigin();
    IbisItkFloat3ImageType::SpacingType spacing = refITKimg->GetSpacing();

    QString filename = QFileDialog::getOpenFileName( 0, tr("Replacement"), sceneDir, tr("*.mnc") );
    typedef itk::ImageFileReader< IbisItkFloat3ImageType > ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(filename.toUtf8().data());

    try
    {
        reader->Update();
    }
    catch( itk::ExceptionObject & err )
    {
        return 0;
    }

    vtkMatrix4x4 *refMatrix = ref->GetLocalTransform()->GetMatrix();
    std::cout << "Initial matrix:\n";
    std::cout << refMatrix->GetElement( 0, 0 ) << " " << refMatrix->GetElement( 0, 1 ) << "." << refMatrix->GetElement( 0, 2 ) << "." << refMatrix->GetElement( 0, 3 ) << std::endl;
    std::cout << refMatrix->GetElement( 1, 0 ) << " " << refMatrix->GetElement( 1, 1 ) << "." << refMatrix->GetElement( 1, 2 ) << "." << refMatrix->GetElement( 1, 3 ) << std::endl;
    std::cout << refMatrix->GetElement( 2, 0 ) << " " << refMatrix->GetElement( 2, 1 ) << "." << refMatrix->GetElement( 2, 2 ) << "." << refMatrix->GetElement( 2, 3 ) << std::endl;
    IbisItkFloat3ImageType::Pointer itkImage = reader->GetOutput();
#if 0
    vtkImageData * refImage = ref->GetImage();
    int * dimensions = refImage->GetDimensions();
    const long unsigned int numberOfPixels =  dimensions[0] * dimensions[1] * dimensions[2];
    IbisItkFloat3ImageType::Pointer refItkImage = ref->GetItkImage();
    float *refitkImageBuffer = refItkImage->GetBufferPointer();
    memcpy(refitkImageBuffer, itkImage->GetBufferPointer(), numberOfPixels*sizeof(float));
    refItkImage->Update();
#else
    itkImage->SetDirection( dir );
    itkImage->SetOrigin( origin );
    itkImage->SetSpacing( spacing );
    ref->SetItkImage( itkImage );
#endif
    ref->ObjectModified();
    vtkMatrix4x4 *outMatrix = ref->GetLocalTransform()->GetMatrix();
    std::cout << "Output matrix:\n";
    std::cout << outMatrix->GetElement( 0, 0 ) << " " << outMatrix->GetElement( 0, 1 ) << "." << outMatrix->GetElement( 0, 2 ) << "." << outMatrix->GetElement( 0, 3 ) << std::endl;
    std::cout << outMatrix->GetElement( 1, 0 ) << " " << outMatrix->GetElement( 1, 1 ) << "." << outMatrix->GetElement( 1, 2 ) << "." << outMatrix->GetElement( 1, 3 ) << std::endl;
    std::cout << outMatrix->GetElement( 2, 0 ) << " " << outMatrix->GetElement( 2, 1 ) << "." << outMatrix->GetElement( 2, 2 ) << "." << outMatrix->GetElement( 2, 3 ) << std::endl;

    return 0;
}
