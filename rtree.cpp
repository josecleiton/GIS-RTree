#include "rtree.hpp"
#include "insertwindow.hpp"
#include "variaveis.hpp"
namespace SpatialIndex{

Chave::Chave(Retangulo& _mbr, streampos& _dado, bool folha): MBR(_mbr){
    if(folha)
        this->Dado = _dado;
    else
        this->ChildPtr = _dado;
}

Node::Node(unsigned nivel, unsigned count, vector<Chave>& itens):
    m_Nivel(nivel), m_Count(count), Chaves(itens){
}

RTree::RTree(): count(0){
}

bool RTree::IsEmpty(){
    return !count;
}

Node* RTree::GetPtr(){
    return raiz;
}

streampos& RTree::GetPos(){
    return posRaiz;
}

Node* RTree::ReadPage(streampos& no){
    ifstream file(RTREE_FILE, ios::binary);
    if(file.is_open()){
        bool active;
        file.seekg(no);
        file.read(reinterpret_cast<char*>(&active), sizeof(bool));
        if(active){
            vector<Chave> temp;
            struct Chave* key = nullptr;
            unsigned int nivel, count;
            file.read(reinterpret_cast<char*>(&nivel), sizeof(unsigned));
            file.read(reinterpret_cast<char*>(&count), sizeof(unsigned));
            for(unsigned i=0; i<count; i++){
                file.read(reinterpret_cast<char*>(key), sizeof(struct Chave));
                temp.push_back(*key);
            }
            Node* resultado = new Node(nivel, count, temp);
            file.close();
            return resultado;
        }
        else{
            cerr << "Página inválida! Reorganize antes de fazer outra requisição." << endl;
            file.close();
            exit(10);
        }
    }
    cerr << "Arquivo: " << RTREE_FILE << " não foi aberto." << endl;
    return nullptr;
}

list<Node*>* RTree::Traversal(streampos& raizPos, Ponto& P){
    list<Node*>* resultado = new list<Node*>;
    Node* no = ReadPage(raizPos);
    if(no->Folha())
        resultado->push_back(no);
    else{
        for(auto chave: no->Chaves){
            if(chave.MBR.Contem(P)){
                resultado->splice(resultado->end(), *(Traversal(chave.ChildPtr, P)));
            }
        }
    }
    return resultado;
}

list<streampos>* RTree::Busca(Ponto& P){
    list<streampos>* resultado = new list<streampos>;
    list<Node*>* SL = Traversal(root.GetPos(), P);
    for(auto no: *SL){
        if(no->Folha()){
            for(auto chave: no->Chaves){
                if(chave.MBR.Contem(P))
                    resultado->push_back(chave.Dado);
            }
        }
    }
    return resultado;
}
void RTree::Inserir(Retangulo& FigureToInsert, const streampos& pos){
}

/*
void RTree::Inserir(Retangulo& FigureToInsert, const streampos& pos){
    Node* no = root.GetPtr();
    while(!no->Folha())
        no = EscolhaSubArvore(no, FigureToInsert);
    if(InsertInLeaf(no, FigureToInsert, pos))
        DividirEAjustar(no);
    else
        AjustaCaminho(no);
}

Node* RTree::EscolhaSubArvore(Node* no, Retangulo& FigureToInsert){
    for(auto chaves: no->Chaves){
        //chaves.MBR.Contem(FigureToInsert); IMPLEMENTAR MÉTODO DE INTERSECÇÃO DE RETANGULOS
    }
}
*/
bool Node::Folha(){
    return !m_Nivel;
}

bool Node::Overflow(){
    return (m_Count > MAXCHAVES)?true:false;
}

} // NAMESPACE SPATIALINDEX
