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

//    void on_transformPushButton_clicked();
//    void on_sceneObjectsComboBox_currentIndexChanged(int index);
    void EditMatrixDialogClosed();
    void UpdateUI();

private:
    vtkQtMatrixDialog * m_matrixDialog;
    SceneObject *m_selectedObject;

    TransformOperationsPluginInterface *m_pluginInterface;

    Ui::TransformOperationsWidget *ui;
};

#endif // TRANSFORMOPERATIONSWIDGET_H
