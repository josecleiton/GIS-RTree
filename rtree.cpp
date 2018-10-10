#include "rtree.hpp"
#include "insertwindow.hpp"
#include "variaveis.hpp"
namespace SpatialIndex{

Chave::Chave(Retangulo& _mbr, streampos& _dado, int id): MBR(_mbr){
    if(id == FOLHA)
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
            this->DiskPos = no;
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
//void RTree::Inserir(Retangulo& FigureToInsert, const streampos& pos){

//}


void RTree::Inserir(Retangulo& MbrForma, const streampos& pos){
    Node* no = root.GetPtr();
    while(!no->Folha())
        no = EscolhaSubArvore(no, MbrForma);
    if(InserirNaFolha(no, MbrForma, pos))
        DividirEAjustar(no);
    else
        AjustaCaminho(no);
}

Node* RTree::EscolhaSubArvore(Node* no, Retangulo& MbrForma){
    vector<pair<Node*, double>> contem;
    for(auto chaves: no->Chaves){
        chaves.MBR.Contem(MbrForma);
        if(chaves.MBR.Contem(MbrForma)){
            Node* ptrNo = new Node(chaves.ChildPtr);
            double area = chaves.MBR.GetArea();
            pair<Node*, double> candidato = make_pair(ptrNo, area);
            contem.push_back(candidato);
        }
    }

    if(contem.size()){
        Node* resultado = contem.front().first;
        contem.front().first = nullptr;
        if(contem.size()>1){
            sort(contem.begin(), contem.end(), comparacao);
            for(auto candidatos: contem)
                if(candidatos.first != nullptr)
                    delete candidatos.first;
        }
        return resultado;
    }
    else{ // SE NENHUM NO PODER CONTER AF ORMA, ESCOLHA O QUE PRECISA CRESCER MENOS

    }

}


bool RTree::InserirNaFolha(Node* caminho, Retangulo& EntryMBR, streampos& EntryPOS){

    if(caminho->Chaves.size()+1 > MAXCHAVES)
        return true;
    Chave inserir(EntryMBR, EntryPOS, FOLHA);
    caminho->Chaves.push_back(inserir);

    return false;
}

bool Node::Folha(){
    return !Nivel;
}

bool Node::Overflow(){
    return (Chaves.size() > MAXCHAVES)?true:false;
}

} // NAMESPACE SPATIALINDEX
