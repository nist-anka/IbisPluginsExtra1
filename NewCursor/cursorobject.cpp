#include "cursorobject.h"
#include "ibisapi.h"
#include "imageobject.h"
#include "scenemanager.h"
#include "view.h"

#include <QFileDialog>
#include <QString>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkImageData.h>
#include <vtkProperty.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include "vtkMath.h"

CursorObject::CursorObject()
{
    m_ibisAPI = nullptr;
    this->SetListable( false );
    this->SetName( "Cursor" );
    m_cursorLineThickness = 1;
}

CursorObject::~CursorObject()
{

}

void CursorObject::Serialize( Serializer * ser )
{
    ser->BeginSection("GlobalCursor");
    if( ser->IsReader() )
    {
        QColor cursorColor = this->GetCursorColor();
        int color;
        if( ::Serialize( &ser, "CursorColor_r", color ) )
            cursorColor.setRed( color );
        if( ::Serialize( &ser, "CursorColor_g", color ) )
                cursorColor.setGreen( color );
        if( ::Serialize( &ser, "CursorColor_b", color ) )
                cursorColor.setBlue( color );
        this->SetCursorColor( cursorColor );
    }
    else
    {
        int color = this->GetCursorColor().red();
        ::Serialize( &ser, "CursorColor_r", color );
        color = this->GetCursorColor().green();
        ::Serialize( &ser, "CursorColor_g", color );
        color = this->GetCursorColor().blue();
        ::Serialize( &ser, "CursorColor_b", color );
    }
    ser->EndSection();
}

void CursorObject::Setup( View * view )
{
    SceneObject::Setup( view );
}

void CursorObject::Release( View * view )
{
    SceneObject::Release( view );
}

void CursorObject::Update()
{
}

void CursorObject::SetIbisAPI( IbisAPI * api )
{
    m_ibisAPI = api;
    m_ibisAPI->AddObject( this );
    connect( m_ibisAPI, SIGNAL( CursorPositionChanged() ), this, SLOT( Update() ) );
    this->SetCursorColor( m_ibisAPI->GetCursorColor() );
    this->Update();
}

void CursorObject::SetCursorColor( const QColor & c )
{
    m_cursorColor = c;
    if( m_ibisAPI )
        m_ibisAPI->SetCursorColor( m_cursorColor );
}

void CursorObject::SetCursorLineThickness( int s )
{
    m_cursorLineThickness = s;
    this->Update();
}

void CursorObject::ObjectAddedToScene()
{
    Q_ASSERT( GetManager() );

    connect( this->GetManager(), SIGNAL(CursorPositionChanged()), this, SLOT(Update()) );
}

void CursorObject::ObjectAboutToBeRemovedFromScene()
{
    Q_ASSERT( GetManager() );
    disconnect( this->GetManager(), SIGNAL(CursorPositionChanged()), this, SLOT(Update()) );
}

