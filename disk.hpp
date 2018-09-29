#ifndef DISK_HPP
#define DISK_HPP
#include <fstream>
#include "stdlib.h"
#include "SpatialData.hpp"
using namespace SpatialData;

namespace DiskAPI{

#define FILENAME "../GIS/formas.bin"

enum TiposForma{PONTO, LINHA, POLIGONAL, POLIGONO, POLIGONO_CONVEXO, POLIGONO_NAO_CONVEXO, INDEFINIDO};

#pragma pack(push, 1)
struct VerticeArray{
    double x, y;
};

struct Registro{
    char tipo;
    unsigned m_size;
    struct VerticeArray *v{};
    char fim[9] = "endfield";
};
#pragma pack(pop)

class Disk{
private:
    fstream file;
public:
    Disk(string);
    ~Disk();
    bool Save(char, unsigned, Vertice*); // ESCREVE SEQUENCIALMENTE NO DISCO
    bool Read(); // LE UM POR VEZ
};

} // DiskAPI


#endif // DISK_HPP
