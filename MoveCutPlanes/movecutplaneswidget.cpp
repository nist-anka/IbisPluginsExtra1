#include "movecutplaneswidget.h"
#include "ui_movecutplaneswidget.h"

MoveCutPlanesWidget::MoveCutPlanesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MoveCutPlanesWidget)
{
    ui->setupUi(this);
}

MoveCutPlanesWidget::~MoveCutPlanesWidget()
{
    delete ui;
}

void MoveCutPlanesWidget::on_movePushButton_clicked()
{

}
