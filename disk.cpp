#include "disk.hpp"

namespace DiskAPI{
Disk::Disk(string name, bool append){
    if(append)
        file.open(name, ios::binary|ios::app);
    else
        file.open(name, ios::binary|ios::out);
    if(!file.is_open())
        cout << "Arquivo não encontrado no disco." << endl;
}

Disk::~Disk(){
    if(file.is_open())
        file.close();
}

bool Disk::SalvarForma(unsigned char& _tipo, unsigned& numeroVertices, Vertice* _vertices){
    /*
     * SALVA UM REGISTRO NO SEGUINTE FORMATO:
     * struct Registro{
     *  bool isActive = true;
     *  char tipo;
     *  unsigned numeroVertices;
     *  lista de vertices;
     *  char delimitador[2];
     * }
     * Totalizando: (8+numeroVertices*16) bytes
     */
    if(file.is_open()){
        bool active = true; // BYTE UTILIZADO PARA A REMOÇÃO LÓGICA DE UM REGISTRO
        file.write(reinterpret_cast<char*>(&active), sizeof(bool));
        file.write(reinterpret_cast<char*>(&_tipo), sizeof(unsigned char));
        file.write(reinterpret_cast<char*>(&numeroVertices), sizeof(unsigned));
        for(unsigned i=0; i<numeroVertices; i++, _vertices->Horario()){
            file.write(reinterpret_cast<char*>(&(_vertices->x)), sizeof(double));
            file.write(reinterpret_cast<char*>(&(_vertices->y)), sizeof(double));
        }
        unsigned char delimitador[2] = {246,232};
        file.write(reinterpret_cast<char*>(delimitador), sizeof(unsigned char)*2);
        return true;
    }
    cout << "Forma geométrica não pode ser salva no disco." << endl;
    return false;
}

streamoff Disk::GetTell(){
    return file.tellp();
}

} // DISK API NAMESPACE
