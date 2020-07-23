
#ifndef __HideObjectsPluginInterface_h_
#define __HideObjectsPluginInterface_h_

#include "toolplugininterface.h"
class UserObjects;

class HideObjectsPluginInterface : public ToolPluginInterface
{

    Q_OBJECT
    Q_INTERFACES(IbisPlugin)
    Q_PLUGIN_METADATA(IID "Ibis.HideObjectsPluginInterface" )

public:

    vtkTypeMacro( HideObjectsPluginInterface, ToolPluginInterface );

    HideObjectsPluginInterface();
    ~HideObjectsPluginInterface();
    virtual QString GetPluginName() override { return QString("UserObjects"); }
    bool CanRun() override { return true; }
    QString GetMenuEntryString() override { return QString("User Objects"); }
    QWidget * CreateFloatingWidget() override;
private:
    UserObjects * m_interfaceWidget;
};

#endif
