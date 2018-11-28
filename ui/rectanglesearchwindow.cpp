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
}

RectangleSearchWindow::~RectangleSearchWindow()
{
    delete ui;
}

void RectangleSearchWindow::ClearForm(){
    ui->p1x->clear();
    ui->p1y->clear();
    ui->p2x->clear();
    ui->p2y->clear();
}

void RectangleSearchWindow::ClearReg(){
    this->listaRegistros = nullptr;
}

void RectangleSearchWindow::on_button_clicked()
{
    SpatialData::Ponto A(ui->p1x->text().toDouble(), ui->p1y->text().toDouble());
    SpatialData::Ponto B(ui->p2x->text().toDouble(), ui->p2y->text().toDouble());
    SpatialData::Retangulo R(A, B);
    vector<SpatialIndex::NodeAux> caminho;
    bool ChaveEncontrada = root.Busca(root.GetPtr(), R, caminho);
    if(ChaveEncontrada){
        SpatialIndex::Chave K = caminho.front().ptr->Chaves[caminho.front().index];
        SpatialData::Retangulo MBR = K.MBR;
        this->SetMBR(MBR);
        streampos pos = K.Dado;
        DiskAPI::Registro** Reg = new DiskAPI::Registro*[1];
        Reg[0] = io.Read(pos);
        if(Reg[0] != nullptr){
            if(!Interseccao()){
                FindWindow FW;
                FW.setModal(true);
                FW.setRegistros(Reg, 1);
                FW.exec();
                if(FW.GetRemove()){
                    root.Remove(caminho);
                    io.Remove(pos);
                }
                delete Reg[0];
                delete[] Reg;
            }
            this->listaRegistros = Reg;
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
        this->listaRegistros= nullptr;
    }
}

DiskAPI::Registro** RectangleSearchWindow::GetRegistro(){
    return this->listaRegistros;
}

bool RectangleSearchWindow::Interseccao(){
    return this->interseccao;
}

void RectangleSearchWindow::SetInterseccao(bool k){
    this->interseccao = k;
}

void RectangleSearchWindow::SetMBR(SpatialData::Retangulo R){
    this->MBR = R;
}

SpatialData::Retangulo RectangleSearchWindow::GetMBR(){
    return this->MBR;
}

void RectangleSearchWindow::on_cancel_clicked()
{
    close();
}
