#ifndef TRANSFORMOPERATIONSWIDGET_H
#define TRANSFORMOPERATIONSWIDGET_H

#include <QWidget>
#include "transformoperationsplugininterface.h"

class SceneObject;

namespace Ui {
class TransformOperationsWidget;
}

class vtkQtMatrixDialog;

class TransformOperationsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TransformOperationsWidget(QWidget *parent = 0);
    ~TransformOperationsWidget();
    void SetInterface( TransformOperationsPluginInterface *intface );

private slots:

    void on_identityPushButton_clicked();
    void on_concat1PushButton_clicked();
    void on_concat2PushButton_clicked();
    void on_inversePushButton_clicked();
    void on_inputPushButton_clicked();
    void on_inputConcat2PushButton_clicked();
    void EditMatrixDialogClosed();
    void UpdateUI();

private:
    vtkQtMatrixDialog * m_matrixDialog;
    SceneObject *m_selectedObject;

    TransformOperationsPluginInterface *m_pluginInterface;

    Ui::TransformOperationsWidget *ui;
};

#endif // TRANSFORMOPERATIONSWIDGET_H
