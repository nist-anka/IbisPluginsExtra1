#include "transformoperationswidget.h"
#include "ui_transformoperationswidget.h"

#include <iostream>
#include "vtkTransform.h"
#include "vtkMatrix4x4.h"
#include <vtkIndent.h>
#include "ibisapi.h"
#include "guiutilities.h"
#include "sceneobject.h"
#include "imageobject.h"
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
    inputUsingconcatenatedTwiceTransform = vtkSmartPointer<vtkTransform>::New();
    inputTransformInversed = vtkSmartPointer<vtkTransform>::New();
    concatenatedTransformInverse = vtkSmartPointer<vtkTransform>::New();
    m_referenceObject = nullptr;
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
    connect( m_matrixDialog, SIGNAL(MatrixModified(vtkMatrix4x4 *)), this, SLOT(UpdateMatrix( vtkMatrix4x4 *)) );
    connect( m_matrixDialog, SIGNAL(destroyed()), this, SLOT(EditMatrixDialogClosed()) );
}

void TransformOperationsWidget::on_concat2PushButton_clicked()
{
    m_matrixDialog = new vtkQtMatrixDialog( false, 0 );
    m_matrixDialog->setWindowTitle( "Edit matrix" );
    m_matrixDialog->setAttribute( Qt::WA_DeleteOnClose );
    m_matrixDialog->SetMatrix( secondTransformToConcatenate->GetMatrix() );
    m_matrixDialog->show();
    connect( m_matrixDialog, SIGNAL(MatrixModified(vtkMatrix4x4 *)), this, SLOT(UpdateMatrix( vtkMatrix4x4 *)) );
    connect( m_matrixDialog, SIGNAL(destroyed()), this, SLOT(EditMatrixDialogClosed()) );
}

void TransformOperationsWidget::on_applyConcatOncePushButton_clicked()
{
    if( m_referenceObject != nullptr )
    {
        m_referenceObject->SetLocalTransform( concatenatedOnceTransform );
        this->UpdateUI();
    }
}

void TransformOperationsWidget::on_applyConcatInversePushButton_clicked()
{
    if( m_referenceObject != nullptr )
    {
        m_referenceObject->GetLocalTransform()->Identity();
        m_referenceObject->GetLocalTransform()->SetInput( concatenatedTransformInverse );
        this->UpdateUI();
    }
}

void TransformOperationsWidget::on_applyConcatTwicePushButton_clicked()
{
    if( m_referenceObject != nullptr )
    {
        m_referenceObject->SetLocalTransform( concatenatedTwiceTransform );
        this->UpdateUI();
    }
}

void TransformOperationsWidget::on_applyInputInversePushButton_clicked()
{
    if( m_referenceObject != nullptr )
    {
        m_referenceObject->SetLocalTransform( inputTransformInversed );
        this->UpdateUI();
    }
}

void TransformOperationsWidget::SetInterface( TransformOperationsPluginInterface *intface )
{
    m_pluginInterface = intface;
    this->CheckReferenceDataObject();
    this->UpdateUI();
}

void TransformOperationsWidget::UpdateMatrix( vtkMatrix4x4 * newMatrix )
{
    if( m_selectedObject )
    {
        m_selectedObject->GetLocalTransform()->SetMatrix( newMatrix );
        m_selectedObject->NotifyTransformChanged();
    }
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
    this->MatrixToString( inputUsingconcatenatedTwiceTransform->GetMatrix(), temp );
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

    inputUsingconcatenatedTwiceTransform->Identity();
    inputUsingconcatenatedTwiceTransform->Update();
    inputUsingconcatenatedTwiceTransform->SetInput( concatenatedTwiceTransform );

    inputTransformInversed->Identity();
    inputTransformInversed->SetInput( concatenatedTwiceTransform );
    inputTransformInversed->Inverse();
    vtkIndent indent;
    std::cout << "Inversed inputTransformInversed\n";
    inputTransformInversed->PrintSelf( std::cout, indent );

    double point[3] = { 0.0, 0.0, 0.0 };
    double *outputPoint = inputTransformInversed->TransformDoublePoint( point );

    concatenatedTransformInverse = concatenatedTwiceTransform->GetLinearInverse();
    indent = indent.GetNextIndent();
    std::cout << "Inversed concatenatedTransformInverse\n";
    concatenatedTransformInverse->PrintSelf( std::cout, indent );

    double point1[3] = { 0.0, 0.0, 0.0 };
    double *outputPoint1 = concatenatedTransformInverse->TransformDoublePoint( point1 ) ;
}

void TransformOperationsWidget::EditMatrixDialogClosed()
{
    m_matrixDialog = 0;
}

void TransformOperationsWidget::MatrixToString(const vtkMatrix4x4 *mat , QString &formattedOutput )
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

void TransformOperationsWidget::CheckReferenceDataObject( )
{
    Q_ASSERT(m_pluginInterface);
    IbisAPI *ibisAPI = m_pluginInterface->GetIbisAPI();
    Q_ASSERT(ibisAPI);

    m_referenceObject = ibisAPI->GetReferenceDataObject();
    if( m_referenceObject != nullptr )
    {
        vtkTransform *referenceTransform = vtkTransform::New();
        referenceTransform->Identity();
        vtkTransform *invReferenceTransformnsform = vtkTransform::New();
        invReferenceTransformnsform->Identity();
        referenceTransform->SetInput( m_referenceObject->GetWorldTransform() );
        vtkIndent indent;
        vtkTransform *wt = m_referenceObject->GetWorldTransform();
        std::cout << "World Transform Local Transform: " << m_referenceObject->GetLocalTransform() << std::endl;
        std::cout << "World Transform of WorldObject: " << m_referenceObject->GetParent()->GetWorldTransform() << std::endl;
        std::cout << "World Transform wt: " << wt << std::endl;
        wt->PrintSelf( std::cout, indent );
        indent = indent.GetNextIndent();
        std::cout << "Reference Transform: referenceTransform: " << referenceTransform << std::endl;
        referenceTransform->PrintSelf( std::cout, indent );
        invReferenceTransformnsform->SetInput( m_referenceObject->GetWorldTransform() );
        invReferenceTransformnsform->Inverse();
        indent = indent.GetNextIndent();
        std::cout << "Inversed Reference Transform: invReferenceTransformnsform\n";
        invReferenceTransformnsform->PrintSelf( std::cout, indent );
        vtkLinearTransform *linearInverse = referenceTransform->GetLinearInverse();
        std::cout << "Linear Inverse of Reference Transform: linearInverse\n";
        indent = indent.GetNextIndent();
        linearInverse->PrintSelf( std::cout, indent );
        vtkLinearTransform *linearInverse1 = wt->GetLinearInverse();
        linearInverse1->Update();
        std::cout << "Linear Inverse of wt: linearInverse1\n";
        linearInverse1->PrintSelf( std::cout, indent );
        std::cout << std::flush;
        referenceTransform->Delete();
        invReferenceTransformnsform->Delete();
    }
}
