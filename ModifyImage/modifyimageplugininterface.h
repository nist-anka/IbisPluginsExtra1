
#ifndef __ReplaceITKImagePluginInterface_h_
#define __ReplaceITKImagePluginInterface_h_

#include "toolplugininterface.h"


class ModifyImagePluginInterface : public ToolPluginInterface
{

    Q_OBJECT
    Q_INTERFACES(IbisPlugin)
    Q_PLUGIN_METADATA(IID "Ibis.ReplaceITKImagePluginInterface" )

public:

    vtkTypeMacro( ModifyImagePluginInterface, ToolPluginInterface );

    ModifyImagePluginInterface();
    ~ModifyImagePluginInterface();
    virtual QString GetPluginName() { return QString("ModifyImage"); }
    bool CanRun() { return true; }
    QString GetMenuEntryString() { return QString("Modify ImageObject"); }
    QWidget * CreateFloatingWidget();
};

#endif
