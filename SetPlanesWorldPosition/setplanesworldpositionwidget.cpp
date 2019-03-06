#include "setplanesworldpositionwidget.h"
#include "ui_setplanesworldpositionwidget.h"
#include "ibisapi.h"

SetPlanesWorldPositionWidget::SetPlanesWorldPositionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetPlanesWorldPositionWidget)
{
    ui->setupUi(this);

    m_pluginInterface = nullptr;
}

SetPlanesWorldPositionWidget::~SetPlanesWorldPositionWidget()
{
    delete ui;
}


void SetPlanesWorldPositionWidget::SetInterface( SetPlanesWorldPositionPluginInterface *intf )
{
    if( intf )
    {
        m_pluginInterface = intf;
        this->UpdateUI();
        IbisAPI *ibisAPI = m_pluginInterface->GetIbisAPI();
        Q_ASSERT(ibisAPI);
        connect( ibisAPI, SIGNAL(CursorPositionChanged()), SLOT(UpdateUI()) );
    }
}


void SetPlanesWorldPositionWidget::UpdateUI()
{
    Q_ASSERT(m_pluginInterface);
    IbisAPI *ibisAPI = m_pluginInterface->GetIbisAPI();
    Q_ASSERT(ibisAPI);
    double currentPosition[3];
    ibisAPI->GetCursorWorldPosition( currentPosition );
    ui->curXLineEdit->setText( QString::number( currentPosition[0], 'f', 2 ) );
    ui->curYLineEdit->setText( QString::number( currentPosition[1], 'f', 2 ) );
    ui->curZLineEdit->setText( QString::number( currentPosition[2], 'f', 2 ) );
}

void SetPlanesWorldPositionWidget::on_applyPushButton_clicked()
{
}
