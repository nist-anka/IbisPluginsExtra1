
#include "makeimageobjectplugininterface.h"
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
#include "opendatafiledialog.h"

MakeImageObjectPluginInterface::MakeImageObjectPluginInterface()
{
}

MakeImageObjectPluginInterface::~MakeImageObjectPluginInterface()
{
}

QWidget * MakeImageObjectPluginInterface::CreateFloatingWidget()
{
    IbisAPI *ibisAPI = GetIbisAPI();
    Q_ASSERT(ibisAPI);
    QString sceneDir = ibisAPI->GetSceneDirectory();
    ImageObject *newImage = ImageObject::New();

    QString filename = QFileDialog::getOpenFileName( 0, tr("Open file"), sceneDir, tr("*.mnc") );
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

    QFileInfo fi(filename );
    IbisItkFloat3ImageType::Pointer itkImage = reader->GetOutput();
    IbisItkVtkConverter *itktovtkConverter = IbisItkVtkConverter::New();
    vtkImageData *vtkImage = itktovtkConverter->ConvertItkImageToVtkImage( itkImage );
    newImage->SetImage( vtkImage );
    ibisAPI->AddObject( newImage );
    newImage->SetName( fi.baseName() );

    return 0;
}
