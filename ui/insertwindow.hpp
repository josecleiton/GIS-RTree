#ifndef INSERTWINDOW_HPP
#define INSERTWINDOW_HPP

#include <QDialog>
#include "icon.hpp"
#include <QMessageBox>
#include <iostream>
#include <fstream>
#include <vector>
#include "regex.hpp"
#include "rtree.hpp"
#include "spatialdata.hpp"
#include "disk.hpp"
#include "variaveis.hpp"

using namespace std;

namespace Ui {
class InsertWindow;
}

class InsertWindow : public QDialog
{
    Q_OBJECT

public:
    explicit InsertWindow(QWidget *parent = nullptr);
    ~InsertWindow();

private slots:
    void on_submit_clicked();

    void on_cancel_clicked();

private:
    Ui::InsertWindow *ui;
    void Inserir();
    void InserirConjunto();
};


#endif // INSERTWINDOW_HPP
