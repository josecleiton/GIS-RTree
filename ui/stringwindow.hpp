#ifndef STRINGWINDOW_HPP
#define STRINGWINDOW_HPP

#include <QDialog>
#include <string>
#include "icon.hpp"
#include "regex.hpp"

namespace Ui {
class StringWindow;
}

class StringWindow : public QDialog
{
    Q_OBJECT

public:
    explicit StringWindow(QWidget *parent = nullptr);
    ~StringWindow();
    std::string getStr();

private slots:
    void on_ok_clicked();

private:
    Ui::StringWindow *ui;
    std::string str;
};

#endif // STRINGWINDOW_HPP
