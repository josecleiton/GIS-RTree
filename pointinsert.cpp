#include "pointinsert.hpp"
#include "ui_pointinsert.h"

PointInsert::PointInsert(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PointInsert)
{
    ui->setupUi(this);
    QRegExp regx(REAL_NUMBER_REGEX);
    QRegExpValidator *validator = new QRegExpValidator(regx, this);
    ui->campoX->setValidator(validator);
    ui->campoY->setValidator(validator);
}

PointInsert::~PointInsert()
{
    delete ui;
}

SpatialData::Ponto PointInsert::GetPonto(){
    return SpatialData::Ponto(ui->campoX->text().toDouble(), ui->campoY->text().toDouble());
}

SpatialData::Vertice* PointInsert::GetVertice(){
    SpatialData::Ponto p = GetPonto();
    SpatialData::Vertice* v = new SpatialData::Vertice(p);
    return v;
}
