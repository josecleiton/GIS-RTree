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
    SpatialIndex::Node* test = new SpatialIndex::Node(root.GetPtr()->DiskPos);
    cout << endl << "Sa porra roda! " << endl;
    delete test;
    return a.exec();
}
