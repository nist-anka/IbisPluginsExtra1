#ifndef APPLYTRANSFORMTOOBJECTWIDGET_H
#define APPLYTRANSFORMTOOBJECTWIDGET_H

#include <QWidget>

namespace Ui {
class ApplyTransformToObjectWidget;
}

class ApplyTransformToObjectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ApplyTransformToObjectWidget(QWidget *parent = 0);
    ~ApplyTransformToObjectWidget();

private:
    Ui::ApplyTransformToObjectWidget *ui;
};

#endif // APPLYTRANSFORMTOOBJECTWIDGET_H
