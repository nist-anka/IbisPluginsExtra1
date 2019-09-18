
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
    double spacing[3];
    refImage->GetSpacing( spacing );
    refImage->SetSpacing( 0.5*spacing[0], 0.5*spacing[1], 0.5*spacing[2] );
    ref->SetImage(refImage);
    ref->Export();

    return 0;
}
