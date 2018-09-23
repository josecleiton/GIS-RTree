/*  SPATIAL DATA HEADER
 *  ESSE ARQUIVO TERÁ TODAS AS FORMAS GEOMÉTRICAS NECESSÁRIAS NO BANCO DE DADOS
 *  OU SEJA, PONTOS, RETAS E POLIGONOS
 *  ALÉM DOS ALGORITMOS DE GEOMETRIA COMPUTACIONAL
 */
#ifndef SPATIALDATA_HPP
#define SPATIALDATA_HPP
#include "stdlib.hpp"
namespace SpatialData{

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

/*
 * Super-classe dos polígonos, quaisquer poligonos deve herdar dessa classe
 */
class Poligono{
protected:
    vector<Ponto> Coordenadas; //Todo poligono tem suas coordenadas
    short NumVertices; // Numero de vertices do poligono
    short NumDiagonais; // Numero de diagonais
    double Perimetro; // Perimetro
    double Area; // Area
    void SetDiagonais(){ this->NumDiagonais = ((NumVertices)*(NumVertices-3))/2; }
public:
    vector<Ponto>& GetCoordenadas(){ return Coordenadas; }
    size_t GetNumCoordenadas() { return Coordenadas.size(); }
    short GetNumVertices() const{ return NumVertices; }
    double GetPerimetro() const{ return Perimetro; }
};

class Retangulo: Poligono{ // Herdando os atributos da classe poligono
public:
    Retangulo(const Ponto& MenorEixoX = Ponto(0.0, 0.0), const Ponto& MaiorEixoX = Ponto(0.0, 0.0),\
              const Ponto& MenorEixoY = Ponto(0.0, 0.0), const Ponto& MaiorEixoY = Ponto(0.0, 0.0)){
            NumVertices = 4;
            SetDiagonais(); // Numero de diagonais é atualizado em relação ao numero de vertices
            Coordenadas.resize(static_cast<size_t>(NumVertices));
            Coordenadas[0] = MenorEixoX;
            Coordenadas[1] = MaiorEixoX;
            Coordenadas[2] = MenorEixoY;
            Coordenadas[3] = MaiorEixoY;
            double Base = Coordenadas[0].Distancia(Coordenadas[1]);
            double Altura = Coordenadas[2].Distancia(Coordenadas[0]);
            Perimetro = 2*Base + 2*Altura;
            Area = Base * Altura;
    }
};

}
#endif // SPATIALDATA_HPP
