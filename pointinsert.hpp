#ifndef POINTINSERT_HPP
#define POINTINSERT_HPP

#include <QDialog>
#include <QRegExpValidator>
#include "insertwindow.hpp"
#include "spatialdata.hpp"

#define REAL_NUMBER_REGEX "^(-|\\+)?(([1-9][0-9]*)|(0))(?:\\.[0-9]+)?$"

namespace Ui {
class PointInsert;
}

class PointInsert : public QDialog
{
    Q_OBJECT

public:
    explicit PointInsert(QWidget *parent = nullptr);
    ~PointInsert();
    SpatialData::Ponto GetPonto();
    SpatialData::Vertice* GetVertice();

private:
    Ui::PointInsert *ui;
    friend class InsertWindow;
};

#endif // POINTINSERT_HPP
