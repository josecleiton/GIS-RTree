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

class RTree{

    enum{
        ORDEM = 4,
        MINNODES = ORDEM/2,
        MAXNODES = ORDEM,
    };

    struct Node;

    struct Branch{
        Retangulo MBR;
        union{
            Node* ChildPointer;
            size_t Dado; // Guarda o indice da forma em disco
        };
    };

    struct Node{
        int m_Nivel;
        int m_Count;
        vector<Branch> m_Branch;
    };
};
}
#endif // RTREE_HPP
