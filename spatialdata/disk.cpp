#include "disk.hpp"

namespace DiskAPI{
Disk::Disk(string name, bool append){
    if(append)
        file.open(name, fstream::binary|fstream::app|fstream::in);
    else
        file.open(name, fstream::binary|fstream::out);
    Verifica();
}

Disk::Disk(string name){
    file.open(name, fstream::app|fstream::binary|fstream::in);
    Verifica();
}

void Disk::Verifica(){
    if(!file.is_open()){
        cerr << "Arquivo não encontrado no disco." << endl;
        exit(-1);
    }
}

Disk::~Disk(){
    if(file.is_open())
        file.close();
}

streampos Disk::Salvar(unsigned char _tipo, unsigned& numeroVertices, Vertice* _vertices){
    /*
     * SALVA UM REGISTRO NO SEGUINTE FORMATO:
     * struct Registro{
     *  bool isActive = true;
     *  char tipo;
     *  unsigned numeroVertices;
     *  lista de vertices;
     * }
     */
    if(file.is_open()){
        bool active = true; // BYTE UTILIZADO PARA A REMOÇÃO LÓGICA DE UM REGISTRO
        file.seekp(0, fstream::end);
        streampos pos = file.tellp();
        file.write(reinterpret_cast<char*>(&active), sizeof(bool));
        file.write(reinterpret_cast<char*>(&_tipo), sizeof(unsigned char));
        file.write(reinterpret_cast<char*>(&numeroVertices), sizeof(unsigned));
        for(unsigned i=0; i<numeroVertices; i++, _vertices = _vertices->Horario()){
            file.write(reinterpret_cast<char*>(&(_vertices->x)), sizeof(double));
            file.write(reinterpret_cast<char*>(&(_vertices->y)), sizeof(double));
        }
        return pos;
    }
    else{
        cerr << "Forma geométrica não pode ser salva no disco." << endl;
        exit(-1);
    }
}

streampos Disk::Salvar(Circulo& C){
    if(file.is_open()){
        bool active = true;
        streampos pos = file.tellp();
        unsigned char k = CIRCULO;
        file.write(reinterpret_cast<char*>(&active), sizeof(bool));
        file.write(reinterpret_cast<char*>(&k), sizeof(unsigned char));
        file.write(reinterpret_cast<char*>(&C), sizeof(Circulo));
        return pos;
    }
    else{
        cerr << "Forma geométrica não pode ser salva no disco." << endl;
        exit(-1);
    }
}

Registro* Disk::Read(streampos& pos){
    if(file.is_open()){
        file.seekg(pos, fstream::beg);
        bool active = false;
        file.read(reinterpret_cast<char*>(&active), sizeof(bool));
        if(active){ // REGISTRO ATIVO
            unsigned char tipo;
            unsigned numeroVertices = 0;
            double x,y;
            Vertice* Lista = nullptr, *temp = nullptr;
            Registro* Reg = nullptr;
            file.read(reinterpret_cast<char*>(&tipo), sizeof(unsigned char));
            if(tipo != CIRCULO){
                file.read(reinterpret_cast<char*>(&numeroVertices), sizeof(unsigned));
                for(unsigned i=0; i<numeroVertices; i++){
                    file.read(reinterpret_cast<char*>(&x), sizeof(double));
                    file.read(reinterpret_cast<char*>(&y), sizeof(double));
                    if(Lista == nullptr)
                        Lista = new Vertice(x,y);
                    else{
                        temp = new Vertice(x,y);
                        Lista->Push(temp);
                    }
                }
            }
            else{
                Circulo aux;
                file.read(reinterpret_cast<char*>(&aux), sizeof(Circulo));
                if(Lista == nullptr){
                    Lista = new Vertice(aux.centro);
                    Vertice* temp = new Vertice(aux.raio, aux.raio);
                    Lista->Push(temp);
                }
            }
            Reg = new Registro(tipo, Lista, numeroVertices);
            return Reg;
        }
        clog << "Registro na posição: " << pos << " está inativo." << endl;
    }
    return nullptr;
}

Registro::Registro(unsigned char type, Vertice* v, unsigned t): tipo(type), lista(v), tam(t){
}

Registro::~Registro(){
    if(this->tam and this->lista != nullptr) this->lista->Kai();
}

void* Registro::Conversao(){ // SE A CONVERSÃO FOR PARA PONTO OU ARESTA, PRECISO DO delete
    if(tipo == POLIGONO or tipo == POLIGONO_NAO_CONVEXO){
        Poligono* P = new Poligono(lista);
        return P;
    }
    else if(tipo == LINHA){
        Ponto a, b;
        a = lista->GetPonto();
        b = lista->Horario()->GetPonto();
        Aresta* A = new Aresta(a, b);
        return A;
    }
    else if(tipo == PONTO){
        Ponto* P = new Ponto(lista->GetPonto());
        return P;
    }
    else if(tipo == CIRCULO){
        Circulo* c = new Circulo(lista->Horario()->GetX(), lista->GetPonto());
        return c;
    }
    cerr << "Retornando a própria lista de vértices, tipo não suportado para conversão" << endl;
    return lista;
}

void Disk::Remove(streampos& pos){
    if(file.is_open()){
        bool active = false;
        file.seekp(pos);
        file.write(reinterpret_cast<char*>(&active), sizeof(bool));
    }
    else{
        cerr << "Arquivo não abriu. " << endl;
        exit(-40);
    }

}

} // DISK API NAMESPACE
