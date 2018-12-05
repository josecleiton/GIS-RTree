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
    fstream file; // CLASSE QUE MANTEM O ARQUIVO COM OS OBJETOS SPATIALDATA
public:
    Disk(string, bool append);
    Disk(string);
    ~Disk();
    streampos Salvar(unsigned char, unsigned&, Vertice*); // ESCREVE SEQUENCIALMENTE NO DISCO
    Registro* Read(streampos); // LE UM POR VEZ
    streampos Salvar(Circulo&);
    void Remove(streampos&); // REMOVE LOGICAMENTE UM REGISTRO
    void Verifica(); // VERIFICA SE O ARQUIVO FOI ABERTO
    bool RemoveAll(); // REMOVE TODOS OS REGISTROS FISICAMENTE
    void CleanDir(QString path, QString rule); // LIMPA UM DIRETORIO A PARTIR DE UMA REGRA
};

} // DiskAPI


#endif // DISK_HPP
