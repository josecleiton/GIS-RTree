#include "findwindow.hpp"
#include "ui_findwindow.h"

FindWindow::FindWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Forma encontrada!");
    this->remove = false;
}

FindWindow::~FindWindow()
{
    delete ui;
}

void FindWindow::on_remove_clicked()
{
    QMessageBox ask;
    int escolha = ask.warning(nullptr, "Tem certeza?", "Sua decisão afetará significativamente o banco de dados.", QMessageBox::Ok|QMessageBox::Cancel, QMessageBox::Cancel);
    if(escolha == QMessageBox::Ok){
        this->remove = true;
        QMessageBox QMB;
        QMB.information(nullptr, "Sucesso", "Forma(s) removida(s) do banco de dados.");
        this->close();
    }
}

bool FindWindow::setRegistros(DiskAPI::Registro** &R, size_t tam){
    if(R != nullptr and tam){
        vector<Registro*> v(R, R+tam);
        this->regs = v;
        this->ui->ra->setRegistros(this->regs);
        if(!listaValida()){
            this->ui->remove->setDisabled(true);
            this->ui->info->setDisabled(true);
        }
        return true;
    }
    return false;
}

bool FindWindow::GetRemove(){
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
        aux.str("");
        aux.clear();
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
            conteudo += "Origem: ";
            aux << temp->GetOrigem();
            aux.clear();
            conteudo += aux.str()+"\n Destino: ";
            aux << temp->GetDestino();
            conteudo += aux.str() + "\n";
            delete temp;
        }
        else if(tipo == POLIGONO){
            Poligono* temp = reinterpret_cast<Poligono*>(r->Conversao());
            conteudo += "Quantidade de Vértices: "+to_string(temp->GetSize())+"\n";
            conteudo += "Coordenadas dos Vértices: ";
            aux << *temp;
            conteudo += aux.str()+"\n Area: "+to_string(temp->Area())+"\n";
            temp->setFakeKai(true);
            delete temp;
        }
        else if(tipo == CIRCULO){
            Circulo* temp = reinterpret_cast<Circulo*>(r->Conversao());
            aux << temp->centro;
            conteudo += "Centro: "+aux.str()+"\n";
            conteudo += "Raio: "+to_string(temp->raio)+"\n";
            conteudo += "Area: "+to_string(temp->Area())+"\n";
            delete temp;
        }
        else{
            conteudo += "Quantidade de Vértices: "+to_string(r->tam)+"\n";
        }
        info.information(nullptr, "Forma n. "+QString::fromStdString(to_string(count++)), QString::fromStdString(conteudo));
    }
}
