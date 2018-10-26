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
    stack<SpatialIndex::NodeAux>* Pilha = root.Busca(R);
    if(Pilha->empty() or (!Pilha->empty() and Pilha->top().ptr == nullptr)){
        //NÃO ENCONTRADO
        QMessageBox mbox;
        mbox.critical(nullptr, "Erro", "Retângulo não encontrado no banco de dados.");
        mbox.setFixedSize(500,200);
    }
    else{
        SpatialIndex::NodeAux No = Pilha->top();
        Pilha->pop();
        DiskAPI::Disk io(FILENAME);
        streampos pos = No.ptr->Chaves[No.index].Dado;
        DiskAPI::Registro* Reg = io.Read(pos);
        this->reg = Reg;
        if(interseccaoBool){
            FindWindow FW;
            FW.setModal(true);
            FW.setReg(Reg);
            FW.exec();
            if(FW.GetRemove()){
                io.Remove(pos);
                //root.Remove(pos, *Pilha);
            }
            delete Reg;
        }
    }
    close();
}

DiskAPI::Registro* RectangleSearchWindow::GetRegistro(){
    return this->reg;
}
