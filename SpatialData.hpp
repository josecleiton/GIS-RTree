#ifndef SPATIALDATA_HPP
#define SPATIALDATA_HPP
#include "stdlib.hpp"
#include <vector>
namespace SpatialData{

#define NUM_PONTOS_RETANGULO 4

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

class Retangulo{
    vector<Ponto> Coordenadas;
public:
    Retangulo(const Ponto& MenorEixoX, const Ponto& MaiorEixoX, const Ponto& MenorEixoY, const Ponto& MaiorEixoY):
        Coordenadas(NUM_PONTOS_RETANGULO) {
            Coordenadas[0] = MenorEixoX;
            Coordenadas[1] = MaiorEixoX;
            Coordenadas[2] = MenorEixoY;
            Coordenadas[3] = MaiorEixoY;
    }
    vector<Ponto>& GetCoordenadas(){
        return Coordenadas;
    }
};

}
#endif // SPATIALDATA_HPP
