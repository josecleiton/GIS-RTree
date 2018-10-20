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

Chave::Chave(){
    Ponto A, B;
    Retangulo vazio(A, B);
    this->MBR = vazio;
}

Node::Node(Retangulo& R, streampos& PosR){
    Chave k(R, PosR, FOLHA);
    Chaves.push_back(k);
    Nivel = FOLHA;
}

Node::Node(unsigned nivel, vector<Chave>& itens):
    Nivel(nivel), Chaves(itens){
}

Node::Node(streampos& no){
    fstream file(RTREE_FILE, fstream::binary|fstream::in);
    if(file.is_open()){
        bool active;
        file.seekg(no);
        file.read(reinterpret_cast<char*>(&active), sizeof(bool));
        if(active){
            vector<Chave> temp;
            struct Chave* key = new Chave;
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
        }
    }
    else cerr << "Arquivo: " << RTREE_FILE << " não foi aberto." << endl;
}

streampos Node::SalvarNo(){
    fstream file(RTREE_FILE, fstream::binary|fstream::in|fstream::out);
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
        file.close();
        delete key;
    }
    else
        cerr << "Arquivo: " << RTREE_FILE << " não foi aberto." << endl;
    return this->DiskPos;
}

bool Node::Cresce(Retangulo& EntryMBR, unsigned indexChave){
    bool cresceu = false;
    this->Chaves[indexChave].MBR.CresceParaConter(EntryMBR, cresceu);
    return cresceu;
}

RTree::RTree(){
    if(!ArquivoVazio()){
        fstream file(RTREE_FILE, fstream::binary|fstream::in);
        if(file.is_open()){
            streampos PosicaoDaRaiz;
            unsigned count;
            file.read(reinterpret_cast<char*>(&PosicaoDaRaiz), sizeof(streampos));
            file.read(reinterpret_cast<char*>(&count), sizeof(unsigned));
            this->count = count;
            file.close();
            this->raiz = new Node(PosicaoDaRaiz);
        }
    }
    else
        this->raiz = nullptr;
}

RTree::~RTree(){
    fstream file(RTREE_FILE, fstream::binary|fstream::in|fstream::out);
    if(file.is_open()){
        file.seekp(0, fstream::beg);
        file.write(reinterpret_cast<char*>(&(this->raiz->DiskPos)), sizeof(streampos));
        file.write(reinterpret_cast<char*>(&(this->count)), sizeof(unsigned));
        file.close();
        delete this->raiz;
    }
}

bool RTree::ArquivoVazio(){
    fstream file(RTREE_FILE, fstream::binary|fstream::in);
    streampos inicio, fim;
    inicio = file.tellg();
    file.seekg(0, fstream::end);
    fim = file.tellg();
    file.close();
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
    fstream file(RTREE_FILE, fstream::binary|fstream::out|fstream::in);
    if(file.is_open()){
        Node* raiz = new Node(MbrForma, pos);
        streampos posicao = 1;
        unsigned count;
        this->count = count = 1u;
        file.write(reinterpret_cast<char*>(&posicao), sizeof(streampos));
        file.write(reinterpret_cast<char*>(&count), sizeof(unsigned));
        posicao = file.tellp();
        raiz->DiskPos = posicao;
        file.seekp(0, fstream::beg);
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
        return CriaArvore(MbrForma, pos);
    }
    stack<NodeAux> CaminhoNo;
    while(!no->Folha())
        no = EscolhaSubArvore(no, CaminhoNo, MbrForma);
    delete CaminhoNo.top().ptr;
    CaminhoNo.pop();
    InserirNaFolha(no, CaminhoNo, MbrForma, pos);
}

bool comparacaoESA(const pair<NodeAux, double>& primeiro, const pair<NodeAux, double>& segundo){
    return primeiro.second < segundo.second;
}

Node* RTree::EscolhaSubArvore(Node* no, stack<NodeAux>& caminho, Retangulo& MbrForma){
    vector<pair<NodeAux, double>> contem;
    NodeAux temp;
    bool inseriu = false;
    unsigned index = 0;
    for(auto chaves: no->Chaves){
        if(chaves.MBR.Contem(MbrForma)){
            Node* ptrNo = new Node(chaves.ChildPtr);
            double area = chaves.MBR.GetArea();
            NodeAux aux(ptrNo, index);
            pair<NodeAux, double> candidato = make_pair(aux, area);
            contem.push_back(candidato);
            if(!inseriu){
                temp.ptr = no;
                inseriu = true;
            }
        }
        index++;
    }
    if(contem.size()){
        Node* resultado = nullptr;
        if(contem.size()>1){
            sort(contem.begin(), contem.end(), comparacaoESA);
            swap(resultado, contem.front().first.ptr);
            for(auto &candidatos: contem)
                if(candidatos.first.ptr != nullptr)
                    delete candidatos.first.ptr;
        }
        else
            swap(resultado, contem.front().first.ptr);
        temp.index = contem.front().first.index;
        caminho.push(temp);
        return resultado;
    }
    else{ // SE NENHUMA CHAVE CONTER A FORMA, ESCOLHA O QUE PRECISA CRESCER MENOS (menor crescimento da área)

    }

}


void RTree::InserirNaFolha(Node* no, stack<NodeAux>& caminho, Retangulo& EntryMBR, streampos& EntryPOS){
    size_t limite = no->Chaves.size();
    Chave inserir(EntryMBR, EntryPOS, FOLHA);
    if(limite+1 > MAXCHAVES)
        return DividirEAjustar(no, caminho, inserir);
    if(!limite) // NÓ VAZIO
        no->Chaves.push_back(inserir);
    else{
        auto it = no->Chaves.begin();
        for(size_t i=0; (i<limite) and (it->MBR < EntryMBR); i++, it++)
        no->Chaves.insert(it, inserir);

    }
    no->SalvarNo();
    AjustaCaminho(no, caminho, inserir.MBR);
}

void RTree::DividirEAjustar(Node* no, stack<NodeAux>& caminho, Chave& Entry){

}

void RTree::AjustaCaminho(Node* no, stack<NodeAux>& caminho, Retangulo& EntryMBR){
    if(no == raiz) return;
    delete no;
    NodeAux pai = caminho.top();
    caminho.pop();
    if(pai.ptr->Cresce(EntryMBR, pai.index)){
        pai.ptr->SalvarNo();
        AjustaCaminho(pai.ptr, caminho, EntryMBR);
    }
}

bool Node::Folha(){
    return !Nivel;
}

bool Node::Overflow(){
    return (Chaves.size() > MAXCHAVES)?true:false;
}

} // NAMESPACE SPATIALINDEX
