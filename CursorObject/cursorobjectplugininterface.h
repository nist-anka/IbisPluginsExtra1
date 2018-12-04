
#ifndef __CursorObjectPluginInterface_h_
#define __CursorObjectPluginInterface_h_

#include "objectplugininterface.h"


class CursorObjectPluginInterface : public ObjectPluginInterface
{

    Q_OBJECT
    Q_INTERFACES(IbisPlugin)
    Q_PLUGIN_METADATA(IID "Ibis.CursorObjectPluginInterface" )

public:

    vtkTypeMacro( CursorObjectPluginInterface, ObjectPluginInterface );

    CursorObjectPluginInterface();
    ~CursorObjectPluginInterface();
    virtual QString GetPluginName() { return QString("CursorObject"); }
    QString GetMenuEntryString() { return QString("Cursor Object"); }
    SceneObject * CreateObject();
    virtual bool CanBeActivated();
};

#endif
