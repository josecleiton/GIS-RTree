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
    file.open(name, fstream::binary|fstream::app|fstream::in);
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
        file.seekp(0, fstream::end);
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

Registro* Disk::Read(streampos pos){
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
                    Lista->Push(new Vertice(aux.raio, aux.raio));
                }
                numeroVertices = 1;
            }
            Reg = new Registro(tipo, Lista, numeroVertices);
            return Reg;
        }
        clog << "Registro na posição: " << pos << " foi ignorado por estar inativo." << endl;
    }
    return nullptr;
}

bool Disk::RemoveAll(){
    file.close();
    if(remove(FILENAME))
        cerr << "Failed to delete " << FILENAME << ": " << strerror(errno) << '\n';
    else
        clog << "Arquivo " << FILENAME << " excluido." << '\n';
    file.open(FILENAME, fstream::binary|fstream::app|fstream::in);
    if(file.is_open()) return true;
    cerr << "Arquivo: " << FILENAME << " não abriu.";
    exit(-40);
}

void Disk::CleanDir(QString path, QString rule){
    QDirIterator it(path, QStringList() << rule, QDir::Files);
    while(it.hasNext()){
        QFile f(it.next());
        f.remove();
    }
}

Registro::Registro(unsigned char type, Vertice* v, unsigned t): tipo(type), lista(v), tam(t){
}

Registro::~Registro(){
    if(this->tam and this->lista != nullptr) this->lista->Kai();
}

void* Registro::Conversao(){ // SE A CONVERSÃO FOR PARA PONTO OU ARESTA, PRECISO DO delete
    if(tipo == POLIGONO or tipo == POLIGONO_NAO_CONVEXO)
        return new Poligono(lista);
    else if(tipo == LINHA)
        return new Aresta(lista->GetPonto(), lista->Horario()->GetPonto());
    else if(tipo == PONTO)
        return new Ponto(lista->GetPonto());
    else if(tipo == CIRCULO)
        return new Circulo(lista->Horario()->GetX(), lista->GetPonto());
    clog << "Retornando a própria lista de vértices, tipo não suportado para conversão" << endl;
    return lista;
}

string Registro::StrTipo(){
    if(tipo == POLIGONO or tipo == POLIGONO_NAO_CONVEXO) return "POLIGONO";
    else if(tipo == LINHA) return "LINHA";
    else if(tipo == PONTO) return "PONTO";
    else if(tipo == CIRCULO) return "CIRCULO";
    else return "INDEFINIDO";
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
