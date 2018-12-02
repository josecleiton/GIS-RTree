#include "searchwindow.hpp"
#include "ui_searchwindow.h"

SearchWindow::SearchWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Buscar na R-Tree");
    this->setWindowIcon(QIcon(ICON));
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
    if(recwindow.GetNuke()) this->close();
}

void SearchWindow::on_interseccao_clicked(){
    RectangleSearchWindow RSW;
    DiskAPI::Registro **R = new DiskAPI::Registro*[2], **temp = nullptr;
    memset(R, 0, sizeof(DiskAPI::Registro*)*2);
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
            Registro** FakeArrayRegister = new Registro*[1];
            FakeArrayRegister[0] = nullptr;
            unsigned char ndtype;
            if(R[0]->tipo == R[1]->tipo){
                ndtype = R[0]->tipo;
                if(ndtype == POLIGONO){
                    Poligono* P = reinterpret_cast<Poligono*>(R[0]->Conversao());
                    Poligono* Q = reinterpret_cast<Poligono*>(R[1]->Conversao());
                    Poligono* Z = P->Interseccao(*Q);
                    FakeArrayRegister[0] = new Registro(ndtype, Z->GetVertice(), Z->GetSize());
                    if(FW.setRegistros(FakeArrayRegister, 1, true)){
                        FW.exec();
                    }
                    if(FakeArrayRegister[0] != nullptr)
                        delete FakeArrayRegister[0];
                    delete P;
                    delete Q;
                    Z->setFakeKai(true); // FAKE KAI É NECESSÁRIO PORQUE O DESTRUCTOR DO REGISTRO
                    delete Z; //            JÁ LIBEROU A LISTA DE VERTICES
                    R[0]->lista = R[1]->lista = nullptr;
                }
                else if(ndtype == LINHA){
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
                else if(ndtype == PONTO){
                    Ponto* P1 = reinterpret_cast<Ponto*>(R[0]->Conversao());
                    Ponto* P2 = reinterpret_cast<Ponto*>(R[1]->Conversao());
                    if(P1 != P2)
                        QMB.warning(nullptr, "Pontos não se encontram", "Pontos não são iguais para haver intersecção.");
                    else
                        QMB.information(nullptr, "Pontos se encontram!", "Pontos são iguais, logo se intersectam.");
                    delete P1;
                    delete P2;
                }
                else if(ndtype == CIRCULO){
                    Circulo* A = reinterpret_cast<Circulo*>(R[0]->Conversao());
                    Circulo* B = reinterpret_cast<Circulo*>(R[1]->Conversao());
                    pair<Vertice*, unsigned> lista = A->Interseccao(*B);
                    if(lista.first != nullptr)
                        FakeArrayRegister[0] = new Registro(INDEFINIDO, lista.first, lista.second);
                    if(FW.setRegistros(FakeArrayRegister, 1, true))
                        FW.exec();
                    if(FakeArrayRegister[0] != nullptr) delete FakeArrayRegister[0];
                    delete A;
                    delete B;
                }
            }
            else{
                if(R[0]->tipo == POLIGONO or R[1]->tipo == POLIGONO){ // POLIGONO
                    int pol = (R[0]->tipo == POLIGONO)?0:1;
                    Poligono* P = reinterpret_cast<Poligono*>(R[pol]->Conversao());
                    ndtype = R[!pol]->tipo;
                    if(ndtype == PONTO){// ponto no poligono
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
                } // POLIGONO
                else if(R[0]->tipo == CIRCULO or R[1]->tipo == CIRCULO){ // CIRCULO
                    int cic = (R[0]->tipo == CIRCULO)?0:1;
                    Circulo* C = reinterpret_cast<Circulo*>(R[cic]->Conversao());
                    ndtype = R[!cic]->tipo;
                    if(ndtype == LINHA){
                        Aresta* A = reinterpret_cast<Aresta*>(R[!cic]->Conversao());
                        pair<Vertice*, unsigned> lista = C->Interseccao(*A);
                        if(lista.first != nullptr)
                            FakeArrayRegister[0] = new Registro(INDEFINIDO, lista.first, lista.second);
                        if(FW.setRegistros(FakeArrayRegister, 1, true))
                            FW.exec();
                        if(lista.first != nullptr) delete FakeArrayRegister[0];
                        delete A;
                    }
                    else if(ndtype == PONTO){
                        Ponto* P = reinterpret_cast<Ponto*>(R[!cic]->Conversao());
                        int classificacao = C->Interseccao(*P);
                        if(classificacao != FORA){
                            QString msg = "Ponto está: ";
                            msg += (classificacao == DENTRO)?"DENTRO":"NA BORDA";
                            msg += " da circunferência.\n";
                            QMB.information(nullptr, "Ponto", msg);
                            if(classificacao == DENTRO){
                                FW.setRegistros(R, 2, false);
                                FW.setInterCircle(true);
                                FW.exec();
                            }
                            else{
                                Vertice* FakeList = new Vertice(*P);
                                FakeArrayRegister[0] = new Registro(PONTO, FakeList, 1);
                                FW.setRegistros(FakeArrayRegister, 1, true);
                                FW.exec();
                                FakeArrayRegister[0]->tam = 0;
                                delete FakeList;
                                delete FakeArrayRegister[0];
                            }
                        }
                        else QMB.critical(nullptr, "Ponto", "Ponto está fora da circunferência");
                        delete P;
                    }

                    delete C;
                } // CIRCULO
                else if(R[0]->tipo == LINHA or R[1]->tipo == LINHA){ // linha
                    int line = (R[0]->tipo == LINHA)?0:1;
                    Aresta* A = reinterpret_cast<Aresta*>(R[line]->Conversao());
                    ndtype = R[!line]->tipo;
                    if(ndtype == PONTO){
                        Ponto* P = reinterpret_cast<Ponto*>(R[!line]->Conversao());
                        int clasf = P->Classificacao(*A);
                        bool inter = (clasf == ENTRE or clasf == ORIGEM or clasf == DESTINO) ? true : false;
                        QString result;
                        result = "Ponto não está na reta!\n";
                        if(inter){
                            result.replace(6, 3, "");
                            stringstream aux;
                            aux << *P;
                            result += "Coordenada do ponto: " + QString::fromStdString(aux.str())+"\n";
                        }
                        QMB.information(nullptr, "Intersecção", result);
                        delete P;
                    }
                    delete A;
                } // linha
            }
            delete[] FakeArrayRegister;
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
            QMB.critical(nullptr, "Erro", "Ponto não encontrado na R-Tree.");
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
    SpatialIndex::Hash PH;
    vector<streampos>* result = PH.SelectAll(query);
    size_t sizeResult = result->size();
    QMessageBox QMB;
    unsigned ativos = 0;
    if(sizeResult){
        DiskAPI::Registro** ArrayReg = new Registro* [sizeResult];
        for(size_t i=0; i<sizeResult; i++){
            ArrayReg[i] = io.Read((*result)[i]);
            if(ArrayReg[i]) ativos++;
        }
        if(ativos){
            QMB.information(nullptr, "Formas encontradas!", "Foram encontradas "+QString::fromStdString(to_string(ativos))+" forma(s) do tipo: "+QString::fromStdString(query)+".");
            FindWindow FW;
            FW.setModal(true);
            FW.setRegistros(ArrayReg, sizeResult, true);
            FW.exec();
            for(size_t i=0; i<sizeResult; i++)
                if(ArrayReg[i] != nullptr)
                    delete ArrayReg[i];
            delete[] ArrayReg;
            return;
        }
        delete[] ArrayReg;
    }
    QMB.critical(nullptr, "Não há formas com esse ID", "Tente novamente, por favor.");
}

void SearchWindow::on_all_clicked()
{
    //PEGA TODAS AS CHAVES E MOSTRA
    FindWindow FW;
    FW.setWindowTitle("SELECT *");
    FW.setModal(true);
    auto ListaChaves = root.EncontreAsFolhas(root.GetPtr(), false);
    DiskAPI::Registro **R = new DiskAPI::Registro*[ListaChaves.size()];
    unsigned i = 0;
    for(auto chave: ListaChaves)
        R[i++] = io.Read(chave.Dado);
    FW.setRegistros(R, ListaChaves.size(), false);
    FW.setInterCircle(true);
    FW.exec();
    if(FW.getRemove()){
        root.ApagarArvore();
        io.RemoveAll();
        io.CleanDir(H_FILENAME, "id_*");
        this->close();
    }
    for(i=0; i<ListaChaves.size(); i++)
        if(R[i] != nullptr)
            delete R[i];
    delete[] R;
}
