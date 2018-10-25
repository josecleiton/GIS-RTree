#include "insertcirclewindow.hpp"
#include "ui_insertcirclewindow.h"

InsertCircleWindow::InsertCircleWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InsertCircleWindow)
{
    ui->setupUi(this);
    QRegExp regx(REAL_NUMBER_REGEX);
    QRegExpValidator *validator = new QRegExpValidator(regx, this);
    ui->raio->setValidator(validator);
    ui->xc->setValidator(validator);
    ui->yc->setValidator(validator);
}

InsertCircleWindow::~InsertCircleWindow()
{
    delete ui;
}

SpatialData::Circulo InsertCircleWindow::GetInfo(){
    return SpatialData::Circulo(ui->raio->text().toDouble(), SpatialData::Ponto(ui->xc->text().toDouble(), ui->yc->text().toDouble()));
}
