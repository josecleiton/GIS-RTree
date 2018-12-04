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
    this->setWindowTitle("Insira uma palavra");
    this->setWindowIcon(QIcon(ICON));
}

StringWindow::~StringWindow()
{
    delete ui->campoStr->validator();
    delete ui;
}

void StringWindow::on_ok_clicked()
{
    this->str = ui->campoStr->text().toStdString();
}

std::string StringWindow::getStr(){
    return this->str;
}
