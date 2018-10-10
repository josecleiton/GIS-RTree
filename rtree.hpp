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
    MINCHAVES = ORDEM/2,
    MAXCHAVES = ORDEM
};

enum ChaveID{FOLHA, INTERNO};

struct Node;
#pragma pack(push, 1)
struct Chave{
    Retangulo MBR;
    union{
        streampos ChildPtr; // Ponteiro para o proximo nó em disco
        streampos Dado; // Guarda o indice da forma em disco
    };
    Chave(Retangulo&, streampos&, int);
};
#pragma pack(pop)

struct Node{
    streampos DiskPos;
    unsigned Nivel;
    vector<Chave> Chaves;
    Node(unsigned, vector<Chave>&);
    Node(streampos&);
    bool Folha();
    bool Overflow();
};

class RTree{
private:
    Node* raiz{};
    size_t count;

public:
   RTree();
   void Inserir(Retangulo&, const streampos&);
   bool IsEmpty();
   Node* GetPtr();
   list<Node*>* Traversal(streampos&, Ponto&); // PERCORRE A ARVORE
   list<streampos>* Busca(Ponto&); // BUSCA UM PONTO NA ARVORE
   Node* EscolhaSubArvore(Node*, Retangulo&);
   bool InserirNaFolha(Node*, Retangulo&, streampos&);
};

bool comparacao(const pair<Node*, double>& primeiro, const pair<Node*, double>& segundo){
    return primeiro.second < segundo.second;
}

}
#endif // RTREE_HPP
