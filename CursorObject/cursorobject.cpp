#include "cursorobject.h"
#include "ibisapi.h"
#include "imageobject.h"

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
    m_cursorPolyData = vtkSmartPointer<vtkPolyData>::New();
    this->SetPolyData( m_cursorPolyData );
    this->CreateCursorRepresentation();
    this->SetListable( false );
    this->SetName( "Cursor" );
}

CursorObject::~CursorObject()
{

}

//code copied from vtkMultiImagePlaneWidget
void CursorObject::CreateCursorRepresentation()
{
    // Construct initial points
    //
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    points->SetNumberOfPoints(4);
    int i;
    for (i = 0; i < 4; i++)
    {
        points->SetPoint(i,0.0,0.0,0.0);
    }

    vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
    cells->Allocate(cells->EstimateSize(2,2));
    vtkIdType pts[2];
    pts[0] = 0;
    pts[1] = 1;       // horizontal segment
    cells->InsertNextCell(2,pts);
    pts[0] = 2;
    pts[1] = 3;       // vertical segment
    cells->InsertNextCell(2,pts);

    m_cursorPolyData->SetPoints(points);
    points->Delete();
    m_cursorPolyData->SetLines(cells);
    cells->Delete();

    //Set properties
    m_cursorProperty = vtkSmartPointer<vtkProperty>::New();
    m_cursorProperty->SetAmbient(1);
    m_cursorProperty->SetColor( 1.0, 0.7, 0.25 );
    m_cursorProperty->SetLineWidth( .5 );
    m_cursorProperty->SetRepresentationToWireframe();
    m_cursorProperty->SetInterpolationToFlat();

    //Set actor and mapper
    m_cursorMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    m_cursorMapper->SetInputData( m_cursorPolyData );
    m_cursorMapper->SetResolveCoincidentTopologyToPolygonOffset();

    m_cursorActor = vtkSmartPointer<vtkActor>::New();
    m_cursorActor->SetProperty( m_cursorProperty );
    m_cursorActor->SetMapper( m_cursorMapper );
    m_cursorActor->PickableOn();
}

void CursorObject::Update()
{
    ImageObject* ref =  m_ibisAPI->GetReferenceDataObject();
    vtkImageData *img = ref->GetImage();
    double bounds[6], origin[3];
    img->GetBounds( bounds );
    img->GetOrigin( origin );
    m_ibisAPI->GetCursorPosition( m_cursorPosition );
#if 1
    double p1[3];
    p1[0] = bounds[0];
    p1[1] = bounds[2];
    p1[2] = bounds[5];
    double p2[3];
    p2[0] = bounds[0];
    p2[1] = bounds[3];
    p2[2] = bounds[4];

    double d1[3];
    vtkMath::Subtract( p1, origin, d1 );
    double d2[3];
    vtkMath::Subtract( p2, origin, d2 );

    double a[3];
    double b[3];
    double c[3];
    double d[3];

    for( int i = 0; i < 3; i++)
    {
        a[i] = origin[i]  + m_cursorPosition[i] * d2[i];   // left
        b[i] = p1[i] + m_cursorPosition[i] * d2[i];   // right
        c[i] = origin[i]  + m_cursorPosition[i] * d1[i];   // bottom
        d[i] = p2[i] + m_cursorPosition[i] * d1[i];   // top
    }

    vtkPoints * cursorPts = m_cursorPolyData->GetPoints();

    cursorPts->SetPoint(0,a);
    cursorPts->SetPoint(1,b);
    cursorPts->SetPoint(2,c);
    cursorPts->SetPoint(3,d);
#endif

    this->m_cursorPolyData->Modified();
}

void CursorObject::SetIbisAPI( IbisAPI * api )
{
    m_ibisAPI = api;
    ImageObject* ref =  m_ibisAPI->GetReferenceDataObject();
    m_ibisAPI->AddObject( this );
    this->SetLocalTransform( ref->GetWorldTransform() );
    connect( m_ibisAPI, SIGNAL( CursorPositionChanged() ), this, SLOT( Update() ) );
    this->Update();
}
