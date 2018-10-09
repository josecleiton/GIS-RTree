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

    // TESTE DA API
    DiskAPI::Disk io(FILENAME);
    streampos k(0);
    DiskAPI::Registro* R = io.Read(k);
    Poligono* P = reinterpret_cast<Poligono*>(R->Conversao());
    cout << "Tamanho do poligono em disco: " << P->GetSize() << endl;
    cout << "Vértices do poligono: " << endl;
    for(unsigned i=0; i<P->GetSize(); i++, P->Avancar(HORARIO))
        cout << P->GetPonto() << endl;
    Ponto x(2,0), j(10,5), l(3,4);
    Retangulo test(x, j);
    cout << test.Contem(l) << endl;
    cout << endl << "Sa porra roda! " << endl;

    return a.exec();
}
