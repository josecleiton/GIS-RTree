#ifndef RTREE_HPP
#define RTREE_HPP
#include "stdlib.hpp"
#include "SpatialData.hpp"
using namespace SpatialData;
namespace SpatialIndex{
#define DEGREE 4 // Ordem da árvore

class Node{
    friend class RTree;
    typedef Retangulo BoundingRectangle;
    size_t m_count; // Contador de elementos no nó
    size_t m_level; // Nível do nós em relação a raiz
    union{
        vector<Node*> ChildPointers; // Vetor de Ponteiros para outros nós (se este nó não for uma folha)
        //vector<DataElement> Elements; // Vetor de Elementos (se este nó for uma folha)
    };
    vector<string> Identifier; // Vetor de Identificadores para cada elemento no nó
    vector<BoundingRectangle> Mbb; // Vetor de Retangulos

    void SetCount(size_t Count){ m_count = Count; }
    void SetLevel(size_t Level){ m_level = Level; }
public:
    Node(): // construtor da classe
        Identifier(DEGREE-1){
        if(IsInternalNode()){
            ChildPointers.resize(DEGREE);
        }
    }
    bool IsInternalNode() { return (m_level); } // Se ele for um nó interno, ele não está no nível 0
    bool IsLeaf() { return (!m_level); } // Se ele for um nó-folha ele está no nível 0
    bool IsFull() { return (m_count == DEGREE-1); } // O nó está cheio se houver 3 elementos nele
};

class RTree{
private:
    Node* root{}; // Raiz da árvore
    size_t count; // Contador de nós na árvores
public:
    RTree();
};
}

#endif // RTREE_HPP
