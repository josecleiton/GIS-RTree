#ifndef RTREE_HPP
#define RTREE_HPP
#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <list>
#include <fstream>
#include "spatialdata.hpp"

#define RTREE_FILENAME "../GIS/rtree/rtree.bin"
#define H_FILENAME "../GIS/spatialdata/hash/"
using namespace std;
using namespace SpatialData;
namespace SpatialIndex{
/*
 * Forma geométrica no disco
 * Não faço ideia de como implementar essa classe
 */

enum _RTree{
    ORDEM = 8, // GERA NÓS COM 489 bytes
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
    Chave(Retangulo&, streampos&, unsigned);
    Chave();
    Chave(Node*&);
    friend bool operator==(const Chave&, const Chave&);
    friend bool operator!=(const Chave&, const Chave&);
    friend bool operator<(const Chave&, const Chave&);
    friend bool operator>(const Chave&, const Chave&);
};
#pragma pack(pop)

struct Node{
    streampos DiskPos;
    unsigned Nivel;
    vector<Chave> Chaves;

    //MÉTODOS
    Node(unsigned, vector<Chave>&);
    Node(streampos&);
    Node(Retangulo& R, streampos& PosR);
    Node(vector<Node*>&);
    streampos SalvarNo();
    streampos Delete();
    bool Ajusta(Retangulo&, unsigned);
    bool Folha();
    bool Overflow();
    void InserirChave(Chave&);
    Retangulo GetRetangulo();
};

struct NodeAux{
    Node* ptr;
    unsigned index;
    NodeAux(Node* p = nullptr, unsigned i = 0): ptr(p), index(i){}
};

class RTree{
private:
    Node* raiz{};
    unsigned count{};
    unsigned altura{};
    size_t registros{};
public:
   RTree();
   ~RTree();
   size_t GetCount();
   size_t GetRegistros();
   void Inserir(Retangulo&, streampos&);
   void Inserir(Chave&);
   bool IsEmpty();
   Node* GetPtr();
   void CriaArvore(Retangulo&, streampos&);
   bool ApagarArvore();
   list<Chave> Busca(Ponto&); // BUSCA UM PONTO NA ARVORE
   bool Busca(Node*, Retangulo&, vector<NodeAux>&);
   bool Busca(Node*, Chave&, vector<NodeAux>&);
   Node* EscolhaSubArvore(Node*&, stack<NodeAux>&, Retangulo&);
   list<Chave> Traversal(streampos&, Ponto&);
   void InserirNo(Node*&, stack<NodeAux>&, Chave&);
   void AjustaCaminho(stack<NodeAux>&, Retangulo);
   void DividirEAjustar(Node*&, stack<NodeAux>&);
   Node* Divide(Node*&);
   void CriaNovaRaiz(Node*&, Node*&);
   bool Remove(Chave& K);
   bool Remove(vector<NodeAux>&);
   void Remove(stack<NodeAux>&);
   list<Chave> Reorganizar(stack<NodeAux>&);
   void Reinserir(list<Chave>&);
   list<Chave> EncontreAsFolhas(Node*, bool remove);
   bool ArquivoVazio();
   string GetNivel();
};

void Kai(stack<NodeAux>&);
void Kai(vector<NodeAux>&);

class Hash{
private:
    vector<streampos>* handle{};
    string path;
public:
    Hash();
    ~Hash();
    bool ArquivoVazio();
    void Insere(string, streampos&);
    void Busca(string&, bool&);
    vector<streampos>* SelectAll(string&);
};


}
#endif // RTREE_HPP
