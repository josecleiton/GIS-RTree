#include "SpatialData.hpp"
using namespace SpatialData;

Ponto operator+(const Ponto& This, const Ponto& Other){
    return Ponto(This.GetX()+Other.GetX(), This.GetY()+Other.GetY());
}

Ponto operator-(const Ponto& This, const Ponto& Other){
    return Ponto(This.GetX()-Other.GetX(), This.GetY()-Other.GetY());
}

Ponto operator*(double K, const Ponto& Other){
    return Ponto(K*Other.GetX(), K*Other.GetY());
}
bool operator==(Ponto& This, Ponto& Other){
    double DX = fabs(This.GetX()-Other.GetX());
    double DY = fabs(This.GetY()-Other.GetY());
    return (DX==0.0 and DY==0.0)?true:false;
}
bool operator!=(Ponto& This, Ponto& Other){
    return !(This==Other);
}
bool operator<(Ponto& This, Ponto& Other){
    bool comparacao = (fabs(This.GetX()-This.GetY()) == 0.0)?true:false;
    return ((This.GetX() < Other.GetX()) or ((comparacao) and (This.GetY() < Other.GetY())));
}
bool operator>(Ponto& This, Ponto& Other){
    bool comparacao = (fabs(This.GetX()-This.GetY()) == 0.0)?true:false;
    return ((This.GetX() > Other.GetX()) or ((comparacao) and (This.GetY() > Other.GetY())));
}
/*
void Poligonal::Reset(){
    index = 0U;
}

Linha Poligonal::Prev(){
    if(!index) index = GetArestas()-1;
    else index--;
    return this->LineList[index];
}

Linha Poligonal::Next(){
    if(index == GetArestas()-1) index = 0U;
    else index++;
    return this->LineList[index];
}

vector<Triangulo> Poligono::Triangulacao(){
    Linha l0, l1, l2, l3, l4;
    auto Length = this->LineList.size();

    while(Length>3){
        l2 = LineList[this->GetIndex()];
        do{
            if(l2.GetEar()){
                l3 = this->Next();

            }
        }
    }
}

/*
vector<Triangulo> Poligono::Triangulacao(){
    vector<Linha> P = this->GetCoordenadas();
    vector<Triangulo> Resultado;
    if(this->GetArestas() == 3){
        Triangulo temp(P);
        Resultado.push_back(temp);
        return Resultado;
    }
    else{
        FindConvexVertex(this->GetCoordenadas());
        Linha D = FindIntrudingVertex(this->GetCoordenadas());
        if(D.GetMedida() == 0.0){
            Linha C = P[];

        }
    }
}
*/
