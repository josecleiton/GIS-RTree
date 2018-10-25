#ifndef INSERTWINDOW_HPP
#define INSERTWINDOW_HPP

#include <QDialog>
#include <QRegExpValidator>
#include <iostream>
#include <fstream>
#include <vector>
#include "regex.hpp"
using namespace std;

#define INSERT_FORM "../GIS/form" //ARQUIVO TEMPORÁRIO QUE 'SEGURA' A INFORMAÇÃO DA JANELA INSERT


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
};


#endif // INSERTWINDOW_HPP
