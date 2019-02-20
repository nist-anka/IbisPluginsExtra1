
#include "applytransformtoobjectplugininterface.h"
#include <QtPlugin>
#include <QString>
#include "applytransformtoobjectwidget.h"


#include "vtkTransform.h"
#include "vtkMatrix4x4.h"
#include "vtkSmartPointer.h"
#include "ibisapi.h"
#include "sceneobject.h"

ApplyTransformToObjectPluginInterface::ApplyTransformToObjectPluginInterface()
{
}

ApplyTransformToObjectPluginInterface::~ApplyTransformToObjectPluginInterface()
{
}

QWidget * ApplyTransformToObjectPluginInterface::CreateFloatingWidget()
{
    // Start the window
    m_interfaceWidget = new ApplyTransformToObjectWidget( 0 );
    m_interfaceWidget->setAttribute( Qt::WA_DeleteOnClose, true );
    m_interfaceWidget->setWindowTitle( "Apply Transform To Object" );

    return m_interfaceWidget;
    /*
    vtkMatrix4x4 *outMatrix = ref->GetLocalTransform()->GetMatrix();
    std::cout << "Output matrix:\n";
    std::cout << outMatrix->GetElement( 0, 0 ) << " " << outMatrix->GetElement( 0, 1 ) << "." << outMatrix->GetElement( 0, 2 ) << "." << outMatrix->GetElement( 0, 3 ) << std::endl;
    std::cout << outMatrix->GetElement( 1, 0 ) << " " << outMatrix->GetElement( 1, 1 ) << "." << outMatrix->GetElement( 1, 2 ) << "." << outMatrix->GetElement( 1, 3 ) << std::endl;
    std::cout << outMatrix->GetElement( 2, 0 ) << " " << outMatrix->GetElement( 2, 1 ) << "." << outMatrix->GetElement( 2, 2 ) << "." << outMatrix->GetElement( 2, 3 ) << std::endl;
    */
}
