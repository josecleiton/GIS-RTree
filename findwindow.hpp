#ifndef FINDWINDOW_HPP
#define FINDWINDOW_HPP

#include <QDialog>
#include <QMessageBox>
#include <iostream>
#include "disk.hpp"
#include "renderarea.hpp"

namespace Ui {
class FindWindow;
}

class FindWindow : public QDialog
{
    Q_OBJECT

public:
    explicit FindWindow(QWidget *parent = nullptr);
    ~FindWindow();
    bool setReg(DiskAPI::Registro* R);
    bool GetRemove();

private slots:
    void on_remove_clicked();

private:
    Ui::FindWindow *ui;
    bool remove;
    unsigned count{};
};

#endif // FINDWINDOW_HPP
