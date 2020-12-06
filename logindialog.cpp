#include "logindialog.h"
#include "ui_logindialog.h"

#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    setWindowTitle("Авторизация");
    ui->edtPassword->setEchoMode(QLineEdit::EchoMode::Password);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::autification()
{
    if(this->login == "" || this->password == ""){
        QMessageBox::information(this, "Информация!", "Введите логин и пароль!");
        return;
    }

    if(this->login == "user" && this->password == "1234")
    {
        this->isLogined = true;
    }
}

bool LoginDialog::getIsLogined()
{
  return this->isLogined;
}

void LoginDialog::on_btnEnter_clicked()
{
    this->login = ui->edtLogin->text();
    this->password = ui->edtPassword->text();

    autification();

    close();
}

void LoginDialog::on_btnCancel_clicked()
{

}
