#ifndef RTREE_HPP
#define RTREE_HPP
#include <iostream>
#include <vector>
#include <list>
#include "spatialdata.hpp"
#define RTREE_FILE "../GIS/rtree.bin"
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
#pragma pack(push, 1)
struct Chave{
    Retangulo MBR;
    union{
        streampos ChildPtr; // Ponteiro para o proximo nó em disco
        streampos Dado; // Guarda o indice da forma em disco
    };
    Chave(Retangulo&, streampos&, bool);
    bool Contem(Ponto&);
};
#pragma pack(pop)

struct Node{
    unsigned m_Nivel;
    unsigned m_Count;
    vector<Chave> Chaves;
    Node(unsigned nivel, unsigned count, vector<Chave>&);
    bool Folha();
};

class RTree{
private:
    Node* raiz{};
    streampos posRaiz;
    size_t count;

public:
   RTree();
   void Push(Retangulo&, const streampos&);
   bool IsEmpty();
   Node* GetPtr();
   streampos& GetPos();
   list<Node*> Traversal(streampos&, Ponto&); // PERCORRE A ARVORE
   list<streampos&> Busca(Ponto&); // BUSCA UM PONTO NA ARVORE
   Node* ReadPage(streampos&);
};

}
#endif // RTREE_HPP
