
#include "newcursorplugininterface.h"
#include <QtPlugin>
#include "ibisapi.h"
#include "cursorobject.h"

NewCursorPluginInterface::NewCursorPluginInterface()
{
}

NewCursorPluginInterface::~NewCursorPluginInterface()
{
}

bool NewCursorPluginInterface::CanBeActivated()
{
    IbisAPI *ibisAPI = GetIbisAPI();
    Q_ASSERT(ibisAPI);
    if( ibisAPI->GetReferenceDataObject() )
        return true;
    return false;
}

SceneObject *NewCursorPluginInterface::CreateObject()
{
    IbisAPI *ibisAPI = GetIbisAPI();
    Q_ASSERT(ibisAPI);
    CursorObject * cursor = CursorObject::New();
    ibisAPI->AddObject( cursor );
//    cursor->SetIbisAPI( ibisAPI );
}
