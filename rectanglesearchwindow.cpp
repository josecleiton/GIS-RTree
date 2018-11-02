#include "rectanglesearchwindow.hpp"
#include "ui_rectanglesearchwindow.h"

RectangleSearchWindow::RectangleSearchWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RectangleSearchWindow)
{
    ui->setupUi(this);
    QRegExp regx(REAL_NUMBER_REGEX);
    QRegExpValidator *validator = new QRegExpValidator(regx, this);
    ui->p1x->setValidator(validator);
    ui->p1y->setValidator(validator);
    ui->p2x->setValidator(validator);
    ui->p2y->setValidator(validator);
    this->reg = nullptr;
}

RectangleSearchWindow::~RectangleSearchWindow()
{
    delete ui;
}

void RectangleSearchWindow::on_button_clicked()
{
    SpatialData::Ponto A(ui->p1x->text().toDouble(), ui->p1y->text().toDouble());
    SpatialData::Ponto B(ui->p2x->text().toDouble(), ui->p2y->text().toDouble());
    SpatialData::Retangulo R(A, B);
    SpatialIndex::Chave ChaveEncontrada = root.Busca(R);
    if(ChaveEncontrada.Dado == 0){
        //NÃO ENCONTRADO
        QMessageBox mbox;
        mbox.critical(nullptr, "Erro", "Retângulo não encontrado no banco de dados.");
    }
    else{
        DiskAPI::Disk io(FILENAME);
        streampos pos = ChaveEncontrada.Dado;
        DiskAPI::Registro* Reg = io.Read(pos);
        this->reg = Reg;
        if(!Interseccao()){
            FindWindow FW;
            FW.setModal(true);
            FW.setReg(Reg);
            FW.exec();
            if(FW.GetRemove()){
                root.Remove(ChaveEncontrada);
                io.Remove(pos);
            }
            delete Reg;
        }
    }
}

DiskAPI::Registro* RectangleSearchWindow::GetRegistro(){
    return this->reg;
}

bool RectangleSearchWindow::Interseccao(){
    return this->interseccao;
}

void RectangleSearchWindow::SetInterseccao(bool k){
    this->interseccao = k;
}

void RectangleSearchWindow::on_cancel_clicked()
{
    close();
}
