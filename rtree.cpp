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

Chave::Chave(const Chave& K){
    this->ChildPtr = K.ChildPtr;
    this->Dado = K.Dado;
    this->MBR = K.MBR;
}

Node::Node(Retangulo& R, streampos& PosR){
    Chave k(R, PosR, FOLHA);
    Chaves.push_back(k);
    Nivel = FOLHA;
}

Node::Node(unsigned nivel, vector<Chave>& itens):
    Nivel(nivel), Chaves(itens){
}

Node::Node(vector<Node*>& K){
    Nivel = INTERNO;
    for(auto &item: K){
        Retangulo aux(item->GetRetangulo());
        Chave A(aux, item->DiskPos, INTERNO);
        Chaves.push_back(A);
        delete item;
    }
    this->SalvarNo();
}

Node::Node(streampos& no){
    fstream file(RTREE_FILE, fstream::binary|fstream::in);
    if(file.is_open()){
        bool active;
        file.seekg(no);
        file.read(reinterpret_cast<char*>(&active), sizeof(bool));
        if(active){
            vector<Chave> temp;
            Chave* key = new Chave;
            unsigned int nivel, count;
            file.read(reinterpret_cast<char*>(&nivel), sizeof(unsigned));
            file.read(reinterpret_cast<char*>(&count), sizeof(unsigned));
            temp.resize(count);
            for(unsigned i=0; i<count; i++){
                file.read(reinterpret_cast<char*>(key), sizeof(Chave));
                temp[i] = *key;
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
        if(DiskPos)
            file.seekp(this->DiskPos);
        else{
            file.seekp(0, fstream::end);
            this->DiskPos = file.tellp();
        }
        file.write(reinterpret_cast<char*>(&active), sizeof(bool));
        file.write(reinterpret_cast<char*>(&(this->Nivel)), sizeof(unsigned));
        file.write(reinterpret_cast<char*>(&count), sizeof(unsigned));
        for(unsigned i=0; i<MAXCHAVES; i++){
            if(i<count)
                file.write(reinterpret_cast<char*>(&(this->Chaves[i])), sizeof(Chave));
            else
                file.write(reinterpret_cast<char*>(key), sizeof(Chave));
        }
        file.close();
        delete key;
    }
    else
        cerr << "Arquivo: " << RTREE_FILE << " não foi aberto." << endl;
    return this->DiskPos;
}
/*
bool Node::Cresce(Retangulo& EntryMBR, unsigned indexChave){
    bool cresceu = false;
    //this->Chaves[indexChave].MBR.CresceParaConter(EntryMBR, cresceu);
    return cresceu;
}
*/

Retangulo Node::GetRetangulo(){
    Ponto P1(DBL_MAX, DBL_MAX), P2(-DBL_MAX, -DBL_MAX);
    for(auto i: Chaves){
        Ponto aux1 = i.MBR.GetDiagonal().GetOrigem();
        Ponto aux2 = i.MBR.GetDiagonal().GetDestino();
        if(aux1 <= P1)
            P1 = aux1;
        if(aux2 >= P2)
            P2 = aux1;
    }
    return Retangulo(P1,P2);
}

RTree::RTree(){
    if(!ArquivoVazio()){
        fstream file(RTREE_FILE, fstream::binary|fstream::in);
        if(file.is_open()){
            streampos PosicaoDaRaiz;
            size_t count, nivel;
            file.read(reinterpret_cast<char*>(&PosicaoDaRaiz), sizeof(streampos));
            file.read(reinterpret_cast<char*>(&count), sizeof(size_t));
            file.read(reinterpret_cast<char*>(&nivel), sizeof(size_t));
            this->count = count;
            this->nivel = nivel;
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
        file.write(reinterpret_cast<char*>(&(this->count)), sizeof(size_t));
        file.write(reinterpret_cast<char*>(&(this->nivel)), sizeof(size_t));
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

stack<NodeAux>* RTree::Busca(Retangulo& R){
    Node* no = root.GetPtr();
    stack<NodeAux>* Caminho = new stack<NodeAux>;
    while(no != nullptr and !no->Folha())
        no = EscolhaSubArvore(no, *Caminho, R, true);
    if(no == nullptr){
        NodeAux aux;
        Caminho->push(aux);
    }
    return Caminho;
}

void RTree::CriaArvore(Retangulo& MbrForma, streampos& pos){
    fstream file(RTREE_FILE, fstream::binary|fstream::out|fstream::in);
    if(file.is_open()){
        Node* raiz = new Node(MbrForma, pos);
        streampos posicao = 1;
        size_t count, nivel;
        this->count = count = 1ull;
        this->nivel = nivel = 0ull;
        file.write(reinterpret_cast<char*>(&posicao), sizeof(streampos));
        file.write(reinterpret_cast<char*>(&count), sizeof(size_t));
        file.write(reinterpret_cast<char*>(&nivel), sizeof(size_t));
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
    if(no == nullptr)
        return CriaArvore(MbrForma, pos);
    stack<NodeAux> CaminhoNo;
    while(!no->Folha())
        no = EscolhaSubArvore(no, CaminhoNo, MbrForma, false);
    if(!CaminhoNo.empty())
        CaminhoNo.pop();
    InserirNaFolha(no, CaminhoNo, MbrForma, pos);
    LiberaPilha(CaminhoNo);
}

Node* RTree::EscolhaSubArvore(Node* &no, stack<NodeAux>& caminho, Retangulo& MbrForma, bool busca){
    vector<pair<NodeAux, double>> contem;
    NodeAux temp;
    temp.ptr = no;
    unsigned index = 0;
    for(auto chaves: no->Chaves){
        if(chaves.MBR.Contem(MbrForma)){
            Node* ptrNo = new Node(chaves.ChildPtr);
            double area = chaves.MBR.GetArea();
            NodeAux aux(ptrNo, index);
            pair<NodeAux, double> candidato = make_pair(aux, area);
            contem.push_back(candidato);
        }
        index++;
    }
    if(contem.size()){
        Node* resultado = nullptr;
        if(contem.size()>1){
            sort(contem.begin(), contem.end(), [](const pair<NodeAux, double>& A, const pair<NodeAux, double>& B){return A.second < B.second;});
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
    if(!busca){
        // SE NENHUMA CHAVE CONTER A FORMA, ESCOLHA O QUE PRECISA CRESCER MENOS (menor crescimento da área)
        pair<double, unsigned> escolha = make_pair(DBL_MAX, 0);
        for(unsigned i=0; i < no->Chaves.size(); i++){
            double aux = no->Chaves[i].MBR.CresceParaConter(MbrForma).GetArea();
            if(aux < escolha.first)
                escolha = make_pair(aux, i);
        }
        temp.index = escolha.second;
        caminho.push(temp);
        Node* ptrNo = new Node(no->Chaves[escolha.second].ChildPtr);
        return ptrNo;
    }
    return nullptr;
}


void RTree::InserirNaFolha(Node* &No, stack<NodeAux>& Caminho, Retangulo& EntryMBR, streampos& EntryPOS){
    Chave inserir(EntryMBR, EntryPOS, FOLHA);
    No->Chaves.push_back(inserir);
    if(No->Overflow())
        return DividirEAjustar(No, Caminho);
    No->SalvarNo();
    AjustaCaminho(No, Caminho);
}

void RTree::AjustaCaminho(Node* &no, stack<NodeAux>& caminho){
    if(no == raiz) return;
    Retangulo R = no->GetRetangulo();
    delete no;
    NodeAux pai = caminho.top();
    caminho.pop();
    if(pai.ptr->Ajusta(R, pai.index)){
        pai.ptr->SalvarNo();
        AjustaCaminho(pai.ptr, caminho);
    }
}

void RTree::DividirEAjustar(Node* &no, stack<NodeAux>& caminho){
    bool EhRaiz = (no == raiz);
    Node* novoNo = Divide(no);
    count++; // quantidade de nós na árvore cresce em 1
    if(EhRaiz)
        CriaNovaRaiz(no, novoNo);
    else{
        NodeAux pai = caminho.top();
        Retangulo R = no->GetRetangulo();
        pai.ptr->Ajusta(R, pai.index);
        InserirNo(novoNo, pai.ptr, caminho);
    }

}

bool Node::Ajusta(Retangulo& MBR, unsigned index){
    bool modificado = false;
    Chaves[index].MBR.Ajusta(MBR, modificado);
    return modificado;
}

void RTree::InserirNo(Node* &NoParaInserir, Node* &NoInterno, stack<NodeAux>& caminho){
    Retangulo R1 = NoParaInserir->GetRetangulo();
    Chave ChaveParaInserir(R1, NoParaInserir->DiskPos, INTERNO);
    delete NoParaInserir;
    NoInterno->Chaves.push_back(ChaveParaInserir);
    if(NoInterno->Overflow())
        return DividirEAjustar(NoInterno, caminho);
    NoInterno->SalvarNo();
    AjustaCaminho(NoInterno, caminho);
}

Node* RTree::Divide(Node* &no){
    Retangulo J;
    pair<unsigned, unsigned> escolhas;
    unsigned lenNo = static_cast<unsigned>(no->Chaves.size());
    double worst = 0.0, d1, d2, expansion = 0.0;
    for(unsigned i=0; i < lenNo; i++){
        for(unsigned j=i+1; j < lenNo; j++){
            J = no->Chaves[i].MBR.CresceParaConter(no->Chaves[j].MBR);
            double k = J.GetArea() - no->Chaves[i].MBR.GetArea() - no->Chaves[j].MBR.GetArea();
            if(k > worst){
                worst = k;
                escolhas = make_pair(i, j);
            }
        }
    }
    vector<Chave> ChavesRestantes, G1(1), G2(1);
    G1[0] = no->Chaves[escolhas.first];
    G2[0] = no->Chaves[escolhas.second];
    Node* NoG1 = new Node(no->Nivel, G1);
    Node* NoG2 = new Node(no->Nivel, G2);
    Node* BestGroup = nullptr;
    unsigned BestKey;
    Retangulo Aux1, Aux2;
    for(auto &item: no->Chaves)
        if(item != *(G1.begin()) and item != *(G2.begin()))
            ChavesRestantes.push_back(item);
    while(!ChavesRestantes.empty()){
        unsigned i = 0;
        for(auto &item: ChavesRestantes){
            Aux1 = NoG1->GetRetangulo();
            Aux2 = NoG2->GetRetangulo();
            d1 = item.MBR.CresceParaConter(Aux1).GetArea()-item.MBR.GetArea();
            d2 = item.MBR.CresceParaConter(Aux2).GetArea()-item.MBR.GetArea();
            if(d2 - d1 > expansion){
                BestGroup = NoG1;
                BestKey = i;
                expansion = d2 - d1;
            }
            if(d1 - d2 > expansion){
                BestGroup = NoG2;
                BestKey = i;
                expansion = d1 - d2;
            }
            i++;
        }
        BestGroup->Chaves.push_back(ChavesRestantes[i]);
        ChavesRestantes.erase(ChavesRestantes.begin()+i);
        if(NoG1->Chaves.size() == MINCHAVES - ChavesRestantes.size()){
            for(auto &item: ChavesRestantes)
                NoG1->Chaves.push_back(item);
            ChavesRestantes.clear();
        }
        else if(NoG2->Chaves.size() == MINCHAVES - ChavesRestantes.size()){
            for(auto &item: ChavesRestantes)
                NoG2->Chaves.push_back(item);
            ChavesRestantes.clear();
        }

    }
    swap(no->DiskPos, NoG1->DiskPos);
    delete no;
    no = NoG1;
    no->SalvarNo();
    NoG2->SalvarNo();
    return NoG2;
}

void RTree::CriaNovaRaiz(Node* &no, Node* &novoNo){
    vector<Node*> v(2);
    v[0] = no;
    v[1] = novoNo;
    Node* novaRaiz = new Node(v);
    raiz = novaRaiz;
    count++;
    nivel++;
}

bool Node::Folha(){
    return Nivel == FOLHA;
}

bool Node::Overflow(){
    return (Chaves.size() > MAXCHAVES)?true:false;
}

void LiberaPilha(stack<NodeAux>& pilha){
    while(!pilha.empty()){
        if(pilha.top().ptr != nullptr)
            delete pilha.top().ptr;
        pilha.pop();
    }
}

bool operator==(const Chave& A, const Chave& B){
    return (A.ChildPtr == B.ChildPtr or A.Dado == B.Dado) and A.MBR == B.MBR;
}

bool operator!=(const Chave& A, const Chave& B){
    return !(A == B);
}

bool operator<(const Chave& A, const Chave& B){
    return A.MBR < B.MBR;
}
bool operator>(const Chave& A, const Chave& B){
    return A.MBR > B.MBR;
}

} // NAMESPACE SPATIALINDEX
