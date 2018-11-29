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
    this->setWindowTitle("Insira uma circunferência");
}

InsertCircleWindow::~InsertCircleWindow()
{
    delete ui->raio->validator();
    delete ui;
}

SpatialData::Circulo InsertCircleWindow::GetInfo(){
    return this->A;
}

void InsertCircleWindow::on_button_clicked()
{
    this->A.centro = SpatialData::Ponto(ui->xc->text().toDouble(), ui->yc->text().toDouble());
    this->A.raio = ui->raio->text().toDouble();
}

void InsertCircleWindow::on_buttonBox_clicked()
{
    close();
}
