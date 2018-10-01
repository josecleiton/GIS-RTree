#ifndef DISK_HPP
#define DISK_HPP
#include <fstream>
#include "stdlib.h"
#include "spatialdata.hpp"
using namespace SpatialData;

namespace DiskAPI{

#define FILENAME "../GIS/formas.bin"

enum TiposForma{PONTO, LINHA, POLIGONAL, POLIGONO, POLIGONO_CONVEXO, POLIGONO_NAO_CONVEXO, INDEFINIDO};

class Disk{
private:
    fstream file;
public:
    Disk(string, bool);
    ~Disk();
    bool SalvarForma(char, unsigned, Vertice*); // ESCREVE SEQUENCIALMENTE NO DISCO
    bool Read(); // LE UM POR VEZ
};

} // DiskAPI


#endif // DISK_HPP
