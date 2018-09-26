/*  SPATIAL DATA HEADER
 *  ESSE ARQUIVO TERÁ TODAS AS FORMAS GEOMÉTRICAS NECESSÁRIAS NO BANCO DE DADOS
 *  OU SEJA, PONTOS, RETAS E POLIGONOS
 *  ALÉM DOS ALGORITMOS DE GEOMETRIA COMPUTACIONAL
 */
#ifndef SPATIALDATA_HPP
#define SPATIALDATA_HPP
#include "stdlib.hpp"
#include "circular_list.hpp"
namespace SpatialData{

/*
 * Namespace SpatialData
 * -> Contém os objetos que descrevem as formas geográficas
 */

#define PI acos(-1)
enum {ESQUERDA, DIREITA, FRENTE, ATRAS, ENTRE, ORIGEM, DESTINO}; // USADO NA CLASSIFICAÇÃO
enum{HORARIO, ANTIHORARIO}; // SENTIDO DAS ROTAÇÕES NA LISTA DUPLAMENTE ENCADEADA DE VERTICES

class Aresta;

class Ponto{
protected:
    double x, y;
public:
    Ponto(double X = 0.0, double Y = 0.0);
    int Classificacao(Ponto&, Ponto&);
    int Classificacao(Aresta&);
    int Orientacao(Ponto&, Ponto&, Ponto&);
    double AnguloPolar();
    double Tamanho();
    double Distancia(Aresta&);
    double Distancia(Ponto&, Ponto&);
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
    Vertice* Split(Vertice*);
    friend class Poligono;
};

class Poligono{
private:
    Vertice* List;
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
    Vertice* Push(Ponto&);
    void Pop();
    void Resize();
    Poligono* Split(Vertice*);
};

/*
class Linha{
protected:
    vector<Ponto> Coordenadas;
    double Medida;
public:
    Linha(const Ponto& A = Ponto(0.0, 0.0), const Ponto& B = Ponto(0.0, 0.0)): Coordenadas(2){
        Coordenadas[0] = A;
        Coordenadas[1] = B;
        Medida = Coordenadas[0].Distance(Coordenadas[1]);
    }
    vector<Ponto>& GetCoordenadas(){ return Coordenadas; }
    double GetMedida() const{ return Medida; }
};

class Shape{
protected:
    double Area;
    double Perimetro;
};

class Poligonal{
protected:
    vector<Linha> LineList; // Lista de linhas que formam a poligonal
    double Perimetro{};
    unsigned short Arestas;
public:
    Poligonal(){
        for(short i=0; i<2; i++)
            LineList.push_back(Linha());
    }
    Poligonal(vector<Linha>& Input){
        LineList = Input;
        Arestas = static_cast<unsigned short>(LineList.size());
        for(auto Aresta:LineList)
            Perimetro += Aresta.GetMedida();
    }
    vector<Linha>& GetCoordenadas(){ return LineList; }
    double GetPerimetro() const{ return Perimetro; }
    unsigned short GetArestas() const{ return Arestas; }
protected:
    unsigned index{};
    unsigned GetIndex() const{ return index; }
    Linha Prev();
    Linha Next();
    void Reset();
};

class Triangulo;

class Poligono: public Poligonal, public Shape{
protected:
    unsigned short Diagonais; // Lista de linhas que forma o poligono
    enum NomePoligono{
      TRIANGULO=3, QUADRILATERO, PENTAGONO, HEXAGONO, HEPTAGONO, OCTOGONO, ENEAGONO, DECAGONO
    };
public:
    Poligono(vector<Linha>& Input): Poligonal(Input){
        Diagonais = (Arestas*(Arestas-3))/2;
    }
    double GetArea() const{ return Area; }
    double GetDiagonais() const{ return Diagonais; }
    vector<Triangulo> Triangulacao();
};

class Triangulo: public Poligono{
public:
    Triangulo(vector<Linha>& Input): Poligono(Input){
         // Usando a fórmula de Heron para calcular a área sem precisar calcular a altura do triangulo
        double S = 0.0;
        for(auto Aresta: LineList)
            S+=Aresta.GetMedida();
        S /= 2;
        Area = sqrt(S*(S-LineList[0].GetMedida())*(S-LineList[1].GetMedida())*(S-LineList[2].GetMedida()));
    }
};

class Quadrilatero: public Poligono{
private:
    double Base, Altura;
public:
    Quadrilatero(vector<Linha>& Input): Poligono(Input){
        Base = Input[0].GetMedida();
        Altura = Input[1].GetMedida();
        Area = Base * Altura;
    }
};

class Circulo: public Shape{
private:
    Ponto Centro;
    double Raio;
public:
    Circulo(const Ponto& Centro = Ponto(), const double Raio = 0.0): Centro(Centro), Raio(Raio){
        Area = PI*Raio*Raio;
        Perimetro = PI*2*Raio;
    }
    Ponto GetCentro() const{ return Centro; }
    double GetRaio() const{ return Raio; }
    bool PointInCircle(const Ponto& P){
        Ponto temp = P;
        double Distance = temp.Distance(Centro);
        if(Distance <= Raio) return true;
        else return false;
    }
};
*/
} // NAMESPACE SPATIALDATA
#endif // SPATIALDATA_HPP
