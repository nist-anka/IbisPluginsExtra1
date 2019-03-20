
#include "transformoperationsplugininterface.h"
#include <QtPlugin>
#include <QString>
#include "transformoperationswidget.h"

TransformOperationsPluginInterface::TransformOperationsPluginInterface()
{
}

TransformOperationsPluginInterface::~TransformOperationsPluginInterface()
{
}

QWidget * TransformOperationsPluginInterface::CreateFloatingWidget()
{
    // Open the widget
    m_interfaceWidget = new TransformOperationsWidget( 0 );
    m_interfaceWidget->setAttribute( Qt::WA_DeleteOnClose, true );
    m_interfaceWidget->setWindowTitle( "Apply Transform To Object" );
    m_interfaceWidget->SetInterface( this );
    return m_interfaceWidget;
}
