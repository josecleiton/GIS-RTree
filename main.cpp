#include "mainwindow.hpp"
#include <QApplication>
#include "stdlib.hpp"
#include "SpatialData.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    vector<SpatialData::Linha> V, V2;
    V.push_back(SpatialData::Linha(SpatialData::Ponto(5,2), SpatialData::Ponto(10,5)));
    V.push_back(SpatialData::Linha(SpatialData::Ponto(10,5), SpatialData::Ponto(2,10)));
    V.push_back(SpatialData::Linha(SpatialData::Ponto(2,10), SpatialData::Ponto(5,2)));
    SpatialData::Triangulo T(V);

    V2.push_back(SpatialData::Linha(SpatialData::Ponto(12,2), SpatialData::Ponto(15,2)));
    V2.push_back(SpatialData::Linha(SpatialData::Ponto(15,2), SpatialData::Ponto(15,10)));
    V2.push_back(SpatialData::Linha(SpatialData::Ponto(15,10), SpatialData::Ponto(12,10)));
    V2.push_back(SpatialData::Linha(SpatialData::Ponto(12,10), SpatialData::Ponto(12,2)));
    SpatialData::Quadrilatero Q(V2);

    cout << "Numero de arestas de um triangulo: " << T.GetArestas() << endl;
    cout << "Perimetro do triangulo: " << T.GetPerimetro() << endl;
    cout << "Area do triangulo: " << T.GetArea() << endl << endl;
    cout << "Numero de arestas de um quadrilatero: " << Q.GetArestas() << endl;
    cout << "Perimetro do quadrilatero: " << Q.GetPerimetro() << endl;
    cout << "Area do quadrilatero: " << Q.GetArea() << endl;
    cout << "Sa porra roda! POGCHAMP! " << endl;

    return a.exec();
}
