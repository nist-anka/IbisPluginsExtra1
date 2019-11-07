
#include "modifyimageplugininterface.h"
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

ModifyImagePluginInterface::ModifyImagePluginInterface()
{
}

ModifyImagePluginInterface::~ModifyImagePluginInterface()
{
}

QWidget * ModifyImagePluginInterface::CreateFloatingWidget()
{
    IbisAPI *ibisAPI = GetIbisAPI();
    Q_ASSERT(ibisAPI);
    ImageObject *ref = ibisAPI->GetReferenceDataObject();
    if( ! ref )
        return 0;
    vtkImageData * refImage = ref->GetImage();
    double step[3];
    refImage->GetSpacing( step );
    refImage->SetSpacing( 0.5*step[0], 0.5*step[1], 0.5*step[2] );
//    ref->SetImage(refImage);
    IbisItkFloat3ImageType::Pointer refITKimg = ref->GetItkImage();
    IbisItkFloat3ImageType::SpacingType spacing = refITKimg->GetSpacing();
    refITKimg->SetSpacing( step );
    refITKimg->Update();
    ref->ObjectModified();
    ref->Export();

    return 0;
}
