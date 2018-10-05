#ifndef RTREE_HPP
#define RTREE_HPP
#include <iostream>
#include <vector>
#include "spatialdata.hpp"
using namespace std;
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
        Node* ChildPointer{};
        size_t Dado; // Guarda o indice da forma em disco
    };
    Chave(Retangulo&, size_t);
    Chave(Retangulo&, Node*);
};

struct Node{
    int m_Nivel;
    int m_Count;
    vector<Chave> mchaves;
    Node(int nivel=0, int count=0);
};

class RTree{
private:
    Node* raiz;

public:
   RTree();
   void Push();
};

}
#endif // RTREE_HPP
