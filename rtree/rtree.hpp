#ifndef RTREE_HPP
#define RTREE_HPP
#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <list>
#include <fstream>
#include "spatialdata.hpp"

#define RTREE_FILENAME "../GIS/rtree/rtree.bin" // ARQUIVO BINÁRIO COM A RTREE
#define H_FILENAME "../GIS/spatialdata/hash/" // CAMINHO PARA A PASTA DO HASH

using namespace std;
using namespace SpatialData;
namespace SpatialIndex{

/* NAMESPACE QUE LIDA COM A INDEXAÇÃO DE FORMAS GEOMÉTRICAS EM DISCO
 * - RTREE (ORIGINAL)
 * - HASH
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
    Retangulo MBR; // MBR DO QUE A CHAVE APONTAR
    union{
        streampos ChildPtr; // PONTEIRO PARA O PRÓXIMO NÓ EM DISCO
        streampos Dado; // GUARDA O INDICE DA FORMA EM DISCO
    };
    Chave(Retangulo&, streampos&, unsigned);
    Chave();
    Chave(Node*&);
    bool Ajusta(Retangulo& alvo); // AJUSTA O RETANGULO DA CHAVE EM RELAÇÃO AO ALVO
    friend bool operator==(const Chave&, const Chave&);
    friend bool operator!=(const Chave&, const Chave&);
    friend bool operator<(const Chave&, const Chave&);
    friend bool operator>(const Chave&, const Chave&);
};
#pragma pack(pop)

struct Node{
    streampos DiskPos; // POSIÇÃO EM DISCO DO NÓ
    unsigned Nivel; // NÍVEL DO NÓ (FOLHA OU NÃO FOLHA)
    vector<Chave> Chaves; // CHAVES DO NÓ

    //MÉTODOS
    Node(unsigned, vector<Chave>&);
    Node(streampos&);
    Node(Retangulo& R, streampos& PosR);
    Node(vector<Node*>&);
    streampos SalvarNo(); //SALVA NÓ EM DISCO
    streampos Delete(); // REMOVE LOGICAMENTE O NÓ EM DISCO
    bool Folha(); // É FOLHA OU NÃO
    bool Overflow(); // EXCEDEU A ORDEM OU NÃO
    Retangulo GetRetangulo(); // RETORNA O RETÂNGULO QUE ENVOLVE TODO O NÓ
};

struct NodeAux{ // ESTRUTURA AUXILIAR USADA PARA PEGA OS PARENTES DO NÓ
    Node* ptr; // PONTEIRO PARA O NÓ "PAI" POR EXEMPLO
    unsigned index; // INDICE DO NÓ ATUAL NO NÓ PAI
    NodeAux(Node* p = nullptr, unsigned i = 0): ptr(p), index(i){}
};

class RTree{
private:
    Node* raiz{}; // PONTEIRO PARA O NÓ RAIZ
    unsigned count{}; // CONTADOR DE NÓS
    unsigned altura{}; // ALTURA DA ÁRVORE
    size_t registros{}; // NUMERO DE REGISTROS
    fstream treeFile; // BUFFER QUE CARREGA O ARQUIVO RTREE_FILE USADO
    //                           COMO STREAM DOS NÓS DA ÁRVORE R
public:
   RTree();
   ~RTree();
   fstream& File();
   unsigned GetCount();
   unsigned GetAltura();
   size_t GetRegistros();
   void Inserir(Retangulo&, streampos&); // INSERE O RETANGULO NA ÁRVORE
   void Inserir(Chave&);
   bool IsEmpty();
   Node* NodePtr();
   void CriaArvore(Retangulo&, streampos&); // CRIA ÁRVORE COM UM ELEMENTO
   bool ApagarArvore(); // APAGA TODA A ARVORE (FISICAMENTE)
   list<Chave> Busca(Ponto&); // BUSCA UM PONTO NA ARVORE
   bool Busca(Node*, Retangulo&, vector<NodeAux>&); // BUSCA UM RETÂNGULO
   bool Busca(Node*, Chave&, vector<NodeAux>&); // BUSCA UMA CHAVE
   Node* EscolhaSubArvore(Node*&, stack<NodeAux>&, Retangulo&); // ESCOLHE A MELHOR SUBÁRVORE PARA UM RETANGULO SER INSERIDO
   list<Chave> Traversal(streampos&, Ponto&); // PERCORRE A ÁRVORE
   void InserirNo(Node*&, stack<NodeAux>&, Chave&); // INSERE UMA CHAVE NO NÓ
   void AjustaCaminho(stack<NodeAux>&, Retangulo); // AJUSTA OS RETANGULOS DA ÁRVORE SE HOUVER MODIFICAÇÃO NOS MESMOS
   void DividirEAjustar(Node*&, stack<NodeAux>&); // DIVIDE E AJUSTA O CAMINHO
   Node* Divide(Node*&);  // DIVIDE PROPRIAMENTE O NÓ QUE TEM CARDINALIDADE >= AO MAXCHAVES
   void CriaNovaRaiz(Node*&, Node*&); // SE A RAIZ FOR DIVIDIDA, CRIA UMA NOVA
   bool Remove(Chave& K); // REMOVE A PARTIR DE UMA CHAVE
   bool Remove(vector<NodeAux>&); // REMOVE A PARTIR DO CAMINHO
   void Remove(stack<NodeAux>&); // REMOVE A PARTIR DO CAMINHO
   list<Chave> Reorganizar(stack<NodeAux>&); // RETORNA UMA LISTA DE CHAVES QUE NECESSITAM SER REINSERIDAS A PARTIR DE UMA REMOÇÃO
   void Reinserir(list<Chave>&); // REINSERE A LISTA DE CHAVES RETORNADA PELA FUNÇÃO ANTERIOR
   list<Chave> EncontreAsFolhas(Node*, bool remove); // RETORNA TODAS AS FOLHAS A PARTIR DE UM NÓ
   bool ArquivoVazio(); // VERIFICA SE O ARQUIVO QUE GUARDA A RTREE ESTÁ VAZIO
};

void Kai(stack<NodeAux>&); // LIBERA UMA PILHA DE NÓS
void Kai(vector<NodeAux>&); // LIBERA UM VETOR DE NÓS

class Hash{ // PSEUDO HASH COM FORMAS GEOMETRICAS
private:
    vector<streampos>* handle{}; // VETOR DE PONTEIROS PARA DISCO
    string path; // CAMINHO PARA A PASTA QUE GUARDARÁ OS ARQUIVOS DE HASH
public:
    Hash();
    ~Hash();
    bool ArquivoVazio(); // VERIFICA SE O ARQUIVO ESTÁ VAZIO
    void Insere(string id, streampos&); // INSERE UMA FORMA COM UM ID X NO ARQUIVO CORRETO
    vector<streampos>* SelectAll(string& query); // RETORNA TODAS AS FORMAS QUE TENHAM QUERY COMO ID
};


}
#endif // RTREE_HPP
