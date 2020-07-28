#ifndef TRANSFORMOPERATIONSWIDGET_H
#define TRANSFORMOPERATIONSWIDGET_H

#include <QWidget>
#include "transformoperationsplugininterface.h"
#include "vtkSmartPointer.h"

class SceneObject;
class ImageObject;
class QString;
class vtkQtMatrixDialog;
class vtkTransform;
class vtkMatrix4x4;
class vtkLinearTransform;

namespace Ui {
class TransformOperationsWidget;
}


class TransformOperationsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TransformOperationsWidget(QWidget *parent = 0);
    ~TransformOperationsWidget();
    void SetInterface( TransformOperationsPluginInterface *intface );

private slots:

    void on_concat1PushButton_clicked();
    void on_concat2PushButton_clicked();
    void on_applyConcatOncePushButton_clicked();
    void on_applyConcatInversePushButton_clicked();
    void on_applyConcatTwicePushButton_clicked();
    void on_applyInputInversePushButton_clicked();
    void EditMatrixDialogClosed();
    void UpdateMatrix( vtkMatrix4x4 *);
    void UpdateUI();

private:
    vtkQtMatrixDialog * m_matrixDialog;
    SceneObject *m_selectedObject;
    ImageObject *m_referenceObject;
    TransformOperationsPluginInterface *m_pluginInterface;

    void MatrixToString(const vtkMatrix4x4 *mat, QString &formattedOutput );
    void UpdateTransforms();
    void CheckReferenceDataObject( );

    vtkSmartPointer<vtkTransform> concatenatedOnceTransform;
    vtkSmartPointer<vtkTransform> concatenatedTwiceTransform;
    vtkSmartPointer<vtkTransform> inputUsingconcatenatedTwiceTransform;
    vtkSmartPointer<vtkTransform> firstTransformToConcatenateOrInput;
    vtkSmartPointer<vtkTransform> secondTransformToConcatenate;
    vtkSmartPointer<vtkTransform> inputTransformInversed;
    vtkSmartPointer<vtkLinearTransform> concatenatedTransformInverse;

    Ui::TransformOperationsWidget *ui;
};

#endif // TRANSFORMOPERATIONSWIDGET_H
