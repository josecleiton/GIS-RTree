#ifndef DISK_HPP
#define DISK_HPP
#include <iostream>
#include <fstream>
#include "spatialdata.hpp"
#define FILENAME "../GIS/formas.bin"

using namespace std;
using namespace SpatialData;

namespace DiskAPI{


enum TiposForma{PONTO, LINHA, POLIGONAL, POLIGONO, POLIGONO_NAO_CONVEXO, INDEFINIDO};

struct Registro{
    unsigned char tipo;
    Vertice* lista{};
    Registro(unsigned char, Vertice*);
};

class Disk{
private:
    fstream file{};
public:
    Disk(string, bool);
    Disk(string);
    ~Disk();
    streampos SalvarForma(unsigned char&, unsigned&, Vertice*); // ESCREVE SEQUENCIALMENTE NO DISCO
    Registro* Read(streampos&); // LE UM POR VEZ
    void Verifica(); // VERIFICA SE O ARQUIVO FOI ABERTO
};

} // DiskAPI


#endif // DISK_HPP
