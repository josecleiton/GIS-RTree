#include "mainwindow.hpp"
#include <QApplication>
#include "stdlib.hpp"
#include "SpatialData.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    SpatialData::Poligono P, Q;
    P.Push(SpatialData::Ponto(10,5));
    P.Push(SpatialData::Ponto(5,2));
    P.Push(SpatialData::Ponto(2,10));

    Q.Push(SpatialData::Ponto(12,2));
    Q.Push(SpatialData::Ponto(15,2));
    Q.Push(SpatialData::Ponto(15,10));
    Q.Push(SpatialData::Ponto(12,10));

    cout << "Triangulo = " << P.GetSize() << endl;
    cout << "Quadrilatero = " << Q.GetSize() << endl;
    cout << "Sa porra roda! POGCHAMP! " << endl;
    return a.exec();
}
