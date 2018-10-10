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
    bool Folha();
    bool Overflow();
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
   Node* EscolhaSubArvore(Node*, Retangulo&);
   bool InserirNaFolha(Node*, Retangulo&, streampos&);
   void AjustaCaminho(Node*);
   void DividirEAjustar(Node*);
   bool ArquivoVazio();
};

bool comparacao(const pair<Node*, double>&, const pair<Node*, double>&);

}
#endif // RTREE_HPP
