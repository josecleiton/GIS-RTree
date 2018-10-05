#ifndef POINTINSERT_HPP
#define POINTINSERT_HPP

#include <QDialog>
#include "insertwindow.hpp"
#include "spatialdata.hpp"

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
