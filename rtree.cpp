#include "rtree.hpp"
#include "insertwindow.hpp"
namespace SpatialIndex{

Chave::Chave(Retangulo& _mbr, streampos& _dado): MBR(_mbr), Dado(_dado){
}

Chave::Chave(Retangulo& _mbr, Node* ptr): MBR(_mbr), ChildPointer(ptr){

}

Node::Node(int nivel, int count): m_Nivel(nivel), m_Count(count){

}

RTree::RTree(): raiz(nullptr), count(0){
}

void RTree::Push(Retangulo&, const streampos&){
}

bool RTree::IsEmpty(){
    return !count;
}

Node* RTree::GetPtr(){
    return raiz;
}

list<Node*> RTree::Traversal(Node* raiz, Ponto& P){
    list<Node*> resultado;
    if(raiz->Folha())
        resultado.push_back(raiz);
    else{
        for(auto it: raiz->Chaves){
            if(it.MBR.Contem(P)){

            }
        }
    }
}

list<streampos&> RTree::Busca(Ponto& P){

}

bool Node::Folha(){
    return !m_Nivel;
}

} // NAMESPACE SPATIALINDEX
