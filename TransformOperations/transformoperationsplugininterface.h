
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
    virtual QString GetPluginName() override { return QString("TransformOperations"); }
    bool CanRun()  override{ return true; }
    QString GetMenuEntryString() override { return QString("Testing Operations on Transforms"); }
    QWidget * CreateFloatingWidget() override;


protected:
    TransformOperationsWidget * m_interfaceWidget;
};

#endif
