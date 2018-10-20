#ifndef RTREE_HPP
#define RTREE_HPP
#include <iostream>
#include <stack>
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
    ORDEM = 8,
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
    Chave();
};
#pragma pack(pop)

struct Node{
    streampos DiskPos;
    unsigned Nivel;
    vector<Chave> Chaves;
    Node(unsigned, vector<Chave>&);
    Node(streampos&);
    Node(Retangulo& R, streampos& PosR);
    streampos SalvarNo();
    bool Cresce(Retangulo&, unsigned);
    bool Folha();
    bool Overflow();
};

struct NodeAux{
    Node* ptr;
    unsigned index;
    NodeAux(Node* p = nullptr, unsigned i = 0): ptr(p), index(i){}
};

class RTree{
private:
    Node* raiz{};
    size_t count;

public:
   RTree();
   ~RTree();
   void Inserir(Retangulo&, streampos&);
   bool IsEmpty();
   Node* GetPtr();
   void CriaArvore(Retangulo&, streampos&);
   list<Node*>* Traversal(streampos&, Ponto&); // PERCORRE A ARVORE
   list<streampos>* Busca(Ponto&); // BUSCA UM PONTO NA ARVORE
   Node* EscolhaSubArvore(Node*, stack<NodeAux>&, Retangulo&);
   void InserirNaFolha(Node*, stack<NodeAux>&, Retangulo&, streampos&);
   void AjustaCaminho(Node*, stack<NodeAux>&, Retangulo&);
   void DividirEAjustar(Node*, stack<NodeAux>&, Chave&);
   bool ArquivoVazio();
};

bool comparacaoESA(const pair<NodeAux, double>&, const pair<NodeAux, double>&);

}
#endif // RTREE_HPP
