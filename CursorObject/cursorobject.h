#ifndef CURSOROBJECT_H
#define CURSOROBJECT_H

#include <QObject>
#include "polydataobject.h"
#include <vtkSmartPointer.h>

class IbisAPI;
class vtkPolyData;
class vtkProperty;
class vtkPolyDataMapper;
class vtkActor;

class CursorObject : public PolyDataObject
{
    Q_OBJECT

public:
    static CursorObject *New() { return new CursorObject; }

    vtkTypeMacro( CursorObject,PolyDataObject );

    void SetIbisAPI( IbisAPI * api );

public slots:
    void Update();

protected:
    void CreateCursorRepresentation();
    IbisAPI *m_ibisAPI;
    vtkSmartPointer<vtkPolyData> m_cursorPolyData;
    vtkSmartPointer<vtkProperty> m_cursorProperty;
    vtkSmartPointer<vtkActor> m_cursorActor;
    vtkSmartPointer<vtkPolyDataMapper> m_cursorMapper;

    CursorObject();
    virtual ~CursorObject();
};

#endif // CURSOROBJECT_H
