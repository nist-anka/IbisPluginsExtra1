#ifndef USEROBJECTS_H
#define USEROBJECTS_H

#include <QWidget>
class IbisAPI;

namespace Ui {
class UserObjects;
}

class UserObjects : public QWidget
{
    Q_OBJECT

public:
    explicit UserObjects(QWidget *parent = 0);
    ~UserObjects();
    void SetIbisAPI( IbisAPI *api );

public slots:
    void UpdateUI();

private slots:
    void on_hideCheckBox_toggled( bool checked );
    void on_objectsComboBox_currentIndexChanged( int );

private:
    Ui::UserObjects *ui;
    IbisAPI *m_ibisAPI;
};

#endif // USEROBJECTS_H
