#include "mainwindow.hpp"
#include <QApplication>
#include <iostream>
#include "variaveis.hpp"
#include "rtree.hpp"
#include "spatialdata.hpp"
#include "disk.hpp"
using namespace std;

SpatialIndex::RTree root;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    SpatialData::Poligono P, Q, Q1;
    P.Push(SpatialData::Ponto(10,5));
    P.Push(SpatialData::Ponto(5,2));
    P.Push(SpatialData::Ponto(2,10));

    Q.Push(SpatialData::Ponto(12,2));
    Q.Push(SpatialData::Ponto(15,2));
    Q.Push(SpatialData::Ponto(15,10));
    Q.Push(SpatialData::Ponto(12,10));

    Q1.Push(SpatialData::Ponto(2,3));
    Q1.Push(SpatialData::Ponto(8,8));
    Q1.Push(SpatialData::Ponto(7,12));
    Q1.Push(SpatialData::Ponto(1,8));
    double Q1Area = SpatialData::Area(Q1);
    Retangulo MBRQ1 = Q1.GetVertice()->Envelope();
    //auto kek = SpatialData::Triangulacao(Q1);
    cout << "Triangulo = " << P.GetSize() << " lados" << endl;
    cout << "Quadrilatero = " << Q.GetSize() << " lados" << endl;
    cout << "Area do Poligono = " << Q1Area << endl;
    cout << "Sa porra roda! " << endl;

    DiskAPI::Disk io(FILENAME, false);
    io.SalvarForma(DiskAPI::POLIGONO_CONVEXO, Q.GetSize(), Q.GetVertice());

    return a.exec();
}
