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
    m_cursorPolyData = vtkSmartPointer<vtkPolyData>::New();
    m_cursorPoints = vtkSmartPointer<vtkPoints>::New();
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
    Q_ASSERT( GetManager() );
    this->SetCursorColor( GetManager()->GetCursorColor() );
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
    connect( this, SIGNAL(ObjectModified()), view, SLOT(NotifyNeedRender()) );
    view->NotifyNeedRender();
}

void CursorObject::Release( View * view )
{
    disconnect( this, SIGNAL(ObjectModified()), view, SLOT(NotifyNeedRender()) );
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

void CursorObject::Hide()
{
    PerViewContainer::iterator it = m_perViewContainer.begin();
    while( it != m_perViewContainer.end() )
    {
        PerViewElements * perView = (*it).second;
        perView->cursorActor->VisibilityOff();
        ++it;
    }
}

void CursorObject::Show()
{
    PerViewContainer::iterator it = m_perViewContainer.begin();
    while( it != m_perViewContainer.end() )
    {
        PerViewElements * perView = (*it).second;
        perView->cursorActor->VisibilityOn();
        ++it;
    }
}


void CursorObject::Update()
{
    UpdatePoints();
    PerViewContainer::iterator it = m_perViewContainer.begin();
    while( it != m_perViewContainer.end() )
    {
        PerViewElements * perView = (*it).second;
        perView->cursorMapper->Update();
        ++it;
    }
    this->ObjectModified();
}

void CursorObject::ComputeLinesEnds( double endPoints[6][3] )
{
    Q_ASSERT( GetManager() );
    ImageObject *ref = GetManager()->GetReferenceDataObject();
    Q_ASSERT( ref );
    vtkImageData *img = ref->GetImage();
    double bounds[6];
    double position[3];
    img->GetBounds( bounds );
    GetManager()->GetCursorPosition( position );
    // line along x
    endPoints[0][0] = bounds[0];
    endPoints[0][1] = position[1];
    endPoints[0][2] = position[2];
    endPoints[1][0] = bounds[1];
    endPoints[1][1] = position[1];
    endPoints[1][2] = position[2];
    // line along y
    endPoints[2][0] = position[0];
    endPoints[2][1] = bounds[2];
    endPoints[2][2] = position[2];
    endPoints[3][0] = position[0];
    endPoints[3][1] = bounds[3];
    endPoints[3][2] = position[2];
    // line along z
    endPoints[4][0] = position[0];
    endPoints[4][1] = position[1];
    endPoints[4][2] = bounds[4];
    endPoints[5][0] = position[0];
    endPoints[5][1] = position[1];
    endPoints[5][2] = bounds[5];
}

//void CursorObject::CreateCursorRepresentation2D( int viewType, vtkPolyData* polyData )
//{
//    double endPoints[6][3];
//    ComputeLinesEnds( endPoints );
//    vtkSmartPointer<vtkPoints> pts = vtkSmartPointer<vtkPoints>::New();
//    switch( viewType )
//    {
//        case SAGITTAL_VIEW_TYPE: //plane y and z
//            pts->InsertNextPoint( endPoints[2][0], endPoints[2][1], endPoints[2][2] );
//            pts->InsertNextPoint( endPoints[3][0], endPoints[3][1], endPoints[3][2] );
//            pts->InsertNextPoint( endPoints[4][0], endPoints[4][1], endPoints[4][2] );
//            pts->InsertNextPoint( endPoints[5][0], endPoints[5][1], endPoints[5][2] );
//            break;
//        case CORONAL_VIEW_TYPE: // plane x and z
//            pts->InsertNextPoint( endPoints[0][0], endPoints[0][1], endPoints[0][2] );
//            pts->InsertNextPoint( endPoints[1][0], endPoints[1][1], endPoints[1][2] );
//            pts->InsertNextPoint( endPoints[4][0], endPoints[4][1], endPoints[4][2] );
//            pts->InsertNextPoint( endPoints[5][0], endPoints[5][1], endPoints[5][2] );
//            break;
//        default: // plane x and y
//        case TRANSVERSE_VIEW_TYPE:
//            pts->InsertNextPoint( endPoints[0][0], endPoints[0][1], endPoints[0][2] );
//            pts->InsertNextPoint( endPoints[1][0], endPoints[1][1], endPoints[1][2] );
//            pts->InsertNextPoint( endPoints[2][0], endPoints[2][1], endPoints[2][2] );
//            pts->InsertNextPoint( endPoints[3][0], endPoints[3][1], endPoints[3][2] );
//            break;
//    }

//    static vtkIdType linesIndex[2][2]= { {0,1}, {2,3} };
//    vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
//    for( int i = 0; i < 2; ++i )
//        lines->InsertNextCell( 2, linesIndex[i] );

//    polyData->SetPoints( pts );
//    polyData->SetLines( lines );
//}

//void CursorObject::CreateCursorRepresentation3D( vtkPolyData * polyData )
//{
//    double endPoints[6][3];
//    ComputeLinesEnds( endPoints );
//    vtkSmartPointer<vtkPoints> pts = vtkSmartPointer<vtkPoints>::New();
//    pts->InsertNextPoint( endPoints[0][0], endPoints[0][1], endPoints[0][2] );
//    pts->InsertNextPoint( endPoints[1][0], endPoints[1][1], endPoints[1][2] );
//    pts->InsertNextPoint( endPoints[2][0], endPoints[2][1], endPoints[2][2] );
//    pts->InsertNextPoint( endPoints[3][0], endPoints[3][1], endPoints[3][2] );
//    pts->InsertNextPoint( endPoints[4][0], endPoints[4][1], endPoints[4][2] );
//    pts->InsertNextPoint( endPoints[5][0], endPoints[5][1], endPoints[5][2] );

//    static vtkIdType linesIndex[3][2]= { {0,1}, {2,3}, {4,5} };
//    vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
//    for( int i = 0; i < 3; ++i )
//        lines->InsertNextCell( 2, linesIndex[i] );

//    polyData->SetPoints( pts );
//    polyData->SetLines( lines );
//}

void CursorObject::UpdatePoints()
{
    double endPoints[6][3];
    ComputeLinesEnds( endPoints );
    m_cursorPoints->InsertNextPoint( endPoints[0][0], endPoints[0][1], endPoints[0][2] );
    m_cursorPoints->InsertNextPoint( endPoints[1][0], endPoints[1][1], endPoints[1][2] );
    m_cursorPoints->InsertNextPoint( endPoints[2][0], endPoints[2][1], endPoints[2][2] );
    m_cursorPoints->InsertNextPoint( endPoints[3][0], endPoints[3][1], endPoints[3][2] );
    m_cursorPoints->InsertNextPoint( endPoints[4][0], endPoints[4][1], endPoints[4][2] );
    m_cursorPoints->InsertNextPoint( endPoints[5][0], endPoints[5][1], endPoints[5][2] );
}
void CursorObject::CreateCursorRepresentation( )
{
    UpdatePoints();
    static vtkIdType linesIndex[3][2]= { {0,1}, {2,3}, {4,5} };
    vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
    for( int i = 0; i < 3; ++i )
        lines->InsertNextCell( 2, linesIndex[i] );

    m_cursorPolyData->SetPoints( m_cursorPoints );
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
    this->ObjectModified();
 }

void CursorObject::SetCursorLineThickness( int s )
{
    m_cursorLineThickness = s;
    this->ObjectModified();
}

void CursorObject::ObjectAddedToScene()
{
    Q_ASSERT( GetManager() );
    CreateCursorRepresentation();

    connect( this->GetManager(), SIGNAL(CursorPositionChanged()), this, SLOT(Update()) );
    this->Update();
}

void CursorObject::ObjectAboutToBeRemovedFromScene()
{
    Q_ASSERT( GetManager() );
    disconnect( this->GetManager(), SIGNAL(CursorPositionChanged()), this, SLOT(Update()) );
}

