
#include "hideobjectsplugininterface.h"
#include <QtPlugin>
#include <QFileDialog>
#include <QString>
#include <QList>

#include "userobjects.h"
#include "ibisapi.h"

HideObjectsPluginInterface::HideObjectsPluginInterface()
{
}

HideObjectsPluginInterface::~HideObjectsPluginInterface()
{
}

QWidget * HideObjectsPluginInterface::CreateFloatingWidget()
{
    m_interfaceWidget = new UserObjects( 0 );
    m_interfaceWidget->setAttribute( Qt::WA_DeleteOnClose, true );
    m_interfaceWidget->setWindowTitle( "User Objects" );
    m_interfaceWidget->SetIbisAPI( this->GetIbisAPI() );
    connect( this->GetIbisAPI(), SIGNAL(ObjectAdded(int)), m_interfaceWidget, SLOT(UpdateUI()) );
    connect( this->GetIbisAPI(), SIGNAL(ObjectRemoved(int)), m_interfaceWidget, SLOT(UpdateUI()) );
    return m_interfaceWidget;
}
