#ifndef RECTANGLESEARCHWINDOW_HPP
#define RECTANGLESEARCHWINDOW_HPP

#include <QDialog>
#include <QRegExp>
#include <QRegExpValidator>
#include <QMessageBox>
#include "variaveis.hpp"
#include "rtree.hpp"
#include "spatialdata.hpp"
#include "regex.hpp"
#include "disk.hpp"
#include "findwindow.hpp"

namespace Ui {
class RectangleSearchWindow;
}

class RectangleSearchWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RectangleSearchWindow(QWidget *parent = nullptr);
    ~RectangleSearchWindow();
    DiskAPI::Registro* GetRegistro();

private slots:
    void on_button_clicked();

private:
    Ui::RectangleSearchWindow *ui;
    DiskAPI::Registro* reg;
};

#endif // RECTANGLESEARCHWINDOW_HPP
