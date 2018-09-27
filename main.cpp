#include "mainwindow.hpp"
#include <QApplication>
#include "stdlib.hpp"
#include "SpatialData.hpp"

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

    auto V = SpatialData::Triangulacao(Q1);
    double Area = SpatialData::AreaNgono(V);

    cout << "Triangulo = " << P.GetSize() << " lados" << endl;
    cout << "Quadrilatero = " << Q.GetSize() << " lados" << endl;
    cout << "Triangulos gerados pela triangulação do Quadrilátero = " << V.size() << endl;
    cout << "Area do Quadrilátero = " << Area << endl;
    cout << "Sa porra roda! " << endl;
    return a.exec();
}
