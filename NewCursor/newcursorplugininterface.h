
#ifndef __NewCursorPluginInterface_h_
#define __NewCursorPluginInterface_h_

#include "objectplugininterface.h"


class NewCursorPluginInterface : public ObjectPluginInterface
{

    Q_OBJECT
    Q_INTERFACES(IbisPlugin)
    Q_PLUGIN_METADATA(IID "Ibis.NewCursorPluginInterface" )

public:

    vtkTypeMacro( NewCursorPluginInterface, ObjectPluginInterface );

    NewCursorPluginInterface();
    ~NewCursorPluginInterface();
    virtual QString GetPluginName() override { return QString("NewCursor"); }
    QString GetMenuEntryString() override { return QString("Cursor Object"); }
    SceneObject * CreateObject() override;
    virtual bool CanBeActivated() override;
};

#endif
