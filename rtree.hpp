#ifndef RTREE_HPP
#define RTREE_HPP
#include "stdlib.hpp"
#include "spatialdata.hpp"
using namespace SpatialData;
namespace SpatialIndex{
/*
 * Forma geométrica no disco
 * Não faço ideia de como implementar essa classe
 */


enum{
    ORDEM = 4,
    MINNODES = ORDEM/2,
    MAXNODES = ORDEM,
};

struct Node;

struct Chave{
    Retangulo MBR;
    union{
        Node* ChildPointer;
        size_t Dado; // Guarda o indice da forma em disco
    };
    Chave();
};

struct Node{
    int m_Nivel;
    int m_Count;
    vector<Chave> mchaves;
    Node();
};

class RTree{
private:
    Node* raiz;

public:
   RTree();
};

}
#endif // RTREE_HPP
