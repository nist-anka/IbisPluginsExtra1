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

private slots:

    void on_transformPushButton_clicked();
    void on_applyPushButtonClicked();

private:
    Ui::ApplyTransformToObjectWidget *ui;
};

#endif // APPLYTRANSFORMTOOBJECTWIDGET_H
