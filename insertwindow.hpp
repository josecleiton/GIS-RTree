#ifndef INSERTWINDOW_HPP
#define INSERTWINDOW_HPP

#include <QDialog>

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
    void on_pushButton_clicked();

private:
    Ui::InsertWindow *ui;
};

#endif // INSERTWINDOW_HPP
