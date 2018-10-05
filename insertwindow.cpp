#include "insertwindow.hpp"
#include "ui_insertwindow.h"
#include "rtree.hpp"
#include "spatialdata.hpp"
#include "disk.hpp"
#include "variaveis.hpp"

InsertWindow::InsertWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InsertWindow)
{
    ui->setupUi(this);
}

InsertWindow::~InsertWindow()
{
    delete ui;
}

void InsertWindow::on_submit_clicked()
{
    unsigned int num_vertices = ui->numVert->text().toUInt();
    string forma = ui->tipoForma->text().toStdString();
    unsigned char tipo;
    Vertice* pontos = nullptr;
    for(auto letra: forma)
        letra = static_cast<char>(toupper(letra));

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

    // PEGAR OS VERTICES POR UM MÉTODO
    // CRIAR A LISTA DE VERTICES

    DiskAPI::Disk io(FILENAME, true);
    streamoff posicao_forma = io.GetTell(); // posicao da proxima forma no disco.
    io.SalvarForma(tipo, num_vertices, pontos);
    Retangulo MBR = pontos->Envelope();
    root.Push(MBR, posicao_forma);

    //string id = ui->ID->text().toStdString();
    //fstream saida(INSERT_FORM, ios::out);
    /*if(saida.is_open()){
        for(auto it: resultado)
            saida << it << endl;
        saida.close();
    }*/

    close(); // termina a janela
}
