#include "cursorobject.h"
#include "ibisapi.h"
#include "imageobject.h"

#include <QFileDialog>
#include <QString>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkProperty.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>

CursorObject::CursorObject()
{
    m_ibisAPI = nullptr;
    m_cursorPolyData = vtkSmartPointer<vtkPolyData>::New();
    this->SetPolyData( m_cursorPolyData );
    this->CreateCursorRepresentation();
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
    m_cursorActor->PickableOff();
}

void CursorObject::Update()
{
    double p1[3];
    this->PlaneSource->GetPoint1(p1);
    double p2[3];
    this->PlaneSource->GetPoint2(p2);
    double o[3];
    this->PlaneSource->GetOrigin(o);

    double d1[3];
    vtkMath::Subtract( p1, o, d1 );
    double d2[3];
    vtkMath::Subtract( p2, o, d2 );

    double a[3];
    double b[3];
    double c[3];
    double d[3];

    for( int i = 0; i < 3; i++)
    {
        a[i] = o[i]  + this->CursorPosition[1] * d2[i];   // left
        b[i] = p1[i] + this->CursorPosition[1] * d2[i];   // right
        c[i] = o[i]  + this->CursorPosition[0] * d1[i];   // bottom
        d[i] = p2[i] + this->CursorPosition[0] * d1[i];   // top
    }

    vtkPoints * cursorPts = this->CursorPolyData->GetPoints();

    cursorPts->SetPoint(0,a);
    cursorPts->SetPoint(1,b);
    cursorPts->SetPoint(2,c);
    cursorPts->SetPoint(3,d);

    this->CursorPolyData->Modified();}

void CursorObject::SetIbisAPI( IbisAPI * api )
{
    m_ibisAPI = api;
    ImageObject* ref =  m_ibisAPI->GetReferenceDataObject();
    m_ibisAPI->AddObject( this, ref );
    connect( m_ibisAPI, SIGNAL( CursorPositionChanged() ), this, SLOT( Update() ) );
}
