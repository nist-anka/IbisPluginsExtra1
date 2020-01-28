#ifndef MOVECUTPLANESWIDGET_H
#define MOVECUTPLANESWIDGET_H

#include <QWidget>

namespace Ui {
class MoveCutPlanesWidget;
}

class MoveCutPlanesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MoveCutPlanesWidget(QWidget *parent = 0);
    ~MoveCutPlanesWidget();

private slots:
    void on_movePushButton_clicked();

private:
    Ui::MoveCutPlanesWidget *ui;
    void UpdateUI();

};

#endif // MOVECUTPLANESWIDGET_H
