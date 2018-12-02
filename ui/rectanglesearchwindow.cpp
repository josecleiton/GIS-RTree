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
    this->setWindowIcon(QIcon(ICON));
}

RectangleSearchWindow::~RectangleSearchWindow()
{
    delete ui->p1x->validator();
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
        DiskAPI::Registro** Reg = new DiskAPI::Registro*[1];
        Reg[0] = io.Read(K.Dado);
        if(Reg[0] != nullptr){
            if(!Interseccao()){
                FindWindow FW;
                FW.setModal(true);
                FW.setRegistros(Reg, 1, false);
                FW.exec();
                if(FW.getRemove()){
                    if(!root.Remove(caminho))
                        io.Remove(K.Dado);
                    else{
                        root.ApagarArvore();
                        io.RemoveAll();
                        io.CleanDir(H_FILENAME, "id_*");
                        this->nuke = true;
                    }
                }
                delete Reg[0];
                delete[] Reg;
            }
            this->listaRegistros = Reg;
            SpatialIndex::Kai(caminho);
        }
        else{
            QMessageBox erro;
            erro.critical(nullptr, "ERRO CRITICO DE ORGANIZAÇÃO", "Fatal error: os dados necessitam de reorganização.");
            this->close();
        }
    }
    else{
        //NÃO ENCONTRADO
        QMessageBox mbox;
        mbox.critical(nullptr, "Erro", "Retângulo não encontrado na R-Tree.");
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

bool RectangleSearchWindow::GetNuke(){
    return this->nuke;
}

void RectangleSearchWindow::on_cancel_clicked()
{
    close();
}
