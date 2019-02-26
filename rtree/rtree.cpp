#include "rtree.hpp"
#include "variaveis.hpp"

namespace SpatialIndex{

Chave::Chave(Retangulo _mbr, streampos _dado): MBR(_mbr), ChildPtr(_dado){
}

Chave::Chave(){
    Ponto A, B;
    Retangulo vazio(A, B);
    this->MBR = vazio;
}

Chave::Chave(Node*& No){
    Retangulo R1 = No->GetRetangulo();
    Chave k(R1, No->DiskPos);
    *this = k;
}

bool Chave::Ajusta(Retangulo& alvo){
    bool modificado = false;
    this->MBR.Ajusta(alvo, modificado);
    return modificado;
}

Node::Node(Retangulo& R, streampos& PosR){
    Chave k(R, PosR);
    Chaves.push_back(k);
    Nivel = FOLHA;
}

Node::Node(unsigned nivel, vector<Chave>& itens):
    Nivel(nivel), Chaves(itens){
}

Node::Node(Node*& no1, Node*& no2){
    Nivel = INTERNO;
    list<Node*> chavesNovaRaiz;
    chavesNovaRaiz.push_back(no1);
    chavesNovaRaiz.push_back(no2);
    for(auto &item: chavesNovaRaiz){
        Retangulo MBR_NovaChave(item->GetRetangulo());
        Chave novaChave(MBR_NovaChave, item->DiskPos);
        Chaves.push_back(novaChave);
        delete item;
    }
    this->SalvarNo();
}

Node::Node(streampos& no){
    bool active = false;
    root.File().seekg(no, fstream::beg);
    root.File().read(reinterpret_cast<char*>(&active), sizeof(bool));
    if(active){
        unsigned numChaves;
        root.File().read(reinterpret_cast<char*>(&Nivel), sizeof(unsigned));
        root.File().read(reinterpret_cast<char*>(&numChaves), sizeof(unsigned));
        this->Chaves.resize(numChaves);
        for(unsigned i=0; i<numChaves; i++)
            root.File().read(reinterpret_cast<char*>(&(Chaves[i])), sizeof(Chave));
        this->DiskPos = no;
    }
    else
        cerr << "Página inválida! Reorganize antes de fazer outra requisição." << endl;
}

streampos Node::SalvarNo(){
    unsigned numChaves = static_cast<unsigned>(this->Chaves.size());
    bool active = true;
    Chave key(Retangulo(), 0);
    if(DiskPos)
        root.File().seekp(this->DiskPos, fstream::beg);
    else{
        root.File().seekp(0, fstream::end);
        this->DiskPos = root.File().tellp();
    }
    root.File().write(reinterpret_cast<char*>(&active), sizeof(bool));
    root.File().write(reinterpret_cast<char*>(&(this->Nivel)), sizeof(unsigned));
    root.File().write(reinterpret_cast<char*>(&numChaves), sizeof(unsigned));
    for(unsigned i=0; i<MAXCHAVES; i++){
        if(i<numChaves)
            root.File().write(reinterpret_cast<char*>(&(this->Chaves[i])), sizeof(Chave));
        else
            root.File().write(reinterpret_cast<char*>(&key), sizeof(Chave));
    }
    return this->DiskPos;
}

streampos Node::Delete(){
    bool active = false;
    root.File().seekp(this->DiskPos, fstream::beg);
    root.File().write(reinterpret_cast<char*>(&active), sizeof(bool));
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
    vector<double> CoordOrigem(2, DBL_MAX), CoordDestino(2, DBL_MIN);
    vector<pair<double, double>> CoordTemporarias(2); // Possíveis coordenadas min/max a serem avaliadas pela rotina
    for(auto item: Chaves){
        CoordTemporarias[0] = make_pair(item.MBR.GetDiagonal().GetOrigem().GetX(), item.MBR.GetDiagonal().GetOrigem().GetY());
        CoordTemporarias[1] = make_pair(item.MBR.GetDiagonal().GetDestino().GetX(), item.MBR.GetDiagonal().GetDestino().GetY());

        if(CoordTemporarias[0].first < CoordOrigem[0])
            CoordOrigem[0] = CoordTemporarias[0].first;
        if(CoordTemporarias[0].second < CoordOrigem[1])
            CoordOrigem[1] = CoordTemporarias[0].second;
        if(CoordTemporarias[1].first > CoordDestino[0])
            CoordDestino[0] = CoordTemporarias[1].first;
        if(CoordTemporarias[1].second > CoordDestino[1])
            CoordDestino[1] = CoordTemporarias[1].second;
    }
    Ponto Origem(CoordOrigem[0], CoordOrigem[1]), Destino(CoordDestino[0], CoordDestino[1]);
    return Retangulo(Origem, Destino);
}

RTree::RTree(){
    this->treeFile.open(RTREE_FILENAME, fstream::binary|fstream::in|fstream::out);
    if(!ArquivoVazio()){
        if(this->treeFile.is_open()){
            streampos PosicaoDaRaiz;
            this->treeFile.read(reinterpret_cast<char*>(&PosicaoDaRaiz), sizeof(streampos));
            this->treeFile.read(reinterpret_cast<char*>(&(this->count)), sizeof(unsigned));
            this->treeFile.read(reinterpret_cast<char*>(&(this->altura)), sizeof(unsigned));
            this->treeFile.read(reinterpret_cast<char*>(&(this->registros)), sizeof(size_t));
            this->raiz = new Node(PosicaoDaRaiz);
        }
        else{
            cerr << "Exceção ao ler/abrir o arquivo: " << RTREE_FILENAME << endl;
            cerr << strerror(errno) << endl;
            abort();
        }
    }
    else
        this->raiz = nullptr;
}

bool RTree::ArquivoVazio(){
    if(this->treeFile.is_open()){
        streampos inicio, fim;
        inicio = this->treeFile.tellg();
        this->treeFile.seekg(0, fstream::end);
        fim = this->treeFile.tellg();
        this->treeFile.seekg(0, fstream::beg);
        return inicio == fim;
    }
    else{
        cerr << RTREE_FILENAME << " " << strerror(errno) << endl;
        abort();
    }
}

RTree::~RTree(){
    if(raiz != nullptr){
        this->treeFile.seekp(0, fstream::beg);
        this->treeFile.write(reinterpret_cast<char*>(&(this->raiz->DiskPos)), sizeof(streampos));
        this->treeFile.write(reinterpret_cast<char*>(&(this->count)), sizeof(unsigned));
        this->treeFile.write(reinterpret_cast<char*>(&(this->altura)), sizeof(unsigned));
        this->treeFile.write(reinterpret_cast<char*>(&(this->registros)), sizeof(size_t));
        delete this->raiz;
    }
    this->treeFile.close();
}

void RTree::CriaArvore(Retangulo& MbrForma, streampos& pos){
    Node* root = new Node(MbrForma, pos);
    streampos posicao = 1;
    this->count = 1u;
    this->altura = 0u;
    this->registros = 1ull;
    this->treeFile.write(reinterpret_cast<char*>(&posicao), sizeof(streampos));
    this->treeFile.write(reinterpret_cast<char*>(&(this->count)), sizeof(unsigned));
    this->treeFile.write(reinterpret_cast<char*>(&(this->altura)), sizeof (unsigned));
    this->treeFile.write(reinterpret_cast<char*>(&(this->registros)), sizeof(size_t));
    root->DiskPos = this->treeFile.tellp();
    this->treeFile.seekp(0, fstream::beg);
    this->treeFile.write(reinterpret_cast<char*>(&(root->DiskPos)), sizeof(streampos));
    this->raiz = root;
    root->SalvarNo();
}

fstream& RTree::File(){
    return this->treeFile;
}

bool RTree::ApagarArvore(){
    if(raiz != nullptr){
        delete raiz;
        raiz = nullptr;
    }
    this->treeFile.close();
    fstream temp(RTREE_FILENAME, fstream::binary|fstream::out);
    if(temp.is_open()){
        temp.close();
        this->treeFile.open(RTREE_FILENAME, fstream::binary|fstream::out|fstream::in);
        if(this->treeFile.is_open()) return true;
    }
    cerr << "Arquivo: " << RTREE_FILENAME << " não foi reaberto." << endl;
    cerr << strerror(errno) << endl;
    abort();
}

unsigned RTree::GetCount(){
    return this->count;
}

unsigned RTree::GetAltura(){
    return this->altura;
}

size_t RTree::GetRegistros(){
    return this->registros;
}

bool RTree::IsEmpty(){
    return !count;
}

Node* RTree::NodePtr(){
    return this->raiz;
}

bool RTree::Busca(Node* no, Retangulo& K, list<NodeAux>& q){
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

bool RTree::Busca(Node* no, Chave& K, list<NodeAux>& q){
    unsigned i = 0;
    NodeAux temp;
    if(!no->Folha()){
        Node* aux = nullptr;
        for(auto item: no->Chaves){
            if(item.MBR.Contem(K.MBR)){
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
        if(item == K){
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
    streampos RaizPos = root.NodePtr()->DiskPos;
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
    Node* no = root.NodePtr();
    registros++;
    if(no == nullptr)
        return CriaArvore(MbrForma, pos);
    stack<NodeAux> CaminhoNo;
    while(!no->Folha())
        no = EscolhaSubArvore(no, CaminhoNo, MbrForma);
//    if(!CaminhoNo.empty())
//        CaminhoNo.pop();
    Chave Key(MbrForma, pos);
    InserirNo(no, CaminhoNo, Key);
    Kai(CaminhoNo);
}

void RTree::Inserir(Chave& K){
    Chave A = K;
    Node* no = root.NodePtr();
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
    AjustaCaminho(caminho, No->GetRetangulo());
}

Node* RTree::EscolhaSubArvore(Node* &no, stack<NodeAux>& caminho, Retangulo& MbrForma){
    vector<pair<NodeAux, double>> contem;
    NodeAux temp;
    temp.ptr = no;
    unsigned index = 0;
    for(auto chave: no->Chaves){
        if(chave.MBR.Contem(MbrForma)){
            NodeAux aux(new Node(chave.ChildPtr), index);
            contem.push_back(make_pair(aux, chave.MBR.GetArea()));
        }
        index++;
    }
    Node* resultado = nullptr;
    if(!contem.empty()){
        size_t max = 0;
        for(auto it = contem.begin()+1; it != contem.end(); it++)
            if(it->second > contem[max].second)
               max = it-contem.begin();
        swap(contem[max].first.ptr, resultado);
        for(auto &candidato: contem) // LIBERA O RESTO
          if(candidato.first.ptr != nullptr)
            delete candidato.first.ptr;
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
    resultado = new Node(no->Chaves[escolha.second].ChildPtr);
    return resultado;
}

void RTree::AjustaCaminho(stack<NodeAux>& caminho, Retangulo R){
    if(!caminho.empty()){
        // .INDEX = POSIÇÃO DO NÓ ATUAL NO PAI
        // .PTR = PONTEIRO PARA O PAI
        NodeAux pai = caminho.top();
        if(pai.ptr->Chaves[pai.index].Ajusta(R)){
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
    bool isRoot = (no == raiz);
    Node* novoNo = Divide(no);
    count++; // quantidade de nós na árvore cresce em 1
    if(!isRoot){
        Chave chaveNovoNo(novoNo);
        NodeAux pai = caminho.top();
        // .INDEX = POSIÇÃO DO NÓ ATUAL NO PAI
        // .PTR = PONTEIRO PARA O PAI
        caminho.pop();
        Retangulo R = no->GetRetangulo();
        delete novoNo;
        delete no;
        if(pai.ptr->Chaves[pai.index].Ajusta(R))
            pai.ptr->SalvarNo();
        InserirNo(pai.ptr, caminho, chaveNovoNo);
    }
    else
        CriaNovaRaiz(no, novoNo);
}

void RTree::CriaNovaRaiz(Node* &no, Node* &novoNo){
    Node* novaRaiz = new Node(no, novoNo);
    raiz = novaRaiz;
    count++;
    altura++;
}


/*
 * CÓDIGO ABAIXO RETIRADO DO LIVRO:
 *  SPATIAL DATABASES WITH APPLICATION TO GIS - RIGGAUX
*/
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

bool RTree::Remove(Chave& K){
    list<NodeAux> toStack;
    stack<NodeAux> caminho;
    Busca(root.NodePtr(), K, toStack);
    while(!toStack.empty()){
        caminho.push(toStack.back());
        toStack.pop_back();
    }
    Remove(caminho);
    return root.NodePtr()->Chaves.empty(); //SE A RAIZ ESTIVER VAZIA
}

bool RTree::Remove(list<NodeAux>& toStack){
    stack<NodeAux> caminho;
    while(!toStack.empty()){
        caminho.push(toStack.back());
        toStack.pop_back();
    }
    Remove(caminho);
    return root.NodePtr()->Chaves.empty(); //SE A RAIZ ESTIVER VAZIA
}

void RTree::Remove(stack<NodeAux>& Caminho){
    list<Chave> ChavesExcedentes = Reorganizar(Caminho);
    this->registros -= ChavesExcedentes.size();
    Reinserir(ChavesExcedentes);
    Kai(Caminho);
}

list<Chave> RTree::Reorganizar(stack<NodeAux>& Caminho){
    list<Chave> Q;
    if(Caminho.empty()) return Q;
    NodeAux No = Caminho.top();
    No.ptr->Chaves.erase(No.ptr->Chaves.begin()+No.index);
    if(No.ptr->Folha()) registros--;
    if(No.ptr != root.NodePtr()){
        if(No.ptr->Chaves.size() < MINCHAVES){
            this->count--;
            Q.splice(Q.end(), EncontreAsFolhas(No.ptr, true));
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

list<Chave> RTree::EncontreAsFolhas(Node* no, bool remove){ // CUIDADO COM ESSE METODO
    list<Chave> LC;
    if(no->Folha()){
        for(auto item: no->Chaves)
            LC.push_back(item);
    }
    else{
        Node* aux = nullptr;
        for(auto item: no->Chaves){
            aux = new Node(item.ChildPtr);
            LC.splice(LC.end(), EncontreAsFolhas(aux, remove));
        }

    }
    if(no != raiz){
        if(remove) no->Delete();
        delete no;
    }
    return LC;
}

bool Node::Folha(){
    return (Nivel == FOLHA);
}

bool Node::Overflow(){
    return (Chaves.size() > MAXCHAVES);
}

void Kai(stack<NodeAux>& pilha){
    Node* raiz = root.NodePtr();
    Node* aux = nullptr;
    while(!pilha.empty()){
        aux = pilha.top().ptr;
        if(aux != nullptr and aux != raiz)
            delete aux;
        pilha.pop();
    }
}

void Kai(list<NodeAux>& DS){
    Node* raiz = root.NodePtr(), *aux = nullptr;
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

Hash::Hash(){
    this->path = H_FILENAME;
}

Hash::~Hash(){
    if(this->handle != nullptr) delete this->handle;
}

void Hash::Insere(string id, streampos& forma){
    fstream forma_file(path+"id_"+id+".bin", fstream::app|fstream::binary);
    if(forma_file.is_open()){
        forma_file.write(reinterpret_cast<char*>(&forma), sizeof(streampos));
        forma_file.close();
    }
    else{
        cerr << "ERRO NA ABERTURA DO ARQUIVO: " << H_FILENAME << id+".bin" << endl;
        cerr << strerror(errno) << endl;
        abort();
    }
}


vector<streampos>* Hash::SelectAll(string& query){
    if(this->handle != nullptr){
        delete this->handle;
        this->handle = nullptr;
    }
    vector<streampos>* result = new vector<streampos>;
    fstream file(path+"id_"+query+".bin", fstream::in|fstream::binary);
    streampos aux;
    if(file.is_open()){
        while(file.read(reinterpret_cast<char*>(&aux), sizeof(streampos)))
            result->push_back(aux);
        file.close();
    }
    this->handle = result;
    return result;
}

} // NAMESPACE SPATIALINDEX
