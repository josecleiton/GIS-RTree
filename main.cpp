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
    cout << root.isEmpty() << endl;

    // TESTE DA API
    DiskAPI::Disk io(FILENAME);
    streampos k(0);
    DiskAPI::Registro* R = io.Read(k);
    Poligono* P = reinterpret_cast<Poligono*>(R->Conversao());
    cout << "Tamanho do poligono em disco: " << P->GetSize() << endl;

    cout << "Sa porra roda! " << endl;

    return a.exec();
}
