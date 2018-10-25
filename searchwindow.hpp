#ifndef SEARCHWINDOW_HPP
#define SEARCHWINDOW_HPP

#include <QDialog>
#include "rectanglesearchwindow.hpp"

namespace Ui {
class SearchWindow;
}

class SearchWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SearchWindow(QWidget *parent = nullptr);
    ~SearchWindow();

private slots:
    void on_retangulo_clicked();

private:
    Ui::SearchWindow *ui;
};

#endif // SEARCHWINDOW_HPP
