#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

#include "data.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

    Data *item;
    bool edit; //true - добавление, false - редактирование

public:
    explicit Dialog(Data* item, bool edit, QWidget *parent = 0);
    ~Dialog();

    void load();
    void save();

private slots:
    void on_btnClose_clicked();
    void on_btnAdd_clicked();

    void chooseFontButton();
    void copy();
    void cut();
    void paste();
    void undo();
    void redo();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
