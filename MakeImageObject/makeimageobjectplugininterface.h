
#ifndef __MakeImageObjectPluginInterface_h_
#define __MakeImageObjectPluginInterface_h_

#include "toolplugininterface.h"


class MakeImageObjectPluginInterface : public ToolPluginInterface
{

    Q_OBJECT
    Q_INTERFACES(IbisPlugin)
    Q_PLUGIN_METADATA(IID "Ibis.MakeImageObjectPluginInterface" )

public:

    vtkTypeMacro( MakeImageObjectPluginInterface, ToolPluginInterface );

    MakeImageObjectPluginInterface();
    ~MakeImageObjectPluginInterface();
    virtual QString GetPluginName() { return QString("MakeImageObject"); }
    bool CanRun() { return true; }
    QString GetMenuEntryString() { return QString("Make an ImageObject"); }
    QWidget * CreateFloatingWidget();
};

#endif
