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

Node::Node(unsigned nivel, vector<Chave>& itens):
    m_Nivel(nivel), Chaves(itens){
}

Node::Node(streampos& no){
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
            this->m_Nivel = nivel;
            this->Chaves = temp;
            file.close();
        }
        else{
            cerr << "Página inválida! Reorganize antes de fazer outra requisição." << endl;
            file.close();
            exit(10);
        }
    }
    cerr << "Arquivo: " << RTREE_FILE << " não foi aberto." << endl;
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

list<Node*>* RTree::Traversal(streampos& raizPos, Ponto& P){
    list<Node*>* resultado = new list<Node*>;
    Node* no = new Node(raizPos);
    if(no->Folha())
        resultado->push_back(no);
    else{
        for(auto chave: no->Chaves){
            if(chave.MBR.Contem(P)){
                resultado->splice(resultado->end(), *(Traversal(chave.ChildPtr, P)));
            }
        }
        delete no;
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
    list<pair<Node*, double>> contem;
    for(auto chaves: no->Chaves){
        //chaves.MBR.Contem(FigureToInsert); IMPLEMENTAR MÉTODO DE INTERSECÇÃO DE RETANGULOS
        if(chaves.MBR.Contem(FigureToInsert)){
            Node* ptrNo = new Node(chaves.ChildPtr);
            double area = chaves.MBR.GetArea();
            pair<Node*, double> candidato = make_pair(ptrNo, area);
            contem.push_back(candidato);
        }
    }

}
*/
bool Node::Folha(){
    return !m_Nivel;
}

bool Node::Overflow(){
    return (Chaves.size() > MAXCHAVES)?true:false;
}

} // NAMESPACE SPATIALINDEX
