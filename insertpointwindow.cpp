#include "insertpointwindow.hpp"
#include "ui_insertpoint.h"

InsertPoint::InsertPoint(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InsertPoint)
{
    ui->setupUi(this);
    QRegExp regx(REAL_NUMBER_REGEX);
    QRegExpValidator *validator = new QRegExpValidator(regx, this);
    ui->campoX->setValidator(validator);
    ui->campoY->setValidator(validator);
}

InsertPoint::~InsertPoint()
{
    delete ui;
}

SpatialData::Ponto InsertPoint::GetPonto(){
    return SpatialData::Ponto(ui->campoX->text().toDouble(), ui->campoY->text().toDouble());
}

SpatialData::Vertice* InsertPoint::GetVertice(){
    SpatialData::Ponto p = GetPonto();
    SpatialData::Vertice* v = new SpatialData::Vertice(p);
    return v;
}
