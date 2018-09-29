#include "disk.hpp"

namespace DiskAPI{
Disk::Disk(string name){
    file.open(name, ios::binary|ios::out);
    if(!file.is_open())
        cout << "Arquivo não encontrado no disco." << endl;
}

Disk::~Disk(){
    if(file.is_open())
        file.close();
}

bool Disk::SalvarForma(char _tipo, unsigned tam, Vertice* _vertices){
    if(file.is_open()){
        file.write(reinterpret_cast<char*>(&_tipo), sizeof(char));
        file.write(reinterpret_cast<char*>(&tam), sizeof(unsigned));
        for(unsigned i=0; i<tam; i++, _vertices->Horario()){
            file.write(reinterpret_cast<char*>(&(_vertices->x)), sizeof(double));
            file.write(reinterpret_cast<char*>(&(_vertices->y)), sizeof(double));
        }
        char delimitador[2] = {static_cast<char>(246),static_cast<char>(232)};
        file.write(delimitador, sizeof(char)*2);
        return true;
    }
    cout << "Forma geométrica não pode ser salva no disco." << endl;
    return false;
}

} // DISK API NAMESPACE
