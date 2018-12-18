#ifndef CURSOROBJECT_H
#define CURSOROBJECT_H

#include <QObject>
#include <QObject>
#include <QVector>
#include <QList>
#include <QColor>
#include "sceneobject.h"
#include "serializer.h"
#include <vtkSmartPointer.h>

class IbisAPI;

class CursorObject : public SceneObject
{
    Q_OBJECT

public:
    static CursorObject *New() { return new CursorObject; }

    vtkTypeMacro( CursorObject, SceneObject );

    virtual void Serialize( Serializer * ser ) override;
    virtual void Setup( View * view ) override;
    virtual void Release( View * view ) override;


    void SetIbisAPI( IbisAPI * api );
    void SetCursorColor( const QColor & c );
    QColor GetCursorColor();

public slots:
    void Update();

protected:
    IbisAPI *m_ibisAPI;
    QColor m_cursorColor;
    int m_cursorLineThickness;

    CursorObject();
    virtual ~CursorObject();
};

#endif // CURSOROBJECT_H
