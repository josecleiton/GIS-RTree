#ifndef DISK_HPP
#define DISK_HPP
#include <iostream>
#include <fstream>
#include "spatialdata.hpp"

using namespace std;
using namespace SpatialData;

namespace DiskAPI{

#define FILENAME "../GIS/formas.bin"

enum TiposForma{PONTO, LINHA, POLIGONAL, POLIGONO, POLIGONO_NAO_CONVEXO, INDEFINIDO};

class Disk{
private:
    fstream file{};
public:
    Disk(string, bool);
    ~Disk();
    bool SalvarForma(unsigned char&, unsigned&, Vertice*); // ESCREVE SEQUENCIALMENTE NO DISCO
    bool Read(); // LE UM POR VEZ
    streamoff GetTell();
};

} // DiskAPI


#endif // DISK_HPP
