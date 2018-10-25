#ifndef RECTANGLESEARCHWINDOW_HPP
#define RECTANGLESEARCHWINDOW_HPP

#include <QDialog>
#include <QRegExp>
#include <QRegExpValidator>
#include "variaveis.hpp"
#include "rtree.hpp"
#include "spatialdata.hpp"
#include "regex.hpp"

namespace Ui {
class RectangleSearchWindow;
}

class RectangleSearchWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RectangleSearchWindow(QWidget *parent = nullptr);
    ~RectangleSearchWindow();

private slots:
    void on_button_clicked();

private:
    Ui::RectangleSearchWindow *ui;
};

#endif // RECTANGLESEARCHWINDOW_HPP
