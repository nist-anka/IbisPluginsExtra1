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
    this->SetListable( false );
    this->SetName( "Cursor" );
    m_cursorLineThickness = 1;
}

CursorObject::~CursorObject()
{

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
}

QColor CursorObject::GetCursorColor()
{
    return m_cursorColor;
}
