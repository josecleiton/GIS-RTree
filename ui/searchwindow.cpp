#include "searchwindow.hpp"
#include "ui_searchwindow.h"

SearchWindow::SearchWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Buscar na R-Tree");
}

SearchWindow::~SearchWindow()
{
    delete ui;
}

void SearchWindow::on_retangulo_clicked(){
    RectangleSearchWindow recwindow;
    recwindow.setModal(true);
    recwindow.SetInterseccao(false);
    recwindow.exec();
}

void SearchWindow::on_interseccao_clicked(){
    RectangleSearchWindow RSW;
    DiskAPI::Registro* R[2];
    for(unsigned j=0; j<2; j++){
        RSW.setModal(true);
        RSW.SetInterseccao(true);
        RSW.ClearReg();
        RSW.exec();
        R[j] = RSW.GetRegistro();
        RSW.ClearForm();
    }
    QMessageBox QMB;
    FindWindow FW;
    bool active = true;
    if(R[0] == nullptr or R[1] == nullptr){
        if(R[0] != R[1])
            QMB.critical(nullptr, "Erro na busca!", "Uma das formas geométricas não foram encontradas na estrutura, faça outra busca por intersecção.");
        active = false;
    }
    if(active and R[0]->tipo == R[1]->tipo){
        unsigned char type = R[0]->tipo;
        Registro* FakeRegister = nullptr;
        if(type == POLIGONO){
            Poligono* P = reinterpret_cast<Poligono*>(R[0]->Conversao());
            Poligono* Q = reinterpret_cast<Poligono*>(R[1]->Conversao());
            Poligono* Z = P->Interseccao(*Q);
            FakeRegister = new Registro(type, Z->GetVertice(), Z->GetSize());
            if(FW.setReg(FakeRegister)){
                FW.setModal(true);
                FW.exec();
            }
            delete FakeRegister;
            delete P;
            delete Q;
            delete Z;
            R[0]->lista = R[1]->lista = nullptr;
        }
        else if(type == LINHA){
            double t;
            Ponto P;
            Aresta* A = reinterpret_cast<Aresta*>(R[0]->Conversao());
            Aresta* B = reinterpret_cast<Aresta*>(R[1]->Conversao());
            int classificacao = A->Interseccao(*B, t);
            if(classificacao == CONSECUTIVO){
                stringstream aux;
                P = A->GetPonto(t);
                aux << P;
                QMB.information(nullptr, "Sucesso!", "Os segmentos de reta se intersectam no ponto: "+QString::fromStdString(aux.str()));
            }
            else if(classificacao == COLINEAR)
                QMB.warning(nullptr, "Retas não se cruzam", "As retas são colineares.");
            else if(classificacao == PARALELA)
                QMB.warning(nullptr, "Retas não se cruzam", "As retas são paralelas.");

            delete A;
            delete B;
        }
        else if(type == PONTO){
            Ponto* P1 = reinterpret_cast<Ponto*>(R[0]->Conversao());
            Ponto* P2 = reinterpret_cast<Ponto*>(R[1]->Conversao());
            if(P1 != P2)
                QMB.warning(nullptr, "Pontos não se encontram", "Pontos não são iguais para haver intersecção.");
            else
                QMB.information(nullptr, "Pontos se encontram!", "Pontos são iguais, logo se intersectam.");
            delete P1;
            delete P2;
        }
    }
    else if(active){
        if(R[0]->tipo == POLIGONO){
            Poligono* P = reinterpret_cast<Poligono*>(R[0]->Conversao());
            if(R[1]->tipo == PONTO){// ponto no poligono
                Ponto* A = reinterpret_cast<Ponto*>(R[1]->Conversao());
                int classificacao = P->PontoNoPoligono(*A);
                if(classificacao == DENTRO or classificacao == FRONTEIRA){
                    FW.setReg(R[0]);
                    FW.setReg(R[1]);
                    FW.setModal(true);
                    FW.exec();
                }
                else
                    QMB.warning(nullptr, "Ponto não está no poligono", "Ponto se encontra fora do poligono.");
                delete A;
            }
            delete P;
            R[0]->lista = nullptr;
        }
    }



    //REGISTERS KAI
    for(unsigned j=0; j<2; j++)
        if(R[j] != nullptr)
            delete R[j];
}

void SearchWindow::on_ponto_clicked()
{
    QMessageBox QMB;
    InsertPoint ip;
    ip.setModal(true);
    ip.exec();
    Ponto P = ip.GetPonto();
    auto Lista = root.Busca(P);
    stringstream aux;
    aux << P;
    if(!Lista.empty())
        QMB.information(nullptr, "Sucesso!", QString::number(Lista.size())+" caixa(s) contêm o ponto "+QString::fromStdString(aux.str()));
    else
        QMB.critical(nullptr, "Erro", "Ponto não encontrado no BD.");
}