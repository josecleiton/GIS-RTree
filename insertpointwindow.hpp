#ifndef POINTINSERT_HPP
#define POINTINSERT_HPP

#include <QDialog>
#include <QRegExpValidator>
#include "insertwindow.hpp"
#include "spatialdata.hpp"

#define REAL_NUMBER_REGEX "^(-|\\+)?(([1-9][0-9]*)|(0))(?:\\.[0-9]+)?$"

namespace Ui {
class InsertPoint;
}

class InsertPoint: public QDialog
{
    Q_OBJECT

public:
    explicit InsertPoint(QWidget *parent = nullptr);
    ~InsertPoint();
    SpatialData::Ponto GetPonto();
    SpatialData::Vertice* GetVertice();

private:
    Ui::InsertPoint *ui;
    void Clear();
    friend class InsertWindow;
};

#endif // POINTINSERT_HPP
