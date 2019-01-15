#include "cursorobject.h"
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
    this->SetListable( false );
    this->SetName( "Cursor" );
    m_cursorLineThickness = 1;
    m_property = vtkSmartPointer<vtkProperty>::New();
    m_property->SetColor(1.0,0.0,0.0);
    m_property->SetAmbient(1);
    m_property->SetLineWidth(1);
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

    PerViewElements * perView = new PerViewElements;
    perView->cursorMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    perView->cursorMapper->SetInputData( m_cursorPolyData );
    perView->cursorActor = vtkSmartPointer<vtkActor>::New();
    perView->cursorActor->SetMapper( perView->cursorMapper );
    perView->cursorActor->SetProperty( m_property );
    if( view->GetType() == THREED_VIEW_TYPE )
        view->GetRenderer()->AddActor( perView->cursorActor );
    else
        view->GetOverlayRenderer()->AddActor( perView->cursorActor );
    m_perViewContainer[view] = perView;
}

void CursorObject::Release( View * view )
{
    view->RemoveInteractionObject(this);
    PerViewContainer::iterator it = m_perViewContainer.find( view );
    if( it != m_perViewContainer.end() )
    {
        PerViewElements * perView = (*it).second;
        if( view->GetType() == THREED_VIEW_TYPE )
        {
            view->GetRenderer()->RemoveActor( perView->cursorActor );
        }
        else
            view->GetOverlayRenderer()->RemoveActor( perView->cursorActor );
        delete perView;
        this->m_perViewContainer.erase( it );
    }
    SceneObject::Release( view );
}

void CursorObject::Update()
{
    Q_ASSERT( GetManager() );
    ImageObject *ref = GetManager()->GetReferenceDataObject();
    vtkImageData *img = ref->GetImage();
    double bounds[6];
    img->GetBounds( bounds );
    vtkPoints *pts = m_cursorPolyData->GetPoints();
    double pos[3];
    GetManager()->GetCursorPosition( pos );


    PerViewContainer::iterator it = m_perViewContainer.begin();
    while( it != m_perViewContainer.end() )
    {
        PerViewElements * perView = (*it).second;
        perView->cursorMapper->Update();
    }
    this->SetCursorColor( GetManager()->GetCursorColor() );
    this->Modified();
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
    Q_ASSERT( GetManager() );
    m_cursorColor = c;
    double color[3];
    color[0] = m_cursorColor.red() / 255.0;
    color[1] = m_cursorColor.green() / 255.0;
    color[2] = m_cursorColor.blue() / 255.0;
    m_property->SetColor( color );
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
    this->Update();
}

void CursorObject::ObjectAboutToBeRemovedFromScene()
{
    Q_ASSERT( GetManager() );
    disconnect( this->GetManager(), SIGNAL(CursorPositionChanged()), this, SLOT(Update()) );
}

