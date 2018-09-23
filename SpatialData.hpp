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

    double Distancia(const Ponto& OtherPoint){ // Distância Euclidiana entre pontos
        return sqrt(pow((this->x - OtherPoint.x), 2) + pow((this->y - OtherPoint.y), 2));
    }

};

/*
 * Super-classe dos polígonos, quaisquer poligono deve herdar dessa classe em questão
 */
class Poligono{
protected:
    vector<Ponto> Coordenadas; //Todo poligono tem suas coordenadas
    short NumVertices; // Numero de vertices do poligono
    short NumDiagonais; // Numero de diagonais
    double Perimetro; // Perimetro
    double Area; // Area
public:
    vector<Ponto>& GetCoordenadas(){ return Coordenadas; }
    short GetNumVertices(){ return NumVertices; }
    double GetPerimetro(){ return Perimetro; }
};

class Retangulo: Poligono{ // Herdando os atributos da classe poligono
public:
    Retangulo(const Ponto& MenorEixoX = Ponto(0.0, 0.0), const Ponto& MaiorEixoX = Ponto(0.0, 0.0),\
              const Ponto& MenorEixoY = Ponto(0.0, 0.0), const Ponto& MaiorEixoY = Ponto(0.0, 0.0)){
            NumVertices = 4;
            NumDiagonais = ((NumVertices)*(NumVertices-3))/2;
            Coordenadas.resize(static_cast<size_t>(NumVertices));
            Coordenadas[0] = MenorEixoX;
            Coordenadas[1] = MaiorEixoX;
            Coordenadas[2] = MenorEixoY;
            Coordenadas[3] = MaiorEixoY;
            double DistanciaEixoX = Coordenadas[0].Distancia(Coordenadas[1]);
            double DistanciaEixoY = Coordenadas[2].Distancia(Coordenadas[3]);
            Perimetro = 2*DistanciaEixoX + 2*DistanciaEixoY;
            Area = DistanciaEixoX * DistanciaEixoY;
    }
};

}
#endif // SPATIALDATA_HPP
