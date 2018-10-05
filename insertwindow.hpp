#ifndef INSERTWINDOW_HPP
#define INSERTWINDOW_HPP

#include <QDialog>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

#define INSERT_FORM "../GIS/form"

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

private:
    Ui::InsertWindow *ui;
};


#endif // INSERTWINDOW_HPP