
#include "cursorobjectplugininterface.h"
#include <QtPlugin>
#include "ibisapi.h"
#include "cursorobject.h"

CursorObjectPluginInterface::CursorObjectPluginInterface()
{
}

CursorObjectPluginInterface::~CursorObjectPluginInterface()
{
}

bool CursorObjectPluginInterface::CanBeActivated()
{
    IbisAPI *ibisAPI = GetIbisAPI();
    Q_ASSERT(ibisAPI);
    if( ibisAPI->GetReferenceDataObject() )
        return true;
    return false;
}

SceneObject *CursorObjectPluginInterface::CreateObject()
{
    IbisAPI *ibisAPI = GetIbisAPI();
    Q_ASSERT(ibisAPI);
    CursorObject * cursor = CursorObject::New();
    cursor->SetIbisAPI( ibisAPI );
}
