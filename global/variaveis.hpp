#ifndef VARIAVEIS_HPP
#define VARIAVEIS_HPP
#include "rtree.hpp"
#include "disk.hpp"

//ARQUIVO PARA A DEFINIÇÃO DE VARIÁVEIS GLOBAIS

extern SpatialIndex::RTree root;
extern DiskAPI::Disk io;  // UMA API VAGABUNDA PARA AJUDAR NOS ACESSOS AO DISCO

#endif // VARIAVEIS_HPP
