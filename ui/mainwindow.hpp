#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "insertwindow.hpp"
#include "searchwindow.hpp"
#include "variaveis.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_inserir_clicked();
    void on_buscar_clicked();
    void on_cancel_clicked();

    void on_info_clicked();

    void on_creditos_clicked();

private:
    Ui::MainWindow *ui;
    void EmptyDS();
};

#endif // MAINWINDOW_H
