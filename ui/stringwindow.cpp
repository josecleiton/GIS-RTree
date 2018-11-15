#include "stringwindow.hpp"
#include "ui_stringwindow.h"

StringWindow::StringWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StringWindow)
{
    ui->setupUi(this);
    QRegExp regx(WORD_REGEX);
    QRegExpValidator *validator = new QRegExpValidator(regx, this);
    ui->campoStr->setValidator(validator);
    this->setWindowTitle("Identificador");
}

StringWindow::~StringWindow()
{
    delete ui;
}

void StringWindow::on_ok_clicked()
{
    this->str = ui->campoStr->text().toStdString();
}

std::string StringWindow::getStr(){
    return this->str;
}
