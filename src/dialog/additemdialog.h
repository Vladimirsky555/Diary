#ifndef ADDITEMDIALOG_H
#define ADDITEMDIALOG_H

#include <QDialog>
#include <QDateTime>
#include <QRegExp>

#include "model/model.h"
#include "highlighter.h"

namespace Ui {
class AddItemDialog;
}

class AddItemDialog : public QDialog
{
    Q_OBJECT

    Data *item;
    Highlighter *highlighter;

public:
    bool emptyFields;

public:
    AddItemDialog(Data *item, QWidget *parent = 0);
    virtual ~AddItemDialog();

    void load();
    bool save();
    void defineBox();
    bool checkRegExp(QRegExp rx);
    void setPattern(QString pattern);
;
private slots:
    void on_btnAdd_clicked();
    void on_boxCategory_currentIndexChanged(int index);
    void on_btnClose_clicked();
    void on_edtPattern_textChanged(const QString &str);

    void chooseFontButton();
    void copy();
    void cut();
    void paste();
    void undo();
    void redo();

private:
    Ui::AddItemDialog *ui;
};

#endif // ADDITEMDIALOG_H
