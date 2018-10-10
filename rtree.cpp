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
/*
Chave::Chave(){
    Ponto A, B;
    Retangulo vazio(A, B);
    this->MBR = vazio;
}
*/
Node::Node(Retangulo& R, streampos& PosR){
    Chave k(R, PosR, FOLHA);
    Chaves.push_back(k);
    Nivel = FOLHA;

}

Node::Node(unsigned nivel, vector<Chave>& itens):
    Nivel(nivel), Chaves(itens){
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
            this->Nivel = nivel;
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

streampos Node::SalvarNo(){
    ofstream file;
    file.open(RTREE_FILE, ios::binary|ios::app);
    if(file.is_open()){
        unsigned count = static_cast<unsigned>(this->Chaves.size());
        Retangulo V;
        streampos x = 0;
        bool active = true;
        Chave* key = new Chave(V, x, FOLHA);
        file.seekp(this->DiskPos);
        file.write(reinterpret_cast<char*>(&active), sizeof(bool));
        file.write(reinterpret_cast<char*>(&(this->Nivel)), sizeof(unsigned));
        file.write(reinterpret_cast<char*>(&count), sizeof(unsigned));
        for(unsigned i=0; i<MAXCHAVES; i++){
            if(i<count)
                file.write(reinterpret_cast<char*>(&(this->Chaves[i])), sizeof(struct Chave));
            else
                file.write(reinterpret_cast<char*>(key), sizeof(struct Chave));
        }
        delete key;
        file.close();
    }
    else
        cerr << "Arquivo: " << RTREE_FILE << " não foi aberto." << endl;
    return this->DiskPos;
}

RTree::RTree(){
    if(!ArquivoVazio()){
        ifstream file;
        file.open(RTREE_FILE, ios::binary);
        if(file.is_open()){
            streampos PosicaoDaRaiz;
            unsigned count;
            file.read(reinterpret_cast<char*>(&PosicaoDaRaiz), sizeof(streampos));
            file.read(reinterpret_cast<char*>(&count), sizeof(unsigned));
            this->count = count;
            this->raiz = new Node(PosicaoDaRaiz);
            file.close();
        }
    }
    else
        this->raiz = nullptr;
}

RTree::~RTree(){
    ofstream file(RTREE_FILE, ios::binary|ios::app);
    if(file.is_open()){
        file.seekp(0, ios::beg);
        file.write(reinterpret_cast<char*>(&(this->raiz->DiskPos)), sizeof(streampos));
        file.write(reinterpret_cast<char*>(&(this->count)), sizeof(unsigned));
        file.close();
        delete this->raiz;
    }
}

bool RTree::ArquivoVazio(){
    ifstream file;
    file.open(RTREE_FILE, ios::binary);
    streampos inicio, fim;
    inicio = file.tellg();
    file.seekg(0, ios::end);
    fim = file.tellg();
    return inicio == fim;
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
    list<Node*>* SL = Traversal(root.GetPtr()->DiskPos, P);
    for(auto no: *SL){
        if(no->Folha()){
            for(auto chave: no->Chaves){
                if(chave.MBR.Contem(P))
                    resultado->push_back(chave.Dado);
            }
        }
    }
    delete SL;
    return resultado;
}

void RTree::CriaArvore(Retangulo& MbrForma, streampos& pos){
    ofstream file;
    file.open(RTREE_FILE, ios::binary);
    if(file.is_open()){
        Node* raiz = new Node(MbrForma, pos);
        streampos posicao = 1;
        unsigned count = 1;
        file.write(reinterpret_cast<char*>(&posicao), sizeof(streampos));
        file.write(reinterpret_cast<char*>(&count), sizeof(unsigned));
        posicao = file.tellp();
        raiz->DiskPos = posicao;
        file.seekp(0, ios::beg);
        file.write(reinterpret_cast<char*>(&posicao), sizeof(streampos));
        this->raiz = raiz;
        raiz->SalvarNo();
    }
    else{
        cerr << "Arquivo não foi aberto, finalizando o programa." << endl;
        exit(-40);
    }
}

void RTree::Inserir(Retangulo& MbrForma, streampos& pos){
    Node* no = root.GetPtr();
    if(no == nullptr){
        CriaArvore(MbrForma, pos);
        return;
    }
    while(!no->Folha())
        no = EscolhaSubArvore(no, MbrForma);
    if(InserirNaFolha(no, MbrForma, pos))
        DividirEAjustar(no);
    else
        AjustaCaminho(no);
}

void RTree::DividirEAjustar(Node* no){

}

void RTree::AjustaCaminho(Node* no){

}

bool comparacao(const pair<Node*, double>& primeiro, const pair<Node*, double>& segundo){
    return primeiro.second < segundo.second;
}

Node* RTree::EscolhaSubArvore(Node* no, Retangulo& MbrForma){
    vector<pair<Node*, double>> contem;
    for(auto chaves: no->Chaves){
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
    caminho->SalvarNo();
    return false;
}

bool Node::Folha(){
    return !Nivel;
}

bool Node::Overflow(){
    return (Chaves.size() > MAXCHAVES)?true:false;
}

} // NAMESPACE SPATIALINDEX
