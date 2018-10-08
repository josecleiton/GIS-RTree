#include "spatialdata.hpp"
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

double operator*(const Ponto& P, const Ponto& K){
    return P.x*K.x+P.y*K.y;
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

int Ponto::Classificacao(Aresta E){
    return Classificacao(E.origem, E.destino);
}

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

double Ponto::Distancia(Aresta& E){
    Aresta ab = E;
    ab.Flip().Rotacao(); // rotacional em 90º antihorario
    Ponto n(ab.destino-ab.origem); // n = vetor perpendincular a e;
    n = (1.0/n.Tamanho())*n; // normalizando n
    Ponto k = (*this)+n;
    Aresta f(*this, k); // f = n
    double t;
    f.Interseccao(E, t);
    return t;
}

double Ponto::Tamanho(){
    return sqrt(x*x+y*y);
}

double Ponto::Distancia(Ponto& p1){
    return sqrt(pow(x-p1.x, 2)+pow(y-p1.y, 2));
}

ostream& operator<<(ostream& out, const Ponto& P){
    out << "(" << P.GetX() << ", " << P.GetY() << ")";
    return out;
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

Vertice* Vertice::Push(Ponto P){
    Vertice* temp = new Vertice(P);
    return Vertice::Push(temp);
}

Vertice* Vertice::Pop(){
    return static_cast<Vertice*>(Node::Pop());
}

void Vertice::Splice(Vertice* V){ // TENHO QUE EXPLICAR PARA OS MENINOS
    Node::Splice(V);
}

Vertice* Vertice::Split(Vertice* b){ // TENHO QUE EXPLICAR PARA OS MENINOS
    Vertice* bp = b->Antihorario()->Push(new Vertice(b->GetPonto()));
    Push(new Vertice(GetPonto()));
    Splice(bp);
    return bp;
}

// CLASS POLIGONO

Poligono::Poligono(): list(nullptr), m_size(0){
}
Poligono::Poligono(Poligono& p){
    m_size = p.m_size;
    if(!m_size)
        list = nullptr;
    else{
        list = new Vertice(p.GetPonto());
        for(unsigned i=1; i<m_size; i++){
            p.Avancar(HORARIO);
            list = list->Push(new Vertice(p.GetPonto()));
        }
        p.Avancar(HORARIO);
        list = list->Horario();
    }
}

Poligono::Poligono(Vertice* ListaVertice): list(ListaVertice){
    Resize();
}

void Poligono::Resize(){ // DEVE SER CHAMADO SEMPRE QUE UMA CADEIA DE VERTICES DE TAMANHO DESCONHECIDA FOR ADICIONADA AO POLIGONO
    if(list == nullptr)
        m_size = 0;
    else{
        m_size = 1;
        for(Vertice* v = list->Horario(); v != list; ++m_size, v = v->Horario());
    }
}

Poligono::~Poligono(){
    if(list != nullptr){
        Vertice* aux = list->Horario();
        while(list != aux){
            delete aux->Pop();
            aux = list->Horario();
        }
        delete list;
    }
}

/*
 * Funções de acesso à classe
 */
Vertice* Poligono::GetVertice(){
    return list;
}

unsigned Poligono::GetSize(){
    return m_size;
}

Ponto& Poligono::GetPonto(){
    return list->GetPonto();
}

Vertice* Poligono::Horario(){
    return list->Horario();
}

Vertice* Poligono::Antihorario(){
    return list->Antihorario();
}
Vertice* Poligono::Vizinho(int rotacao){
    return list->Vizinho(rotacao);
}

Vertice* Poligono::Avancar(int rotacao){
    return list = list->Vizinho(rotacao);
}

Vertice* Poligono::SetV(Vertice* v){
    return list = v;
}

Vertice* Poligono::Push(Ponto p){
    if(m_size++ == 0)
        list = new Vertice(p);
    else
        list = list->Push(new Vertice(p));
    return list;
}

Vertice* Poligono::Push(Ponto& p){
    if(m_size++ == 0)
        list = new Vertice(p);
    else
        list = list->Push(new Vertice(p));
    return list;
}

void Poligono::Pop(){
    Vertice* aux = list;
    list = (--m_size == 0) ? nullptr : list->Antihorario();
    delete aux->Pop();
}

Poligono* Poligono::Split(Vertice* b){
    Vertice* bp = list->Split(b);
    Resize();
    return new Poligono(bp);
}


Aresta Poligono::GetAresta(){
    return Aresta(GetPonto(), list->Horario()->GetPonto());
}

Retangulo Vertice::Envelope(){
    vector<double> X, Y;
    for(auto it = this->Antihorario(); it != this; it = it->Antihorario()){
        X.push_back(it->x);
        Y.push_back(it->y);
    }
    X.push_back(this->x);
    Y.push_back(this->y);
    sort(X.begin(), X.end());
    sort(Y.begin(), Y.end());
    Ponto origem(X.front(), Y.front());
    Ponto destino(X.back(), Y.back());
    return Retangulo(origem, destino);
}

// CLASSE ARESTA

Aresta::Aresta(Ponto& _origem, Ponto& _dest): origem(_origem), destino(_dest){
}

Aresta::Aresta(): origem(Ponto(0.0, 0.0)), destino(Ponto(0.0,0.0)){

}

Aresta& Aresta::Rotacao(){
    Ponto m = 0.5 * (origem+destino);
    Ponto v = destino - origem;
    Ponto n(v.y, -v.x);
    origem = m - 0.5 * n;
    destino = m + 0.5 * n;
    return *this;
}

Aresta& Aresta::Flip(){
    return Rotacao().Rotacao();
}

Ponto Aresta::GetPonto(double& t){
    return Ponto(origem + t * (destino-origem));
}

double Aresta::Dist() const{
    return sqrt(pow(origem.x - destino.x, 2)+pow(origem.y - destino.y, 2));
}

int Aresta::Interseccao(Aresta& E, double& t){
    Ponto a = origem;
    Ponto b = destino;
    Ponto c = E.origem;
    Ponto d = E.destino;
    Ponto n = Ponto((d-c).y, (c-d).x);
    Ponto k = b-a;
    double denom = n*k;
    if(denom == 0.0){
        int aclass = origem.Classificacao(E);
        if(aclass == ESQUERDA or aclass == DIREITA)
            return PARALELA;
        else
            return COLINEAR;
    }
    k = a-c;
    double num = n*k;
    t = -num/denom;
    return CONSECUTIVO;
}

int Aresta::Cruza(Aresta& E, double& t){
    double s;
    int TipoCruzamento = E.Interseccao(*this, s);
    if(TipoCruzamento==COLINEAR or TipoCruzamento==PARALELA)
        return TipoCruzamento;
    if(s < 0.0 or s > 1.0)
        return CONSECUTIVO_NAO_CRUZADO;
    Interseccao(E, t);
    if(t >= 0.0 and t <= 1.0)
        return CONSECUTIVO_CRUZADO;
    return  CONSECUTIVO_NAO_CRUZADO;
}

bool Aresta::isVertical(){
    double t = fabs(origem.x-destino.x);
    return (t==0.0)?true:false;
}

double Aresta::Inclinacao(){
    if(!isVertical())
        return (destino.y - origem.y)/(destino.x - origem.y);
    return DBL_MAX;
}

Retangulo::Retangulo(Ponto& origem, Ponto& destino, int _id): diagonal(origem, destino), ID(_id){
}

bool Retangulo::InterRetang(Ponto& A , Ponto& B ,Ponto& C,Ponto& D){

    Ponto Pmax, Pmin,Pmin2,Pmax2;

        Pmin.x= min(A.x,B.x);
        Pmin.y= min(A.y,B.y);
        Pmax.x= max(A.x,B.x);
        Pmax.y= max(A.y,B.y);
        Pmin2.x= min(C.x,D.x);
        Pmin2.y= min(C.y,D.y);
        Pmax2.x= max(C.x,D.x);
        Pmax2.y= max(C.y,D.y);


    if((Pmax.x >= Pmin2.x) && (Pmax2.x >= Pmin.x) && (Pmax.y >= Pmin2.y) && (Pmax2.y >= Pmin.y))
        return true;

    return false;

}

Circulo::Circulo(){
    raio= 0.0;
    centro.x=0.0;
    centro.y=0.0;
}
Circulo::Circulo(double R,Ponto centro){
    raio= R;
    centro.x= centro.x;
    centro.y=centro.y;
}
Circulo::~Circulo(){

}
double Circulo::Diametro(){
    return this->raio*2;
}
double Circulo::Comprimento(){
    return this->raio*2*PI;
}
double Circulo::Area(){
    return PI*pow(this->raio,2);
}

int Circulo::CirculoPoint(Ponto &P){

    if(pow((this->centro.x-P.y),2)+pow((this->centro.y-P.y),2)< pow(this->raio,2))
        return 1;// PONTO DENTRO DO CIRCULO
    if(pow((this->centro.x-P.y),2)+pow((this->centro.y-P.y),2)> pow(this->raio,2))
        return -1; // PONTO FORA
    else return 0; // PONTO NA CIRCUNFER?NCIA

}

int Circulo::InterCirculo( Circulo& c1, Circulo& c2)// VERIFICA SE EXISTE INTERSEÇÃO ENTRE CIRCULOS
{
    double dist= sqrt ( pow ((c2.centro.x - c1.centro.x), 2.0 ) + pow((c2.centro.y - c1.centro.y ), 2.0 ));
    double soma=c1.raio+c2.raio;
    double subt= c1.raio-c2.raio;

    if(dist>soma|| dist<fabs(subt))
        return -1; // círculos não se interceptam
    if(dist==0.0 && c1.raio==c2.raio)
        return 0; // circulos idênticos

    return 1; //exite interseção
}

Vertice* Circulo::PontinterCirculo(Circulo& c1, Circulo& c2){ //   PONTOS QUE INTERCEPTA DOIS CIRCULOS
    double dist= sqrt ( pow ((c2.centro.x - c1.centro.x), 2.0 ) + pow((c2.centro.y - c1.centro.y ), 2.0 ));
    double A , H;
    Ponto P,P1 ,P2;

    A= (pow(c1.raio,2.0)-pow(c2.raio,2.0)+pow(dist,2.0))/dist*2.0;
    H= sqrt(pow(c1.raio,2.0)) - pow(A,2.0);

    P.x=c1.centro.x + A*(c2.centro.x-c1.centro.x)/dist;
    P.y=c1.centro.y +A*(c2.centro.y-c1.centro.y)/dist;

    P1.x= P.x + H*(c2.centro.y-c1.centro.y)/dist;
    P1.y= P.y - H*(c2.centro.x-c1.centro.x)/dist;
    double soma=c1.raio+c2.raio;
    Vertice* resultado = new Vertice(P1);
    if(dist!=soma){ //intercepta em um dois
        P2.x= P.x - H*(c2.centro.y-c1.centro.y)/dist;
        P2.y= P.y + H*(c2.centro.x-c1.centro.x)/dist;
        resultado->Push(P2);
    }
    return resultado;
}

Vertice* Circulo::CirculoIntRetas(Circulo& T, Ponto& p1, Ponto& p2){
    Vertice* resultado;
    double A ,B, C, Delta;
    double u1,u2;
    Ponto R1,R2;

    A= pow((p2.x-p1.x),2.0) + pow((p2.y-p1.y),2.0);
    B= -2.0*(p1.x*p1.x+p1.y*p1.y-p1.x*p2.x-p1.y*p2.y+T.centro.x*(p2.x-p1.x)+T.centro.y*(p2.y-p1.y));
    C= pow((T.centro.x-p1.x),2.0)+ pow((T.centro.y-p1.y),2.0)- pow(T.raio,2.0);

    Delta = B*B- 4.0* A * C;

    u1 = (-B + sqrt(Delta))/(2.0*A);
    R1.x= p1.x+ u1*(p2.x-p1.x);
    R1.y=p1.y+ u1*(p2.y-p1.y);

    if(Delta==0.0) {// um ponto de interseção
        u1=-B /(2.0*A);
        R1.x= p1.x + (p2.x-p1.x)*u1;
        R1.y=p1.y + (p2.y-p1.y)*u1;
        resultado = new Vertice(R1);
    }
    else if(Delta>0) // dois pontos de interseção
    {        u1 = (-B - sqrt(Delta))/(2.0*A);
             u2 = (-B + sqrt(Delta))/(2*A);
             R1.x= p1.x + (p2.x-p1.x)*u1;
             R1.y= p1.y + (p2.y-p1.y)*u1;
             R2.x= p1.x+ (p2.x-p1.x)*u2;
             R2.y= p1.y+ (p2.y-p1.y)*u2;
             resultado = new Vertice(R1);
             resultado->Push(R2);
            // Retornar R1 e R2
    }
    return resultado;
}

bool PontoNoPoligonoConvexo(Ponto &s, Poligono &p){
    unsigned TamanhoPoligono = p.GetSize();
    if(TamanhoPoligono == 1)
        return s == p.GetPonto();
    if(TamanhoPoligono == 2){
        int aux = s.Classificacao(p.GetAresta());
        return aux == ENTRE or aux == ORIGEM or aux == DESTINO;
    }
    Vertice* origem = p.GetVertice();
    for(unsigned i=0; i<TamanhoPoligono; i++, p.Avancar(HORARIO)){
        if(s.Classificacao(p.GetAresta()) == ESQUERDA){
            p.SetV(origem);
            return false;
        }
    }
    return true;
}

Vertice* MenorVertice(Poligono &p, int (*cmp)(Ponto*, Ponto*)){
    Vertice* aux = p.GetVertice();
    p.Avancar(HORARIO);
    for(unsigned i = 1; i < p.GetSize(); p.Avancar(HORARIO), i++){
        if((*cmp)(p.GetVertice(), aux) < 0)
            aux = p.GetVertice();
    }
    p.SetV(aux);
    return aux;
}

int EsquerdaDireita(Ponto* a, Ponto* b){
    if(*a < *b) return -1;
    if(*a > *b) return 1;
    return 0;
}

int DireitaEsquerda(Ponto* a, Ponto* b){
    return EsquerdaDireita(b, a);
}

list<Poligono*> Triangulacao(Poligono& P){
    list<Poligono*> triangulos;
    //if(tamanho >= 3){
        if(P.GetSize() == 3)
            triangulos.push_back(&P);
        else{ // POLIGONO TEM MAIS DO QUE 3 LADOS
            FindVerticeConvexo(P);
            Vertice* d = FindIntrudingVertex(P);
            if(d == nullptr){ // no intruding vertex
                Vertice* c = P.Vizinho(HORARIO);
                P.Avancar(ANTIHORARIO);
                Poligono* q = P.Split(c);
                auto r = Triangulacao(P);
                triangulos.splice(triangulos.end(), r);
                triangulos.push_back(q);
            }
            else{
                Poligono* q = P.Split(d);
                auto r = Triangulacao(*q);
                auto s = Triangulacao(P);
                triangulos.splice(triangulos.end(), r);
                triangulos.splice(triangulos.end(), s);
            }
        }
    //}// ELSE BASE DA RECURSÃO
    return triangulos;
}

void FindVerticeConvexo(Poligono& p){
    Vertice* a = p.Vizinho(ANTIHORARIO);
    Vertice* b = p.GetVertice();
    Vertice* c = p.Vizinho(HORARIO);
    while(c->Classificacao(*a, *b) != DIREITA){
        a = b;
        b = p.Avancar(HORARIO);
        c = p.Vizinho(ANTIHORARIO);
    }
}

Vertice* FindIntrudingVertex(Poligono& p){
    Vertice* a = p.Vizinho(ANTIHORARIO);
    Vertice* b = p.GetVertice();
    Vertice* c = p.Avancar(HORARIO);
    Vertice* d = nullptr;
    double bestD = -1.0; // distancia ao melhor candidato;
    Aresta ca(c->GetPonto(), a->GetPonto());
    Vertice *v = p.Avancar(HORARIO);
    while(v != a){
        if(PontoNoTriangulo(*v, *a, *b, *c)){
            double dist = v->Distancia(ca);
            if(dist > bestD){
                d = v;
                bestD = dist;
            }
        }
        v = p.Avancar(HORARIO);
    }
    p.SetV(b);
    return d;
}

bool PontoNoTriangulo(Ponto& p, Ponto& a, Ponto& b, Ponto& c){
    return ((p.Classificacao(a, b) != ESQUERDA) and
            (p.Classificacao(b, c) != ESQUERDA) and
            (p.Classificacao(c, a) != ESQUERDA));
}

double Area(Poligono &P){
    double area= 0.0;
    unsigned tam = P.GetSize();
    vector<double> X(tam+1), Y(tam+1);
    for(unsigned i=0; i<P.GetSize(); i++, P.Avancar(HORARIO)){
        X[i]=P.GetPonto().x;
        Y[i]=P.GetPonto().y;
    }
    X[tam] = X[0];
    Y[tam] = Y[0];
    for(unsigned i=0, j=tam-1; i<tam; j=i, i++){
        area += (X[j]+X[i])*(Y[j]-Y[i]);
    }
    return fabs(area/2.0);
}

}// NAMESPACE SPATIAL DATA
