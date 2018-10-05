#include "insertwindow.hpp"
#include "ui_insertwindow.h"
#include "rtree.hpp"
#include "spatialdata.hpp"
#include "disk.hpp"
#include "variaveis.hpp"
#include "pointinsert.hpp"

InsertWindow::InsertWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InsertWindow){
    ui->setupUi(this);
}

InsertWindow::~InsertWindow(){
    delete ui;
}

void InsertWindow::on_submit_clicked(){
    unsigned int num_vertices = ui->numVert->text().toUInt(); // NUM DE VERTICES DETERMINADO PELA GUI
    string forma = ui->tipoForma->text().toStdString();
    unsigned char tipo; // TIPO DA FORMA (POLIGONO, POLIGONAL ETC)
    Vertice* pontos = nullptr;

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
    else
        tipo = DiskAPI::INDEFINIDO;

    // PEGAR OS VERTICES POR UM MÉTODO USANDO GUI
    // CRIAR A LISTA DE VERTICES
    // RESOLUÇÃO ABAIXO:
    PointInsert VerticeWindow;
    VerticeWindow.setModal(true);
    for(unsigned i=0; i<num_vertices; i++){ // FOR PARA PEGAR TODOS OS PONTOS INSERIDOS PELO USUÁRIO VIA GUI
        VerticeWindow.exec();
        if(pontos == nullptr){
            Ponto temp = VerticeWindow.GetPonto();
            pontos = new Vertice(temp);
        }
        else{
            Vertice* temp = VerticeWindow.GetVertice();
            pontos->Push(temp);
        }
        VerticeWindow.close();
    }

    DiskAPI::Disk io(FILENAME, !root.isEmpty()); // UMA API VAGABUNDA PARA AJUDAR NOS ACESSOS AO DISCO
    streampos posicao_forma = io.SalvarForma(tipo, num_vertices, pontos); // posicao da forma
    Retangulo MBR = pontos->Envelope(); //MBR QUE ENVELOPA A FORMA
    root.Push(MBR, posicao_forma); // FUNÇÃO NA ARVORE PARA INSERIR O MBR E A POSIÇÃO EM DISCO (TEM QUE SER IMPLEMENTADA)

    close(); // termina a janela
}
