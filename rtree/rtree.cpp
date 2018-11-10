#include "rtree.hpp"
#include "variaveis.hpp"

namespace SpatialIndex{

static fstream RTreeFile; // BUFFER QUE CARREGA O ARQUIVO RTREE_FILE USADO
//                           COMO STREAM DOS NÓS DA ÁRVORE R

Chave::Chave(Retangulo& _mbr, streampos& _dado, unsigned id): MBR(_mbr){
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

Chave::Chave(Node*& No){
    Retangulo R1 = No->GetRetangulo();
    Chave k(R1, No->DiskPos, No->Nivel);
    *this = k;
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
    bool active = false;
    RTreeFile.seekg(no, fstream::beg);
    RTreeFile.read(reinterpret_cast<char*>(&active), sizeof(bool));
    if(active){
        unsigned count;
        RTreeFile.read(reinterpret_cast<char*>(&Nivel), sizeof(unsigned));
        RTreeFile.read(reinterpret_cast<char*>(&count), sizeof(unsigned));
        this->Chaves.resize(count);
        for(unsigned i=0; i<count; i++)
            RTreeFile.read(reinterpret_cast<char*>(&(Chaves[i])), sizeof(Chave));
        this->DiskPos = no;
    }
    else
        cerr << "Página inválida! Reorganize antes de fazer outra requisição." << endl;
}

Node::~Node(){
}

streampos Node::SalvarNo(){
    unsigned count = static_cast<unsigned>(this->Chaves.size());
    Retangulo V;
    streampos x = 0;
    bool active = true;
    Chave key(V, x, FOLHA);
    if(DiskPos)
        RTreeFile.seekp(this->DiskPos, fstream::beg);
    else{
        RTreeFile.seekp(0, fstream::end);
        this->DiskPos = RTreeFile.tellp();
    }
    RTreeFile.write(reinterpret_cast<char*>(&active), sizeof(bool));
    RTreeFile.write(reinterpret_cast<char*>(&(this->Nivel)), sizeof(unsigned));
    RTreeFile.write(reinterpret_cast<char*>(&count), sizeof(unsigned));
    for(unsigned i=0; i<MAXCHAVES; i++){
        if(i<count)
            RTreeFile.write(reinterpret_cast<char*>(&(this->Chaves[i])), sizeof(Chave));
        else
            RTreeFile.write(reinterpret_cast<char*>(&key), sizeof(Chave));
    }
    return this->DiskPos;
}

streampos Node::Delete(){
    bool active = false;
    RTreeFile.seekp(this->DiskPos, fstream::beg);
    RTreeFile.write(reinterpret_cast<char*>(&active), sizeof(bool));
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
    vector<double> a(2, DBL_MAX), b(2, DBL_MIN);
    vector<pair<double, double>> k(2);
    for(auto item: Chaves){
        k[0] = make_pair(item.MBR.GetDiagonal().GetOrigem().GetX(), item.MBR.GetDiagonal().GetOrigem().GetY());
        k[1] = make_pair(item.MBR.GetDiagonal().GetDestino().GetX(), item.MBR.GetDiagonal().GetDestino().GetY());
        if(k[0].first < a[0])
            a[0] = k[0].first;
        if(k[0].second < a[1])
            a[1] = k[0].second;
        if(k[1].first > b[0])
            b[0] = k[1].first;
        if(k[1].second > b[1])
            b[1] = k[1].second;
    }
    Ponto A(a[0], a[1]), B(b[0], b[1]);
    return Retangulo(A,B);
}

RTree::RTree(){
    RTreeFile.open(RTREE_FILENAME, fstream::binary|fstream::in|fstream::out);
    if(!ArquivoVazio()){
        if(RTreeFile.is_open()){
            streampos PosicaoDaRaiz;
            RTreeFile.read(reinterpret_cast<char*>(&PosicaoDaRaiz), sizeof(streampos));
            RTreeFile.read(reinterpret_cast<char*>(&(this->count)), sizeof(unsigned));
            RTreeFile.read(reinterpret_cast<char*>(&(this->altura)), sizeof(unsigned));
            RTreeFile.read(reinterpret_cast<char*>(&(this->registros)), sizeof(size_t));
            this->raiz = new Node(PosicaoDaRaiz);
        }
        else{
            cerr << "Exceção ao ler/abrir o arquivo: " << RTREE_FILENAME << endl;
            exit(-40);
        }
    }
    else
        this->raiz = nullptr;
}

bool RTree::ArquivoVazio(){
    if(RTreeFile.is_open()){
        streampos inicio, fim;
        inicio = RTreeFile.tellg();
        RTreeFile.seekg(0, fstream::end);
        fim = RTreeFile.tellg();
        RTreeFile.seekg(0, fstream::beg);
        return inicio == fim;
    }
    return true;
}

RTree::~RTree(){
    if(raiz != nullptr){
        RTreeFile.seekp(0, fstream::beg);
        RTreeFile.write(reinterpret_cast<char*>(&(this->raiz->DiskPos)), sizeof(streampos));
        RTreeFile.write(reinterpret_cast<char*>(&(this->count)), sizeof(unsigned));
        RTreeFile.write(reinterpret_cast<char*>(&(this->altura)), sizeof(unsigned));
        RTreeFile.write(reinterpret_cast<char*>(&(this->registros)), sizeof(size_t));
        delete this->raiz;
    }
    RTreeFile.close();
}

void RTree::CriaArvore(Retangulo& MbrForma, streampos& pos){
    Node* root = new Node(MbrForma, pos);
    streampos posicao = 1;
    this->count = 1u;
    this->altura = 0u;
    this->registros = 0ull;
    RTreeFile.write(reinterpret_cast<char*>(&posicao), sizeof(streampos));
    RTreeFile.write(reinterpret_cast<char*>(&(this->count)), sizeof(unsigned));
    RTreeFile.write(reinterpret_cast<char*>(&(this->altura)), sizeof (unsigned));
    RTreeFile.write(reinterpret_cast<char*>(&(this->registros)), sizeof(size_t));
    root->DiskPos = RTreeFile.tellp();
    RTreeFile.seekp(0, fstream::beg);
    RTreeFile.write(reinterpret_cast<char*>(&(root->DiskPos)), sizeof(streampos));
    this->raiz = root;
    root->SalvarNo();
}

size_t RTree::GetCount(){
    return this->count;
}
size_t RTree::GetRegistros(){
    return this->registros;
}

bool RTree::IsEmpty(){
    return !count;
}

Node* RTree::GetPtr(){
    return raiz;
}

bool RTree::Busca(Node* no, Retangulo& K, vector<NodeAux>& q){
    unsigned i = 0;
    NodeAux temp;
    if(!no->Folha()){
        Node* aux = nullptr;
        for(auto item: no->Chaves){
            if(item.MBR.Contem(K)){
                aux = new Node(item.ChildPtr);
                if(Busca(aux, K, q)){
                    temp.ptr = no;
                    temp.index = i;
                    q.push_back(temp);
                    return true;
                }
                delete aux;
            }
            i++;
        }
        return false;
    }
    for(auto item: no->Chaves){
        if(item.MBR == K){
            temp.ptr = no;
            temp.index = i;
            q.push_back(temp);
            return true;
        }
        i++;
    }
    return false;
}

list<Chave> RTree::Busca(Ponto& P){
    streampos RaizPos = root.GetPtr()->DiskPos;
    list<Chave> ListadeNo = Traversal(RaizPos, P), resultado;
    for(auto item: ListadeNo){
        if(item.MBR.Contem(P))
            resultado.push_back(item);
    }
    return resultado;
}

list<Chave> RTree::Traversal(streampos& no, Ponto& P){
    list<Chave> LC;
    Node* aux = new Node(no);
    if(!aux->Folha()){
        for(auto item: aux->Chaves)
            if(item.MBR.Contem(P))
                LC.splice(LC.end(), Traversal(item.ChildPtr, P));
    }
    else
        for(auto item: aux->Chaves)
            LC.push_back(item);
    delete aux;
    return LC;
}

void RTree::Inserir(Retangulo& MbrForma, streampos& pos){
    Node* no = root.GetPtr();
    registros++;
    if(no == nullptr)
        return CriaArvore(MbrForma, pos);
    stack<NodeAux> CaminhoNo;
    while(!no->Folha())
        no = EscolhaSubArvore(no, CaminhoNo, MbrForma);
//    if(!CaminhoNo.empty())
//        CaminhoNo.pop();
    Chave Key(MbrForma, pos, FOLHA);
    InserirNo(no, CaminhoNo, Key);
    Kai(CaminhoNo);
}

void RTree::Inserir(Chave& K){
    Chave A = K;
    Node* no = root.GetPtr();
    if(no == nullptr)
        return CriaArvore(A.MBR, A.Dado);
    stack<NodeAux> CaminhoNo;
    while(!no->Folha())
        no = EscolhaSubArvore(no, CaminhoNo, A.MBR);
//    if(!CaminhoNo.empty())
//        CaminhoNo.pop();
    InserirNo(no, CaminhoNo, A);
    Kai(CaminhoNo);
}

void RTree::InserirNo(Node* &No, stack<NodeAux>& caminho, Chave& inserir){
    No->Chaves.push_back(inserir);
    if(No->Overflow())
        return DividirEAjustar(No, caminho);
    No->SalvarNo();
    //NodeAux K(No);
    //caminho.push(K);
    AjustaCaminho(caminho, No->GetRetangulo());
}

Node* RTree::EscolhaSubArvore(Node* &no, stack<NodeAux>& caminho, Retangulo& MbrForma){
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
    // SE NENHUMA CHAVE CONTER A FORMA, ESCOLHA O QUE PRECISA CRESCER MENOS (menor crescimento da área)
    double aux = 0.0;
    pair<double, unsigned> escolha = make_pair(no->Chaves[0].MBR.CresceParaConter(MbrForma).GetArea(), 0);
    for(unsigned i=1; i < no->Chaves.size(); i++){
        aux = no->Chaves[i].MBR.CresceParaConter(MbrForma).GetArea();
        if(aux < escolha.first)
            escolha = make_pair(aux, i);
    }
    temp.index = escolha.second;
    caminho.push(temp);
    Node* ptrNo = new Node(no->Chaves[escolha.second].ChildPtr);
    return ptrNo;
}

/*
void RTree::AjustaCaminho(stack<NodeAux>& caminho){
    Node* no = caminho.top().ptr;
    caminho.pop();
    if(no == raiz) return;
    Retangulo R = no->GetRetangulo();
    delete no;
    NodeAux pai = caminho.top();
    if(pai.ptr->Ajusta(R, pai.index)){
        pai.ptr->SalvarNo();
        AjustaCaminho(caminho);
    }
}

*/

void RTree::AjustaCaminho(stack<NodeAux>& caminho, Retangulo R){
    if(!caminho.empty()){
        NodeAux pai = caminho.top();
        if(pai.ptr->Ajusta(R, pai.index)){
            pai.ptr->SalvarNo();
            Retangulo R = pai.ptr->GetRetangulo();
            if(pai.ptr != this->raiz)
                delete pai.ptr;
            caminho.pop();
            AjustaCaminho(caminho, R);
        }
    }
}


void RTree::DividirEAjustar(Node* &no, stack<NodeAux>& caminho){
    bool EhRaiz = (no == raiz);
    Node* novoNo = Divide(no);
    count++; // quantidade de nós na árvore cresce em 1
    if(!EhRaiz){
        Chave k(novoNo);
        NodeAux pai = caminho.top();
        caminho.pop();
        Retangulo R = no->GetRetangulo();
        delete novoNo;
        delete no;
        if(pai.ptr->Ajusta(R, pai.index))
            pai.ptr->SalvarNo();
        InserirNo(pai.ptr, caminho, k);
    }
    else
        CriaNovaRaiz(no, novoNo);
}

bool Node::Ajusta(Retangulo& MBR, unsigned index){
    bool modificado = false;
    Chaves[index].MBR.Ajusta(MBR, modificado);
    return modificado;
}

void RTree::CriaNovaRaiz(Node* &no, Node* &novoNo){
    vector<Node*> v(2);
    v[0] = no;
    v[1] = novoNo;
    Node* novaRaiz = new Node(v);
    raiz = novaRaiz;
    count++;
    altura++;
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
    unsigned BestKey = 0u;
    Retangulo Aux1, Aux2;
    for(auto &item: no->Chaves)
        if(item != *(G1.begin()) and item != *(G2.begin()))
            ChavesRestantes.push_back(item);
    while(!ChavesRestantes.empty()){
        unsigned i = 0;
        BestKey = i;
        expansion = -50.0; // mudar
        BestGroup = nullptr;
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
            else if(d1 - d2 > expansion){
                BestGroup = NoG2;
                BestKey = i;
                expansion = d1 - d2;
            }
            i++;
        }
        if(BestGroup != nullptr){
            BestGroup->Chaves.push_back(ChavesRestantes[BestKey]);
            ChavesRestantes.erase(ChavesRestantes.begin()+BestKey);
        }
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
    NoG1->DiskPos = no->DiskPos;
    delete no;
    no = NoG1;
    no->SalvarNo();
    NoG2->SalvarNo();
    return NoG2;
}

void RTree::Remove(vector<NodeAux>& toStack){
    stack<NodeAux> caminho;
    while(!toStack.empty()){
        caminho.push(toStack.back());
        toStack.pop_back();
    }
    Remove(caminho);
}

void RTree::Remove(stack<NodeAux>& Caminho){
    list<Chave> ChavesExcedentes = Reorganizar(Caminho);
    Reinserir(ChavesExcedentes);
    Kai(Caminho);
}

list<Chave> RTree::Reorganizar(stack<NodeAux>& Caminho){
    list<Chave> Q;
    if(Caminho.empty()) return Q;
    NodeAux No = Caminho.top();
    No.ptr->Chaves.erase(No.ptr->Chaves.begin()+No.index);
    if(No.ptr != root.GetPtr()){
        if(No.ptr->Chaves.size() < MINCHAVES){
            this->count--;
            Q.splice(Q.end(), EncontreAsFolhas(No.ptr));
            Caminho.pop();
            Q.splice(Q.end(), Reorganizar(Caminho));
        }
        else{
            No.ptr->SalvarNo();
            AjustaCaminho(Caminho, No.ptr->GetRetangulo());
        }
    }
    return Q;
}

void RTree::Reinserir(list<Chave>& ChavesExcedentes){
    for(auto &item: ChavesExcedentes)
        Inserir(item.MBR, item.Dado);
    ChavesExcedentes.clear();
}

list<Chave> RTree::EncontreAsFolhas(Node*& no){ // CUIDADO COM ESSE METODO
    list<Chave> LC;
    if(no->Folha()){
        for(auto item: no->Chaves)
            LC.push_back(item);
    }
    else{
        Node* aux = nullptr;
        for(auto item: no->Chaves){
            aux = new Node(item.ChildPtr);
            LC.splice(LC.end(), EncontreAsFolhas(aux));
        }

    }
    if(no != raiz){
        no->Delete();
        delete no;
    }
    return LC;
}

string RTree::GetNivel(){
    string saida;
    size_t N = GetRegistros();
    double m = MINCHAVES, h = ceil(log(N)/log(m));
    unsigned alturaMaxima = static_cast<unsigned>(h-1);
    m = MAXCHAVES;
    h = ceil(log(N)/log(m));
    unsigned alturaMinima = static_cast<unsigned>(h-1);
    saida = "[" + to_string(alturaMinima) +", "+ to_string(alturaMaxima) + "]";
    return saida;
}

bool Node::Folha(){
    return (Nivel == FOLHA);
}

bool Node::Overflow(){
    return (Chaves.size() > MAXCHAVES);
}

void Kai(stack<NodeAux>& pilha){
    Node* raiz = root.GetPtr();
    Node* aux = nullptr;
    while(!pilha.empty()){
        aux = pilha.top().ptr;
        if(aux != nullptr and aux != raiz)
            delete aux;
        pilha.pop();
    }
}

void Kai(vector<NodeAux>& DS){
    Node* raiz = root.GetPtr(), *aux = nullptr;
    while(!DS.empty()){
        aux = DS.back().ptr;
        if(aux != nullptr and aux != raiz)
            delete aux;
        DS.pop_back();
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
