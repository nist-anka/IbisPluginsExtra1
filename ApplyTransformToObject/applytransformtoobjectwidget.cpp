#include "applytransformtoobjectwidget.h"
#include "ui_applytransformtoobjectwidget.h"

ApplyTransformToObjectWidget::ApplyTransformToObjectWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ApplyTransformToObjectWidget)
{
    ui->setupUi(this);
}

ApplyTransformToObjectWidget::~ApplyTransformToObjectWidget()
{
    delete ui;
}
