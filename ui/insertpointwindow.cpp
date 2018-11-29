#include "insertpointwindow.hpp"
#include "ui_insertpointwindow.h"

InsertPoint::InsertPoint(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InsertPoint)
{
    ui->setupUi(this);
    QRegExp regx(REAL_NUMBER_REGEX);
    QRegExpValidator *validator = new QRegExpValidator(regx, this);
    ui->campoX->setValidator(validator);
    ui->campoY->setValidator(validator);
    this->setWindowTitle("Insira um ponto");
}

InsertPoint::~InsertPoint()
{
    delete ui->campoX->validator();
    delete ui;
}

SpatialData::Ponto* InsertPoint::GetPonto(){
    SpatialData::Ponto* P = nullptr;
    if(ui->campoX->text().size() and ui->campoY->text().size())
        P = new SpatialData::Ponto(ui->campoX->text().toDouble(), ui->campoY->text().toDouble());
    Clear();
    return P;
}

SpatialData::Vertice* InsertPoint::GetVertice(){
    SpatialData::Ponto *p = GetPonto();
    SpatialData::Vertice* v = nullptr;
    if(p != nullptr){
        v = new SpatialData::Vertice(*p);
        delete p;
    }
    return v;
}

void InsertPoint::Clear(){
    ui->campoX->clear();
    ui->campoY->clear();
}
