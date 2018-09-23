#include "mainwindow.hpp"
#include <QApplication>
#include "stdlib.hpp"
#include "SpatialData.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    SpatialData::Ponto PontoX1(1,1);
    SpatialData::Ponto PontoX2(5,1);
    SpatialData::Ponto PontoY1(1,10);
    SpatialData::Ponto PontoY2(5,10);
    SpatialData::Retangulo Ret(PontoX1, PontoX2, PontoY1, PontoY2);
    cout << "Sa porra roda! POGCHAMP! " << endl;
    return a.exec();
}
