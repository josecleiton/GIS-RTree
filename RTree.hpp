#ifndef RTREE_HPP
#define RTREE_HPP
#include "stdlib.hpp"
#include "SpatialData.hpp"
using namespace SpatialData;
namespace SpatialIndex{
/*
 * Forma geométrica no disco
 * Não faço ideia de como implementar essa classe
 */

class Dado{

};
/*
class RTree{

    enum{
        ORDEM = 4,
        MINNODES = ORDEM/2,
        MAXNODES = ORDEM-1,
    };

    struct Node;

    struct Branch{
        Quadrilatero MBR;
        union{
            Node* ChildPointer;
            // DADO m_dado;
        };
    };

    struct Node{
        int Nivel;
        int Count;
        vector<Branch> m_Branch;
    };
};
*/
}
#endif // RTREE_HPP
