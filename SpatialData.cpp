#include "SpatialData.hpp"
namespace SpatialData{

Ponto::Ponto(double X, double Y): x(X), y(Y){
}

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
double Ponto::operator[](int k){
    return (k%2)?y:x;
}

int Ponto::Orientacao(Ponto& p0, Ponto& p1, Ponto& p2){
    Ponto a = p1 - p0;
    Ponto b = p2 - p0;
    double sa = a.x*b.y-b.x*a.y;
    if(sa>0.0)
        return 1;
    if(sa<0.0)
        return -1;
    return  0;
}

int Ponto::Classificacao(Ponto& p0, Ponto& p1){
    Ponto p2 = *this;
    Ponto a = p1-p0;
    Ponto b = p2-p0;
    double sa = a.x*b.y-b.x*a.y;
    if(sa>0.0)
        return ESQUERDA;
    if(sa<0.0)
        return DIREITA;
    if((a.x*b.x<0.0) or (a.y*b.y<0.0))
        return ATRAS;
    if(a.Tamanho()<b.Tamanho())
        return FRENTE;
    if(p0 == p2)
        return ORIGEM;
    if(p1 == p2)
        return DESTINO;
    return ENTRE;
}
/*
int Ponto::Classificacao(Aresta& E){
    return Classificacao(E.origem, E.dest);
}
*/
double Ponto::AnguloPolar(){
    if((x==0.0) and (y==0.0))
        return -1.0;
    if(x==0.0)
        return (y>0.0)?90:270;
    double theta = atan(y/x); // em radianos
    theta *= 360/(2*PI); // conversão para ângulo
    if(x>0.0) // quadrante 1 e 4
        return (y>=0.0)?theta:360+theta;
    else // quadrante 2 e 3
        return 180+theta;

}

double Ponto::Tamanho(){
    return sqrt(x*x+y*y);
}

double Ponto::Distancia(Ponto& p0, Ponto& p1){
    return sqrt(pow(p0.x-p1.x, 2)+pow(p0.y-p1.y, 2));
}

// IMPLEMENTAÇÕES DA CLASSE VERTICE ABAIXO

Vertice::Vertice(double x, double y): Ponto(x,y){
}

Vertice::Vertice(Ponto& p): Ponto(p){
}

Vertice* Vertice::Horario(){
    return static_cast<Vertice*>(m_next);
}

Vertice* Vertice::Antihorario(){
    return static_cast<Vertice*>(m_prev);
}

Vertice* Vertice::Vizinho(int rotacao){
    return (rotacao==HORARIO)?Horario():Antihorario();
}

Ponto& Vertice::GetPonto(){
    return *(static_cast<Ponto*>(this));
}

Vertice* Vertice::Push(Vertice* V){
    return static_cast<Vertice*>(Node::Push(V));
}

Vertice* Vertice::Pop(){
    return static_cast<Vertice*>(Node::Pop());
}

void Vertice::Splice(Vertice* V){
    Node::Splice(V);
}

Poligono::Poligono(): List(nullptr), m_size(0){
}
Poligono::Poligono(Poligono& p){
    m_size = p.m_size;
    if(!m_size)
        List = nullptr;
    else{
        List = new Vertice(p.GetPonto());
        for(unsigned i=1; i<m_size; i++){
            p.Avancar(HORARIO);
            List = List->Push(new Vertice(p.GetPonto()));
        }
        p.Avancar(HORARIO);
        List = List->Horario();
    }
}

Poligono::Poligono(Vertice* ListaVertice): List(ListaVertice){
    Resize();
}

void Poligono::Resize(){ // DEVE SER CHAMADO SEMPRE QUE UMA CADEIA DE VERTICES DE TAMANHO DESCONHECIDA FOR ADICIONADA AO POLIGONO
    if(List == nullptr)
        m_size = 0;
    else{
        Vertice* v = List->Horario();
        for(m_size = 1; List != v; ++m_size, List = List->Horario());
    }
}

Poligono::~Poligono(){
    if(List != nullptr){
        Vertice* aux = List->Horario();
        while(List != aux){
            delete aux->Pop();
            aux = List->Horario();
        }
        delete List;
    }
}

/*
 * Funções de acesso à classe
 */
Vertice* Poligono::GetVertice(){
    return List;
}

unsigned Poligono::GetSize(){
    return m_size;
}

Ponto& Poligono::GetPonto(){
    return List->GetPonto();
}

Vertice* Poligono::Horario(){
    return List->Horario();
}

Vertice* Poligono::Antihorario(){
    return List->Antihorario();
}
Vertice* Poligono::Vizinho(int rotacao){
    return List->Vizinho(rotacao);
}

Vertice* Poligono::Avancar(int rotacao){
    return List = List->Vizinho(rotacao);
}

Vertice* Poligono::SetV(Vertice* v){
    return List = v;
}

Vertice* Poligono::Push(Ponto &p){
    if(m_size++ == 0)
        List = new Vertice(p);
    else
        List = List->Push(new Vertice(p));
    return List;
}

void Poligono::Pop(){
    Vertice* aux = List;
    List = (--m_size == 0) ? nullptr : List->Antihorario();
    delete aux->Pop();
}

/*
Aresta& Poligono::GetAresta(){
    return Aresta(GetPonto(), List->Horario()->GetPonto());
}
*/

}// NAMESPACE SPATIAL DATA
/*
Poligono(Vertice*);
~Poligono();
Vertice* GetVertice();
unsigned GetSize();
Ponto GetPonto();
Aresta GetAresta();
Vertice* Avancar(int);
Vertice* SetV(Vertice*);
Vertice* Push(Ponto&);
void Pop();
Poligono* Split(Vertice*);

} // NAMESPACE SPATIAL DATA
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
