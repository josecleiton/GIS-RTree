#include "findwindow.hpp"
#include "ui_findwindow.h"

FindWindow::FindWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Forma encontrada!");
    this->remove = false;
    this->setWindowIcon(QIcon(ICON));
    this->libera = true;
}

FindWindow::~FindWindow()
{
    if(libera){
        for(auto r: this->regs) // LIBERA OS REGISTROS ALOCADOS PREVIAMENTE
            delete r;
    }
    delete ui;
}

void FindWindow::on_remove_clicked()
{
    QMessageBox ask;
    int escolha = ask.warning(nullptr, "Tem certeza?", "Sua decisão afetará significativamente a organização dos dados.", QMessageBox::Ok|QMessageBox::Cancel, QMessageBox::Cancel);
    if(escolha == QMessageBox::Ok){
        this->remove = true;
        QMessageBox QMB;
        QMB.information(nullptr, "Sucesso", "Forma(s) removida(s) da R-Tree.");
        this->close();
    }
}

bool FindWindow::setRegistros(DiskAPI::Registro** &R, size_t tam, bool inter){
    if(R != nullptr and tam){
        vector<Registro*> v(R, R+tam);
        this->regs = v;
        this->ui->ra->setRegistros(this->regs);
        if(!listaValida()){
            this->ui->remove->setDisabled(true);
            this->ui->info->setDisabled(true);
        }
        else if(inter){
            this->ui->remove->setDisabled(true);
            this->libera = false;
        }
        return true;
    }
    return false;
}

bool FindWindow::getRemove(){
    return this->remove;
}

bool FindWindow::listaValida(){
    return this->ui->ra->listaValida();
}

void FindWindow::on_info_clicked() // MOSTRA AS INFORMAÇÕES DOS REGISTROS ENCONTRADOS
{
    QMessageBox info;
    int tipo;
    std::stringstream aux;
    std::string conteudo;
    int count = 1;
    for(auto r: regs){
        aux.str(""); aux.clear();
        conteudo = "Tipo: ";
        tipo = r->tipo;
        conteudo += r->StrTipo()+"\n";
        if(tipo == PONTO){
            conteudo += "Coordenada: ";
            Ponto* temp = reinterpret_cast<Ponto*>(r->Conversao());
            aux << *temp;
            conteudo += aux.str() + "\n";
            delete temp;
        }
        else if(tipo == LINHA){
            Aresta* temp = reinterpret_cast<Aresta*>(r->Conversao());
            string origem;
            conteudo += "Origem: ";
            aux << temp->GetOrigem();
            origem = aux.str();
            conteudo += origem+"\nDestino: ";
            aux.str(""); aux.clear();
            aux << temp->GetDestino();
            conteudo += aux.str() + "\nInclinação: ";
            aux.str(""); aux.clear();
            aux << temp->Inclinacao();
            conteudo += aux.str() + "\n";
            delete temp;
        }
        else if(tipo == POLIGONO){
            Poligono* temp = reinterpret_cast<Poligono*>(r->Conversao());
            conteudo += "Quantidade de Vértices: "+to_string(temp->GetSize())+"\n";
            conteudo += "Coordenadas dos Vértices: ";
            aux << *temp;
            double area = temp->Area();
            conteudo += aux.str()+"\n Area: "+to_string(area)+"\n Centroide: ";
            aux.str(""); aux.clear();
            aux << temp->Centroide(area);
            conteudo += aux.str()+"\n";
            temp->setFakeKai(true);
            delete temp;
        }
        else if(tipo == CIRCULO){
            Circulo* temp = reinterpret_cast<Circulo*>(r->Conversao());
            aux << temp->GetCentro();
            conteudo += "Centro: "+aux.str()+"\n";
            conteudo += "Raio: "+to_string(temp->GetRaio())+"\n";
            conteudo += "Area: "+to_string(temp->Area())+"\n";
            delete temp;
        }
        else{
            Vertice* temp = r->lista, *fim = temp->Antihorario();
            conteudo += "Quantidade de Vértices: "+to_string(r->tam)+"\n";
            conteudo += "Coordenadas dos Vértices: ";
            while(true){
                aux << temp->GetPonto();
                aux << " ";
                if(temp == fim) break;
                temp = temp->Horario();
            }
            conteudo += aux.str() + "\n";
        }
        info.information(nullptr, "Forma n. "+QString::fromStdString(to_string(count++)), QString::fromStdString(conteudo));
    }
}

void FindWindow::setInterCircle(bool op){
    this->ui->ra->setInterCircle(op);
}
