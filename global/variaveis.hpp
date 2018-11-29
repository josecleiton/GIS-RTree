#ifndef VARIAVEIS_HPP
#define VARIAVEIS_HPP
#define ICON "../GIS/ui/jorge.png"
#include "rtree.hpp"
#include "disk.hpp"

//ARQUIVO PARA A DEFINIÇÃO DE VARIÁVEIS GLOBAIS
extern fstream RTreeFile; // BUFFER QUE CARREGA O ARQUIVO RTREE_FILE USADO
//                           COMO STREAM DOS NÓS DA ÁRVORE R
extern SpatialIndex::RTree root;
extern DiskAPI::Disk io;  // UMA API VAGABUNDA PARA AJUDAR NOS ACESSOS AO DISCO

#endif // VARIAVEIS_HPP
