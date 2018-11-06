#include "rectanglesearchwindow.hpp"
#include "ui_rectanglesearchwindow.h"


RectangleSearchWindow::RectangleSearchWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RectangleSearchWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Busca por retângulo");
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
    vector<SpatialIndex::NodeAux> caminho;
    bool ChaveEncontrada = root.Busca(root.GetPtr(), R, caminho);
    if(ChaveEncontrada){
        DiskAPI::Disk io(FILENAME);
        streampos pos = caminho.front().ptr->Chaves[caminho.front().index].Dado;
        DiskAPI::Registro* Reg = io.Read(pos);
        if(Reg != nullptr){
            this->reg = Reg;
            if(!Interseccao()){
                FindWindow FW;
                FW.setModal(true);
                FW.setReg(Reg);
                FW.exec();
                if(FW.GetRemove()){
                    root.Remove(caminho);
                    io.Remove(pos);
                }
                delete Reg;
            }
            SpatialIndex::Kai(caminho);
        }
        else{
            QMessageBox erro;
            erro.critical(nullptr, "ERRO CRITICO DE ORGANIZAÇÃO", "Fatal error: banco de dados necessita reorganização.");
            this->close();
        }
    }
    else{
        //NÃO ENCONTRADO
        QMessageBox mbox;
        mbox.critical(nullptr, "Erro", "Retângulo não encontrado no banco de dados.");
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
