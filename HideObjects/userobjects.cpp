#include "ibisapi.h"
#include "imageobject.h"
#include "sceneobject.h"
#include "userobjects.h"
#include "ui_userobjects.h"

UserObjects::UserObjects(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserObjects)
{
    ui->setupUi(this);
}

UserObjects::~UserObjects()
{
    delete ui;
}

void UserObjects::SetIbisAPI( IbisAPI *api )
{
    Q_ASSERT( api );
    m_ibisAPI = api;
    this->UpdateUI();
}

void UserObjects::UpdateUI()
{
    ImageObject *ref = m_ibisAPI->GetReferenceDataObject();
    if( ! ref )
        return;
    int refID = ref->GetObjectID();
    m_ibisAPI->SetCurrentObject( ref );
    QList<SceneObject*> objects;
    m_ibisAPI->GetAllUserObjects( objects );
    ui->objectsComboBox->blockSignals( true );
    ui->objectsComboBox->clear();

    ui->objectsComboBox->addItem( ref->GetName(), QVariant( refID ));
    for( int i = 0; i < objects.size(); ++i )
    {
        if( objects[i]->GetObjectID() != refID )
            ui->objectsComboBox->addItem( objects[i]->GetName(), QVariant( objects[i]->GetObjectID() ) );
    }
    ui->objectsComboBox->blockSignals( false );
}

void UserObjects::on_hideCheckBox_toggled( bool checked )
{
    QVariant v = ui->objectsComboBox->itemData( ui->objectsComboBox->currentIndex() );
    int currentID = v.toInt();
    SceneObject *currentObj = m_ibisAPI->GetObjectByID( currentID );
    if( currentObj )
        currentObj->SetHidden( checked );
}

void UserObjects::on_objectsComboBox_currentIndexChanged( int )
{
    QVariant v = ui->objectsComboBox->itemData( ui->objectsComboBox->currentIndex() );
    int currentID = v.toInt();
    SceneObject *currentObj = m_ibisAPI->GetObjectByID( currentID );
    if( currentObj->IsHidden() )
        ui->hideCheckBox->setChecked( true );
    else
        ui->hideCheckBox->setChecked( false );
}
