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
    DiskAPI::Registro **R = new DiskAPI::Registro*[2], **temp = nullptr;
    R[0] = R[1] = nullptr;
    SpatialData::Retangulo MBR[2];
    for(unsigned j=0; j<2; j++){
        RSW.setModal(true);
        RSW.SetInterseccao(true);
        RSW.ClearReg();
        RSW.exec();
        temp = RSW.GetRegistro();
        if(temp != nullptr){
            R[j] = temp[0];
            MBR[j] = RSW.GetMBR();
            delete[] temp;
        }
        else break;
        RSW.ClearForm();
    }
    QMessageBox QMB;
    FindWindow FW;
    FW.setModal(true);
    bool active = true;
    if(R[0] != nullptr and R[1] != nullptr){
        if(active and MBR[0].Interseccao(MBR[1])){ // TESTA SE HÁ INTERSECÇÃO ENTRE OS RETANGULOS
            if(R[0]->tipo == R[1]->tipo){
                unsigned char type = R[0]->tipo;
                Registro* FakeRegister = nullptr;
                Registro** FakeArrayRegister = new Registro*[1];
                FakeArrayRegister[0] = nullptr;
                if(type == POLIGONO){
                    Poligono* P = reinterpret_cast<Poligono*>(R[0]->Conversao());
                    Poligono* Q = reinterpret_cast<Poligono*>(R[1]->Conversao());
                    Poligono* Z = P->Interseccao(*Q);
                    FakeRegister = new Registro(type, Z->GetVertice(), Z->GetSize());
                    FakeArrayRegister[0] = FakeRegister;
                    if(FW.setRegistros(FakeArrayRegister, 1, true)){
                        FW.exec();
                    }
                    delete FakeRegister;
                    delete P;
                    delete Q;
                    Z->setFakeKai(true); // FAKE KAI É NECESSÁRIO PORQUE O DESTRUCTOR DO REGISTRO
                    delete Z; //            JÁ LIBEROU A LISTA DE VERTICES
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
                else if(type == CIRCULO){
                    Circulo* A = reinterpret_cast<Circulo*>(R[0]->Conversao());
                    Circulo* B = reinterpret_cast<Circulo*>(R[1]->Conversao());
                    pair<Vertice*, unsigned> lista = A->PontinterCirculo(*A, *B);
                    if(lista.first != nullptr)
                        FakeArrayRegister[0] = new Registro(INDEFINIDO, lista.first, lista.second);
                    if(FW.setRegistros(FakeArrayRegister, 1, true))
                        FW.exec();
                    if(lista.first != nullptr) delete FakeArrayRegister[0];
                }
                delete[] FakeArrayRegister;
            }
            else{
                if(R[0]->tipo == POLIGONO or R[1]->tipo == POLIGONO){
                    int pol = (R[0]->tipo == POLIGONO)?0:1;
                    Poligono* P = reinterpret_cast<Poligono*>(R[pol]->Conversao());
                    if(R[!pol]->tipo == PONTO){// ponto no poligono
                        Ponto* A = reinterpret_cast<Ponto*>(R[!pol]->Conversao());
                        int classificacao = P->PontoNoPoligono(*A);
                        if(classificacao == DENTRO or classificacao == FRONTEIRA){
                            FW.setRegistros(R, 2, false);
                            FW.exec();
                        }
                        else
                            QMB.warning(nullptr, "Ponto não está no poligono", "Ponto se encontra fora do poligono.");
                        delete A;
                    }
                    delete P;
                    R[pol]->lista = nullptr;
                }
                if(R[0]->tipo == CIRCULO or R[1]->tipo == CIRCULO){
                    int cic = (R[0]->tipo == CIRCULO)?0:1;
                    Circulo* C = reinterpret_cast<Circulo*>(R[cic]->Conversao());
                    if(R[!cic]->tipo == LINHA){
                        Aresta* A = reinterpret_cast<Aresta*>(R[!cic]->Conversao());
                        pair<Vertice*, unsigned> lista = C->CirculoIntRetas(*C, A->GetOrigem(), A->GetDestino());
                        Registro** FakeArrayRegister = new Registro*[1];
                        if(lista.first != nullptr)
                            FakeArrayRegister[0] = new Registro(INDEFINIDO, lista.first, lista.second);
                        if(FW.setRegistros(FakeArrayRegister, 1, true))
                            FW.exec();
                        if(lista.first != nullptr) delete FakeArrayRegister[0];
                        delete[] FakeArrayRegister;
                        delete A;
                    }
                }
            }
        } // INTERSECÇÃO ENTRE RETANGULOS
        else if(active)
            QMB.warning(nullptr,"Intersecção entre MBRs", "Não há sobreposição dos MBR, logo não há intersecção entre as formas geométricas.");
    }
    else{
        QMB.critical(nullptr, "Erro na busca!", "Uma das formas geométricas não foram encontradas na estrutura, faça outra busca por intersecção.");
        active = false;
    }
    //REGISTERS KAI
    for(unsigned j=0; j<2; j++)
        if(R[j] != nullptr)
            delete R[j];
    delete[] R;
}

void SearchWindow::on_ponto_clicked()
{
    QMessageBox QMB;
    InsertPoint ip;
    ip.setModal(true);
    ip.exec();
    Ponto *P = ip.GetPonto();
    if(P != nullptr){
        auto Lista = root.Busca(*P);
        stringstream aux;
        aux << *P;
        if(!Lista.empty())
            QMB.information(nullptr, "Sucesso!", QString::number(Lista.size())+" caixa(s) contêm o ponto "+QString::fromStdString(aux.str()));
        else
            QMB.critical(nullptr, "Erro", "Ponto não encontrado no BD.");
        delete P;
    }
    else
        QMB.critical(nullptr, "Erro de entrada", "Insira o ponto.");
}

void SearchWindow::on_id_clicked()
{
    string query;
    StringWindow Aux;
    Aux.setModal(true);
    Aux.exec();
    query = Aux.getStr();
    DiskAPI::Hash PH;
    vector<streampos>* result = PH.SelectAll(query);
    size_t sizeResult = result->size();
    QMessageBox QMB;
    if(sizeResult){
        QMB.information(nullptr, "Formas encontradas!", "Foram encontradas "+QString::fromStdString(to_string(sizeResult))+" forma(s) do tipo: "+QString::fromStdString(query)+".");
        DiskAPI::Registro** ArrayReg = new Registro* [sizeResult];
        for(size_t i=0; i<sizeResult; i++)
            ArrayReg[i] = io.Read((*result)[i]);
        FindWindow FW;
        FW.setModal(true);
        FW.setRegistros(ArrayReg, sizeResult, false);
        FW.exec();
        for(size_t i=0; i<sizeResult; i++)
            delete ArrayReg[i];
        delete[] ArrayReg;
    }
    else
        QMB.critical(nullptr, "Não há formas com esse ID", "Tente novamente, por favor.");
}
