#include "insertwindow.hpp"
#include "ui_insertwindow.h"
#include "rtree.hpp"
#include "spatialdata.hpp"
#include "disk.hpp"
#include "variaveis.hpp"
#include "insertpointwindow.hpp"
#include "insertcirclewindow.hpp"


InsertWindow::InsertWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InsertWindow){
    ui->setupUi(this);
    QRegExp inteiro(INTEGER_REGEX);
    QRegExp palavra(WORD_REGEX);
    QRegExp nome(NAME_REGEX);
    QRegExpValidator *ValidaInt = new QRegExpValidator(inteiro, this);
    QRegExpValidator *ValidaPalavra = new QRegExpValidator(palavra, this);
    //QRegExpValidator *ValidaNome = new QRegExpValidator(nome, this);
    ui->numVert->setValidator(ValidaInt);
    ui->tipoForma->setValidator(ValidaPalavra);
    //ui->ID->setValidator(ValidaNome);
    this->setWindowTitle("Insira uma forma geométrica");
}

InsertWindow::~InsertWindow(){
    delete ui;
}

void InsertWindow::on_submit_clicked(){
    Inserir();
    close(); // termina a janela
}

void InsertWindow::Inserir(){
    QMessageBox MB;
    unsigned int num_vertices = ui->numVert->text().toUInt(); // NUM DE VERTICES DETERMINADO PELA GUI
    //QString identificador = ui->ID->text();
    if(!num_vertices)
        MB.critical(nullptr, "Entrada incorreta!", "Não é possível inserir uma forma sem pontos.");
    else{
        string forma = ui->tipoForma->text().toStdString();
        unsigned char tipo; // TIPO DA FORMA (POLIGONO, POLIGONAL ETC)
        Retangulo MBR;
        streampos posicao_forma;
        bool excep = false;

        for(auto letra = forma.begin(); letra != forma.end(); letra++) // DEIXAR AS LETRAS MAIUSCULAS PARA A COMPARAÇÃO
            *letra = static_cast<char>(toupper(*letra));

        if(forma == "POLIGONO")
            tipo = DiskAPI::POLIGONO;
        else if(forma == "PONTO")
            tipo = DiskAPI::PONTO;
        else if(forma == "LINHA")
            tipo = DiskAPI::LINHA;
        else if(forma == "POLIGONAL")
            tipo = DiskAPI::POLIGONAL;
        else if(forma == "POLIGONO NAO CONVEXO")
            tipo = DiskAPI::POLIGONO_NAO_CONVEXO;
        else if(forma == "CIRCULO")
            tipo = DiskAPI::CIRCULO;
        else
            tipo = DiskAPI::INDEFINIDO;

        // PEGAR OS VERTICES POR UM MÉTODO USANDO GUI
        // CRIAR A LISTA DE VERTICES
        // RESOLUÇÃO ABAIXO:
        if(tipo != DiskAPI::CIRCULO){
            Vertice* pontos = nullptr;
            InsertPoint VerticeWindow;
            VerticeWindow.setModal(true);
            for(unsigned i=0; i<num_vertices; i++){ // FOR PARA PEGAR TODOS OS PONTOS INSERIDOS PELO USUÁRIO VIA GUI
                VerticeWindow.exec();
                if(pontos == nullptr){
                    Ponto* temp = VerticeWindow.GetPonto();
                    if(temp != nullptr){
                        pontos = new Vertice(*temp);
                        delete temp;
                    }
                    else{
                        excep = true;
                        break;
                    }
                }
                else{
                    Vertice* temp = VerticeWindow.GetVertice();
                    if(temp != nullptr)
                        pontos->Push(temp);
                    else{
                        excep = true;
                        break;
                    }
                }
                VerticeWindow.close();
            }
            if(!excep){
                posicao_forma = io.Salvar(tipo, num_vertices, pontos); // posicao da forma
                MBR = pontos->Envelope(); //MBR QUE ENVELOPA A FORMA
            }
            else
                MB.critical(nullptr, "Erro de entrada", "Não foi inserido ponto.");
            pontos->Kai();
        }
        else{
            InsertCircleWindow CircleWindow;
            CircleWindow.setModal(true);
            CircleWindow.exec();
            Circulo C = CircleWindow.GetInfo();
            MBR = C.Envelope();
            posicao_forma = io.Salvar(C);
        }
        if(!excep){
            root.Inserir(MBR, posicao_forma);  // FUNÇÃO NA ARVORE PARA INSERIR O MBR E A POSIÇÃO EM DISCO (TEM QUE SER IMPLEMENTADA)
            MB.information(nullptr, "Sucesso", "Forma inserida no banco de dados.");
        }
    }
}

void InsertWindow::on_cancel_clicked()
{
    InserirConjunto();
    close();
}

void InsertWindow::InserirConjunto(){
    QString identificador = ui->ID->text();
    if(identificador.size()){
        fstream file(identificador.toStdString(), fstream::in);
        if(file.is_open()){
            unsigned numvert = 0;
            char del = ',';
            Vertice* pontos = nullptr;
            double x, y;
            streampos pos;
            Retangulo MBR;
            string handle;
            Ponto P;
            while(!file.eof()){
                file >> numvert;
                for(unsigned i=0; i<numvert+1; i++){
                    file >> del >> x >> del >> y;
                    P.x = x;
                    P.y = y;
                    if(pontos != nullptr)
                        pontos->Push(P);
                    else
                        pontos = new Vertice(x, y);
                }
                getline(file, handle);
                pos = io.Salvar(DiskAPI::POLIGONO_NAO_CONVEXO, numvert, pontos);
                MBR = pontos->Envelope();
                pontos->Kai();
                pontos = nullptr;
                root.Inserir(MBR, pos);
            }
            file.close();
        }
    }
}
