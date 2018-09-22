#ifndef SPATIALDATA_HPP
#define SPATIALDATA_HPP
#include "stdlib.hpp"
#include <vector>
namespace SpatialData{

class Ponto{
private:
    double x{}, y{};
public:
    Ponto(double X, double Y){
        this->x = X;
        this->y = Y;
        cout << "Ponto criado" << endl;
    }

    double Distancia(const Ponto& OtherPoint){
        return sqrt(pow((this->x - OtherPoint.x), 2) + pow((this->y - OtherPoint.y), 2));
    }

};

class Poligono{
protected:
    vector<Ponto> Coordenadas;
    size_t NumVertices;
    size_t NumArestas;
public:
    vector<Ponto>& GetCoordenadas(){
        return Coordenadas;
    }
    size_t GetNumVertices(){
        return NumVertices;
    }
};

class Retangulo: Poligono{
public:
    Retangulo(const Ponto& MenorEixoX, const Ponto& MaiorEixoX, const Ponto& MenorEixoY, const Ponto& MaiorEixoY){
            NumArestas = NumVertices = 4;
            Coordenadas.resize(NumVertices);
            Coordenadas[0] = MenorEixoX;
            Coordenadas[1] = MaiorEixoX;
            Coordenadas[2] = MenorEixoY;
            Coordenadas[3] = MaiorEixoY;
    }
};

}
#endif // SPATIALDATA_HPP
