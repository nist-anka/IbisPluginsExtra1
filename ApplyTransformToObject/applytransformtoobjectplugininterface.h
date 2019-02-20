
#ifndef __ApplyTransformToObjectPluginInterface_h_
#define __ApplyTransformToObjectPluginInterface_h_

#include "toolplugininterface.h"


class ApplyTransformToObjectPluginInterface : public ToolPluginInterface
{

    Q_OBJECT
    Q_INTERFACES(IbisPlugin)
    Q_PLUGIN_METADATA(IID "Ibis.ApplyTransformToObjectPluginInterface" )

public:

    vtkTypeMacro( ApplyTransformToObjectPluginInterface, ToolPluginInterface );

    ApplyTransformToObjectPluginInterface();
    ~ApplyTransformToObjectPluginInterface();
    virtual QString GetPluginName() { return QString("ApplyTransformToObject"); }
    bool CanRun() { return true; }
    QString GetMenuEntryString() { return QString("Apply Transform to Object"); }
    QWidget * CreateFloatingWidget();
};

#endif
