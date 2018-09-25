/*  SPATIAL DATA HEADER
 *  ESSE ARQUIVO TERÁ TODAS AS FORMAS GEOMÉTRICAS NECESSÁRIAS NO BANCO DE DADOS
 *  OU SEJA, PONTOS, RETAS E POLIGONOS
 *  ALÉM DOS ALGORITMOS DE GEOMETRIA COMPUTACIONAL
 */
#ifndef SPATIALDATA_HPP
#define SPATIALDATA_HPP
#include "stdlib.hpp"
namespace SpatialData{

/*
 * Namespace SpatialData
 * -> Contém os objetos que descrevem as formas geográficas
 * Disciplina para a criação de objetos:
 * Forneça os pontos em sentido anti-horário
 */

class Ponto{
private:
    double x, y; // Coordenadas do Ponto
public:
    Ponto(double X = 0.0, double Y = 0.0){
        x = X;
        y = Y;
    }
    double GetX() const{ return x; }
    double GetY() const{ return y; }
    double Distancia(const Ponto& OtherPoint){ // Distância Euclidiana entre pontos
        return sqrt(pow((this->x - OtherPoint.x), 2) + pow((this->y - OtherPoint.y), 2));
    }
    friend Ponto& operator+(const Ponto& This, const Ponto& Other);
    friend Ponto& operator-(const Ponto& This, const Ponto& Other);
};

class Linha{
protected:
    vector<Ponto> Coordenada;
    double Distancia;
public:
    Linha(const Ponto& A = Ponto(0.0, 0.0), const Ponto& B = Ponto(0.0, 0.0)): Coordenada(2){
        Coordenada[0] = A;
        Coordenada[1] = B;
        Distancia = Coordenada[0].Distancia(Coordenada[1]);
    }
    vector<Ponto>& GetCoordenada(){ return Coordenada; }
    double GetDistance() const{ return Distancia; }
};

class Poligonal{
protected:
    vector<Linha> LineList; // Lista de linhas
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
            Perimetro += Aresta.GetDistance();
    }
    vector<Linha>& GetCoordenada(){ return LineList; }
    double GetPerimetro() const{ return Perimetro; }
    unsigned short GetArestas() const{ return Arestas; }
};

class Poligono: public Poligonal{
protected:
    unsigned short Diagonais;
    double Area;
    enum NomePoligono{
      TRIANGULO=3, QUADRILATERO, PENTAGONO, HEXAGONO, HEPTAGONO, OCTOGONO, ENEAGONO, DECAGONO
    };
public:
    Poligono(vector<Linha>& Input): Poligonal(Input){
        Diagonais = (Arestas*(Arestas-3))/2;
    }
    double GetArea() const{ return Area; }
};

class Triangulo: public Poligono{
public:
    Triangulo(vector<Linha>& Input): Poligono(Input){
        /*
         * Usando a fórmula de Herons para calcular a área sem precisar calcular a altura do triangulo
         */
        double S = 0.0;
        for(auto Aresta: Input)
            S+=Aresta.GetDistance();
        S /= 2;
        Area = sqrt(S*(S-Input[0].GetDistance())*(S-Input[1].GetDistance())*(S-Input[2].GetDistance()));
    }
};

class Quadrilatero: public Poligono{
private:
    double Base, Altura;
public:
    Quadrilatero(vector<Linha>& Input): Poligono(Input){
        Base = Input[0].GetDistance();
        Altura = Input[1].GetDistance();
        Area = Base * Altura;
    }
};
} // NAMESPACE SPATIALDATA
#endif // SPATIALDATA_HPP
