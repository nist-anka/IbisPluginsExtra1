
#ifndef __TransformOperationsPluginInterface_h_
#define __TransformOperationsPluginInterface_h_

#include "toolplugininterface.h"

class TransformOperationsWidget;

class TransformOperationsPluginInterface : public ToolPluginInterface
{

    Q_OBJECT
    Q_INTERFACES(IbisPlugin)
    Q_PLUGIN_METADATA(IID "Ibis.TransformOperationsPluginInterface" )

public:

    vtkTypeMacro( TransformOperationsPluginInterface, ToolPluginInterface );

    TransformOperationsPluginInterface();
    ~TransformOperationsPluginInterface();
    virtual QString GetPluginName() { return QString("TransformOperations"); }
    bool CanRun() { return true; }
    QString GetMenuEntryString() { return QString("Testing Operations on Transforms"); }
    QWidget * CreateFloatingWidget();


protected:
    TransformOperationsWidget * m_interfaceWidget;
};

#endif
