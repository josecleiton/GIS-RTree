#ifndef DISK_HPP
#define DISK_HPP
#include <iostream>
#include <fstream>
#include <QDirIterator>
#include "spatialdata.hpp"
#define FILENAME "../GIS/spatialdata/formas.bin"

using namespace std;
using namespace SpatialData;

namespace DiskAPI{


enum TiposForma{PONTO, LINHA, POLIGONAL, POLIGONO, POLIGONO_NAO_CONVEXO, INDEFINIDO, CIRCULO};

struct Registro{
    unsigned char tipo;
    Vertice* lista{};
    unsigned tam{};
    Registro(unsigned char type = INDEFINIDO, Vertice* v = nullptr, unsigned t = 0);
    ~Registro();
    void* Conversao();
    string StrTipo();
};

class Disk{
private:
    fstream file;
public:
    Disk(string, bool);
    Disk(string);
    ~Disk();
    streampos Salvar(unsigned char, unsigned&, Vertice*); // ESCREVE SEQUENCIALMENTE NO DISCO
    Registro* Read(streampos); // LE UM POR VEZ
    streampos Salvar(Circulo&);
    void Remove(streampos&);
    void Verifica(); // VERIFICA SE O ARQUIVO FOI ABERTO
    bool RemoveAll();
    void CleanDir(QString path, QString rule);
};

} // DiskAPI


#endif // DISK_HPP
