#ifndef CURSOROBJECT_H
#define CURSOROBJECT_H

#include <QObject>
#include <QVector>
#include <QList>
#include <QColor>
#include "sceneobject.h"
#include "serializer.h"
#include <vtkSmartPointer.h>

class IbisAPI;
class View;
class vtkActor;
class vtkPolyData;
class vtkProperty;
class vtkPolyDataMapper;
class vtkPoints;

class CursorObject : public SceneObject
{
    Q_OBJECT

public:
    static CursorObject *New() { return new CursorObject; }

    vtkTypeMacro( CursorObject, SceneObject );

    virtual void Serialize( Serializer * ser ) override;
    virtual void Setup( View * view ) override;
    virtual void Release( View * view ) override;


    void SetCursorColor( const QColor & c );
    QColor GetCursorColor() { return m_cursorColor; }
    void SetCursorLineThickness( int s );
    int GetCursorLineThickness() { return m_cursorLineThickness; }

public slots:
    void Update();

protected:
    virtual void Hide() override;
    virtual void Show() override;

    QColor m_cursorColor;
    int m_cursorLineThickness;
    vtkSmartPointer<vtkPoints> m_cursorPoints;

    struct PerViewElements
    {
        PerViewElements() {}
        ~PerViewElements() {}
        vtkSmartPointer<vtkActor> cursorActor;
        vtkSmartPointer<vtkPolyDataMapper> cursorMapper;
    };
    typedef std::map< View*, PerViewElements* > PerViewContainer;
    PerViewContainer m_perViewContainer;

    // Property used to control the appearance of the cursor
    vtkSmartPointer<vtkProperty> m_property;
    vtkSmartPointer<vtkPolyData> m_cursorPolyData;

//    void CreateCursorRepresentation2D( int viewType, vtkPolyData* polyData );
//    void CreateCursorRepresentation3D(vtkPolyData* polyData );
    void CreateCursorRepresentation( );
    void ComputeLinesEnds(double endPoints[6][3] );
    void UpdatePoints();
    virtual void ObjectAddedToScene() override;
    virtual void ObjectAboutToBeRemovedFromScene() override;

    CursorObject();
    virtual ~CursorObject();
};

#endif // CURSOROBJECT_H
