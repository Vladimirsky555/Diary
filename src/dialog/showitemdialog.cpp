#include "showitemdialog.h"
#include "ui_showitemdialog.h"

#include <QMessageBox>

ShowItemDialog::ShowItemDialog(Data *item, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowItemDialog)
{
    ui->setupUi(this);

    setWindowTitle("Просмотр записи");

    highlighter = new Highlighter(this);
    highlighter->setDocument(ui->edtText->document());
    highlighter->setPattern(ui->edtPattern->text());

    if(item){
        ui->edtText->append("Дата: " + item->Date().toString(Qt::ISODate));
        ui->edtText->append("Категория: " + item->Category());
        ui->edtText->append("Заголовок: " + item->Title());
        ui->edtText->append(" ");
        ui->edtText->append(item->Text());
    }
}

ShowItemDialog::~ShowItemDialog()
{
    delete ui;
}

bool ShowItemDialog::checkRegExp(QRegExp rx)
{
    if(rx.isValid() && !rx.isEmpty() && !rx.exactMatch("")){
          return true;
      }else{
          QMessageBox::information(this,"Информсообщение",
                                   trUtf8("Некорректный шаблон регулярного выражения!"));
          return false;
    }
}

void ShowItemDialog::setPattern(QString pattern)
{
    ui->edtPattern->setText(pattern);
}



void ShowItemDialog::on_edtPattern_textChanged(const QString &str)
{
    highlighter->setPattern(str);
    highlighter->rehighlight();
}
