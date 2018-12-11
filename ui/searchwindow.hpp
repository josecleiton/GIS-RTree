#ifndef SEARCHWINDOW_HPP
#define SEARCHWINDOW_HPP

#include <sstream>
#include <QDialog>
#include "icon.hpp"
#include "rectanglesearchwindow.hpp"
#include "findwindow.hpp"
#include "insertpointwindow.hpp"
#include "variaveis.hpp"
#include "stringwindow.hpp"
#include "rtree.hpp"

namespace Ui {
class SearchWindow;
}

class SearchWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SearchWindow(QWidget *parent = nullptr);
    ~SearchWindow();
    bool getNuke();

private slots:
    //BUSCA SIMPLES NA R-TREE PELA DIAGONAL
    void on_retangulo_clicked();

    //BUSCA POR RELAÇÃO ENTRE DUAS FORMAS BUSCADAS NA R-TREE
    void on_interseccao_clicked();

    //BUSCA TODAS AS FORMAS QUE CONTÉM UM PONTO P
    void on_ponto_clicked();

    //UTILIZA O ID USADO NA INSERÇÃO DA FORMA PARA RETORNÁ-LA (HASH)
    void on_id_clicked();

    //RETORNA TODAS FORMAS INDEXADAS NA R-TREE
    void on_all_clicked();

private:
    Ui::SearchWindow *ui;
    bool nuke{};
};

#endif // SEARCHWINDOW_HPP
