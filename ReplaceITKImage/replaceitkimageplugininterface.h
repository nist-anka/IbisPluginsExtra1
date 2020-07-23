
#ifndef __ReplaceITKImagePluginInterface_h_
#define __ReplaceITKImagePluginInterface_h_

#include "toolplugininterface.h"


class ReplaceITKImagePluginInterface : public ToolPluginInterface
{

    Q_OBJECT
    Q_INTERFACES(IbisPlugin)
    Q_PLUGIN_METADATA(IID "Ibis.ReplaceITKImagePluginInterface" )

public:

    vtkTypeMacro( ReplaceITKImagePluginInterface, ToolPluginInterface );

    ReplaceITKImagePluginInterface();
    ~ReplaceITKImagePluginInterface();
    virtual QString GetPluginName() override { return QString("ReplaceITKImage"); }
    bool CanRun() override { return true; }
    QString GetMenuEntryString() override { return QString("Replace ITK Image"); }
    QWidget * CreateFloatingWidget() override;
};

#endif
