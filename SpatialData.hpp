/*  SPATIAL DATA HEADER
 *  ESSE ARQUIVO TERÁ TODAS AS FORMAS GEOMÉTRICAS NECESSÁRIAS NO BANCO DE DADOS
 *  OU SEJA, PONTOS, RETAS E POLIGONOS
 *  ALÉM DOS ALGORITMOS DE GEOMETRIA COMPUTACIONAL
 */
#ifndef SPATIALDATA_HPP
#define SPATIALDATA_HPP
#include <cfloat>
#include "stdlib.hpp"
#include "circular_list.hpp"
namespace SpatialData{

/*
 * Namespace SpatialData
 * -> Contém os objetos que descrevem as formas geográficas
 */

#define PI acos(-1)
enum Classificadores {ESQUERDA, DIREITA, FRENTE, ATRAS, ENTRE, ORIGEM, DESTINO}; // USADO NA CLASSIFICAÇÃO
enum Rotacao {HORARIO, ANTIHORARIO}; // SENTIDO DAS ROTAÇÕES NA LISTA DUPLAMENTE ENCADEADA DE VERTICES
enum Posicao_Relativa_Retas {COLINEAR, PARALELA, CONSECUTIVO, CONSECUTIVO_CRUZADO, CONSECUTIVO_NAO_CRUZADO}; // USADA PELA POSIÇÃO RELATIVA ENTRE SEGMENTOS DE RETA (ARESTAS)

class Aresta;

class Ponto{
public:
    double x, y;
    Ponto(double X = 0.0, double Y = 0.0);
    int Classificacao(Ponto&, Ponto&);
    int Classificacao(Aresta);
    int Orientacao(Ponto&, Ponto&, Ponto&);
    double AnguloPolar();
    double Tamanho();
    double Distancia(Aresta&);
    double Distancia(Ponto&);
    double GetX() const{ return x; }
    double GetY() const{ return y; }
    double operator[](int);
};

class Vertice: public CircularList::Node, public Ponto{
public:
    Vertice(double x, double y);
    Vertice(Ponto&);
    Vertice* Horario(); // percorre a lista em sentido horário
    Vertice* Antihorario(); // percorre a lista em sentido antihorário
    Vertice* Vizinho(int); // retorna o nó vizinho
    Ponto& GetPonto();
    Vertice* Push(Vertice*);
    Vertice* Pop();
    void Splice(Vertice*);
    Aresta Envelope(); // MINIMUM BOUNDING RECTANGLE PARA A R-TREE
    Vertice* Split(Vertice*);
    friend class Poligono;
};

class Poligono{
private:
    Vertice* list;
    unsigned m_size;
    void resize();
public:
    Poligono();
    Poligono(Poligono&);
    Poligono(Vertice*);
    ~Poligono();
    Vertice* GetVertice();
    unsigned GetSize();
    Ponto& GetPonto();
    Aresta GetAresta();
    Vertice* Avancar(int);
    Vertice* Horario();
    Vertice* Antihorario();
    Vertice* Vizinho(int);
    Vertice* SetV(Vertice*);
    Vertice* Push(Ponto);
    Vertice* Push(Ponto&);
    void Pop();
    void Resize();
    Poligono* Split(Vertice*);
    double AreaTriangulacao(vector<Poligono*>&) const;
};

class Aresta{
private:
    Ponto origem;
    Ponto destino;
public:
    Aresta();
    Aresta(Ponto&, Ponto&);
    Aresta& Rotacao();
    Aresta& Flip();
    Ponto GetPonto(double&);
    double Dist() const;
    int Intersect(Aresta&, double&);
    int Cruza(Aresta&, double&);
    bool isVertical();
    double Inclinacao();
    double y(double);
    double Slope();
    friend class Ponto;
};

class Retangulo{
public:
    Ponto SE;
    Ponto NE;
    int ID;
    Retangulo(Ponto&, Ponto&, int);
    Retangulo();
};

double ProdutodePontos(Ponto&, Ponto&);
Vertice* MenorVertice(Poligono&, int (*cmp)(Ponto*, Ponto*));
int EsquerdaDireita(Ponto*, Ponto*);
int DireitaEsquerda(Ponto*, Ponto*);
// PARA TRIANGULAR O(N^2)
list<Poligono*> Triangulacao(Poligono&);
void FindVerticeConvexo(Poligono&);
Vertice* FindIntrudingVertex(Poligono&);
bool PontoNoTriangulo(Ponto&, Ponto&, Ponto&, Ponto&);
double Area(Poligono&);

} // NAMESPACE SPATIALDATA
#endif // SPATIALDATA_HPP
