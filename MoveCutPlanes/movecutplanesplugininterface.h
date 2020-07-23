
#ifndef __MoveCutPlanesPluginInterface_h_
#define __MoveCutPlanesPluginInterface_h_

#include "toolplugininterface.h"


class MoveCutPlanesPluginInterface : public ToolPluginInterface
{

    Q_OBJECT
    Q_INTERFACES(IbisPlugin)
    Q_PLUGIN_METADATA(IID "Ibis.ReplaceITKImagePluginInterface" )

public:

    vtkTypeMacro( MoveCutPlanesPluginInterface, ToolPluginInterface );

    MoveCutPlanesPluginInterface();
    ~MoveCutPlanesPluginInterface();
    virtual QString GetPluginName() override { return QString("ModifyImage"); }
    bool CanRun() override { return true; }
    QString GetMenuEntryString() override { return QString("Modify ImageObject"); }
    QWidget * CreateFloatingWidget() override;
};

#endif //__MoveCutPlanesPluginInterface_h_
