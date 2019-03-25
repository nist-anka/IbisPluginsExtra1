#include "transformoperationswidget.h"
#include "ui_transformoperationswidget.h"

#include "vtkTransform.h"
#include "vtkMatrix4x4.h"
#include "ibisapi.h"
#include "guiutilities.h"
#include "sceneobject.h"
#include "vtkQtMatrixDialog.h"

TransformOperationsWidget::TransformOperationsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TransformOperationsWidget)
{
    ui->setupUi(this);
    m_matrixDialog = nullptr;
    m_selectedObject = nullptr;
    concatenatedOnceTransform = vtkSmartPointer<vtkTransform>::New();
    concatenatedTwiceTransform = vtkSmartPointer<vtkTransform>::New();
    firstTransformToConcatenateOrInput = vtkSmartPointer<vtkTransform>::New();
    secondTransformToConcatenate = vtkSmartPointer<vtkTransform>::New();
    inputUsingFirstTransform = vtkSmartPointer<vtkTransform>::New();
    inputTransformInversed = vtkSmartPointer<vtkTransform>::New();
    concatenatedTransformInverse = vtkSmartPointer<vtkTransform>::New();
}

TransformOperationsWidget::~TransformOperationsWidget()
{
    if( m_matrixDialog )
    {
        m_matrixDialog->close();
        m_matrixDialog = 0;
    }
    delete ui;
}

void TransformOperationsWidget::on_concat1PushButton_clicked()
{
    m_matrixDialog = new vtkQtMatrixDialog( false, 0 );
    m_matrixDialog->setWindowTitle( "Edit matrix" );
    m_matrixDialog->setAttribute( Qt::WA_DeleteOnClose );
    m_matrixDialog->SetMatrix( firstTransformToConcatenateOrInput->GetMatrix() );
    m_matrixDialog->show();
    connect( m_matrixDialog, SIGNAL(MatrixModified()), this, SLOT(UpdateUI()) );
    connect( m_matrixDialog, SIGNAL(destroyed()), this, SLOT(EditMatrixDialogClosed()) );
}

void TransformOperationsWidget::on_concat2PushButton_clicked()
{
    m_matrixDialog = new vtkQtMatrixDialog( false, 0 );
    m_matrixDialog->setWindowTitle( "Edit matrix" );
    m_matrixDialog->setAttribute( Qt::WA_DeleteOnClose );
    m_matrixDialog->SetMatrix( secondTransformToConcatenate->GetMatrix() );
    m_matrixDialog->show();
    connect( m_matrixDialog, SIGNAL(MatrixModified()), this, SLOT(UpdateUI()) );
    connect( m_matrixDialog, SIGNAL(destroyed()), this, SLOT(EditMatrixDialogClosed()) );
}


void TransformOperationsWidget::on_inputPushButton_clicked()
{
    inputUsingFirstTransform->Identity();
    inputUsingFirstTransform->SetInput( firstTransformToConcatenateOrInput );
    inputUsingFirstTransform->Concatenate( secondTransformToConcatenate );
    UpdateUI();
}

void TransformOperationsWidget::on_applyConcatOncePushButton_clicked()
{

}

void TransformOperationsWidget::on_applyConcatInversePushButton_clicked()
{

}

void TransformOperationsWidget::on_applyConcatTwicePushButton_clicked()
{

}

void TransformOperationsWidget::on_applyInputInversePushButton_clicked()
{

}

void TransformOperationsWidget::SetInterface( TransformOperationsPluginInterface *intface )
{
    m_pluginInterface = intface;
    this->UpdateUI();
}

void TransformOperationsWidget::UpdateUI()
{
    Q_ASSERT(m_pluginInterface);
    IbisAPI *ibisAPI = m_pluginInterface->GetIbisAPI();
    Q_ASSERT(ibisAPI);

    QList<SceneObject*> objects;
    ibisAPI->GetAllUserObjects( objects );
    GuiUtilities::UpdateSceneObjectComboBox( ui->sceneObjectsComboBox, objects, ibisAPI->GetCurrentObject()->GetObjectID() );
    m_selectedObject = ibisAPI->GetCurrentObject();
    if( m_selectedObject == nullptr )
        return;
    this->UpdateTransforms();
    QString temp;
    this->MatrixToString( firstTransformToConcatenateOrInput->GetMatrix(), temp );
    ui->concat1TextEdit->setText( temp );
    this->MatrixToString( secondTransformToConcatenate->GetMatrix(), temp );
    ui->concat2TextEdit->setText( temp );
    this->MatrixToString( concatenatedOnceTransform->GetMatrix(), temp );
    ui->concatenated1TextEdit->setText( temp );
    this->MatrixToString( concatenatedTwiceTransform->GetMatrix(), temp );
    ui->concatenated2TextEdit->setText( temp );
    this->MatrixToString( inputUsingFirstTransform->GetMatrix(), temp );
    ui->baseWithInputConcatenatedTextEdit->setText( temp );
    this->MatrixToString( inputTransformInversed->GetMatrix(), temp );
    ui->baseWithInputInversedTextEdit->setText( temp );
    this->MatrixToString( concatenatedTransformInverse->GetMatrix(), temp );
    ui->concatenatedInversedTextEdit->setText( temp );
}

void TransformOperationsWidget::UpdateTransforms()
{
    concatenatedOnceTransform->Identity();
    concatenatedOnceTransform->Update();
    concatenatedOnceTransform->Concatenate( firstTransformToConcatenateOrInput );

    concatenatedTwiceTransform->Identity();
    concatenatedTwiceTransform->Update();
    concatenatedTwiceTransform->Concatenate( firstTransformToConcatenateOrInput );
    concatenatedTwiceTransform->Concatenate( secondTransformToConcatenate );

    inputUsingFirstTransform->Identity();
    inputUsingFirstTransform->Update();
    inputUsingFirstTransform->SetInput( firstTransformToConcatenateOrInput );
    inputUsingFirstTransform->Concatenate( secondTransformToConcatenate );

    inputTransformInversed->Identity();
    inputTransformInversed->Update();
    inputTransformInversed->SetInput( concatenatedTwiceTransform );
    inputTransformInversed->Inverse();

    concatenatedTransformInverse = concatenatedTwiceTransform->GetLinearInverse();
}

void TransformOperationsWidget::EditMatrixDialogClosed()
{
    m_matrixDialog = 0;
}

const void TransformOperationsWidget::MatrixToString(const vtkMatrix4x4 *mat , QString &formattedOutput )
{
    formattedOutput.clear();
    QString tmp;
    for( int i = 0; i < 3; i++ )
    {
        tmp.sprintf(" %+07.4f\t%+07.4f\t%+07.4f\t%+07.4f\n", mat->GetElement(i,0), mat->GetElement(i,1), mat->GetElement(i,2),  mat->GetElement(i,3) );
        formattedOutput.append(tmp);
        tmp.clear();
    }
}
