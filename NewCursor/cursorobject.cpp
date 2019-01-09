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
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include "vtkMath.h"

CursorObject::CursorObject()
{
    m_ibisAPI = nullptr;
    this->SetListable( false );
    this->SetName( "Cursor" );
    m_cursorLineThickness = 1;
    CreateCursorRepresentation();
}

CursorObject::~CursorObject()
{

}

void CursorObject::Serialize( Serializer * ser )
{
    ser->BeginSection("GlobalCursor");
    SceneObject::Serialize(ser);
    ::Serialize( ser, "CursorLineThickness", m_cursorLineThickness );
    if( ser->IsReader() )
    {
        QColor cursorColor = this->GetCursorColor();
        int color;
        if( ::Serialize( ser, "CursorColor_r", color ) )
            cursorColor.setRed( color );
        if( ::Serialize( ser, "CursorColor_g", color ) )
                cursorColor.setGreen( color );
        if( ::Serialize( ser, "CursorColor_b", color ) )
                cursorColor.setBlue( color );
        this->SetCursorColor( cursorColor );
    }
    else
    {
        int color = this->GetCursorColor().red();
        ::Serialize( ser, "CursorColor_r", color );
        color = this->GetCursorColor().green();
        ::Serialize( ser, "CursorColor_g", color );
        color = this->GetCursorColor().blue();
        ::Serialize( ser, "CursorColor_b", color );
    }
    ser->EndSection();
}

void CursorObject::Setup( View * view )
{
    view->AddInteractionObject( this, 0.5 );
    SceneObject::Setup( view );

    PerViewElements perView;
    vtkSmartPointer<vtkPolyDataMapper> cursorMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    cursorMapper->SetInputData( m_cursorPolyData );
    perView.cursorActor = vtkSmartPointer<vtkActor>::New();
    perView.cursorActor->SetMapper( cursorMapper );
//    perView.cursorActor->SetUserTransform();
    if( view->GetType() == THREED_VIEW_TYPE )
        view->GetRenderer()->AddActor( perView.cursorActor );
    else
        view->GetOverlayRenderer()->AddActor( perView.cursorActor );
}

void CursorObject::Release( View * view )
{
    view->RemoveInteractionObject(this);
    SceneObject::Release( view );
}

void CursorObject::SetIbisAPI( IbisAPI * api )
{
    m_ibisAPI = api;
    m_ibisAPI->AddObject( this );
    connect( m_ibisAPI, SIGNAL( CursorPositionChanged() ), this, SLOT( Update() ) );
    this->SetCursorColor( m_ibisAPI->GetCursorColor() );
    this->Update();
}


void CursorObject::Update()
{
}

void CursorObject::CreateCursorRepresentation()
{
    vtkSmartPointer<vtkPoints> pts = vtkSmartPointer<vtkPoints>::New();
    pts->InsertNextPoint( -100.0, 0.0, 0.0 );
    pts->InsertNextPoint( 100.0, 0.0, 0.0 );
    pts->InsertNextPoint( 0.0, -100.0, 0.0 );
    pts->InsertNextPoint( 0.0, 100.0, 0.0 );
    pts->InsertNextPoint( 0.0, 0.0, -100.0 );
    pts->InsertNextPoint( 0.0, 0.0, 100.0 );

    static vtkIdType linesIndex[3][2]= { {0,1}, {2,3}, {4,5} };
    vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
    for( int i = 0; i < 3; ++i )
        lines->InsertNextCell( 2, linesIndex[i] );

    m_cursorPolyData = vtkSmartPointer<vtkPolyData>::New();
    m_cursorPolyData->SetPoints( pts );
    m_cursorPolyData->SetLines( lines );

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

