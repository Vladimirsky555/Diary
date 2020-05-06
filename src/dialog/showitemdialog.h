#ifndef SHOWITEMDIALOG_H
#define SHOWITEMDIALOG_H

#include <QDialog>
#include <QRegExp>

#include "model/model.h"
#include "highlighter.h"

namespace Ui {
class ShowItemDialog;
}

class ShowItemDialog : public QDialog
{
    Q_OBJECT

    Highlighter *highlighter;
    QString pattern;

public:
    ShowItemDialog(Data *item,QWidget *parent = 0);
    virtual ~ShowItemDialog();

    bool checkRegExp(QRegExp rx);
    void setPattern(QString pattern);

private slots:
    void on_edtPattern_textChanged(const QString &str);

private:
    Ui::ShowItemDialog *ui;
};

#endif // SHOWITEMDIALOG_H
