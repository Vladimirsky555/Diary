#include "dialog.h"
#include "ui_dialog.h"

#include <QMessageBox>
#include <QFontDialog>
#include <QDebug>

Dialog::Dialog(Data *item, bool edit, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    this->item = item;
    this->edit = edit;

    ui->boxCategory->addItems(QStringList() <<
                              "---" <<
                              "Запись" <<
                              "План" <<
                              "Идея" <<
                              "Отчёт" <<
                              "Спорт" <<
                              "Учёба"<<
                              "Сон" <<
                              "Осознание");

    if(this->edit){
        setWindowTitle("Добавление записи");
    } else{
        load();
        setWindowTitle("Редактирование записи");
        ui->btnAdd->setText("Редактировать");
    }

    ui->btnCopy->setDefaultAction(ui->actionCopy);
    connect(ui->actionCopy, SIGNAL(triggered()),this, SLOT(copy()));
    QPixmap pCopy(":/images/copyIcon.png");
    ui->actionFont->setIcon(QIcon(pCopy));

    ui->btnCut->setDefaultAction(ui->actionCut);
    connect(ui->actionCut, SIGNAL(triggered()),this, SLOT(cut()));
    QPixmap pCut(":/images/cut-fr.png");
    ui->actionFont->setIcon(QIcon(pCut));

    ui->btnPaste->setDefaultAction(ui->actionPaste);
    connect(ui->actionPaste, SIGNAL(triggered()),this, SLOT(paste()));
    QPixmap pPaste(":/images/pasteIcon.png");
    ui->actionFont->setIcon(QIcon(pPaste));

    ui->btnUndo->setDefaultAction(ui->actionUndo);
    connect(ui->actionUndo, SIGNAL(triggered()),this, SLOT(undo()));
    QPixmap pUndo(":/images/undoIcon.png");
    ui->actionFont->setIcon(QIcon(pUndo));

    ui->btnRedo->setDefaultAction(ui->actionRedo);
    connect(ui->actionRedo, SIGNAL(triggered()),this, SLOT(redo()));
    QPixmap pRedo(":/images/redoIcon.png");
    ui->actionFont->setIcon(QIcon(pRedo));

    ui->btnFont->setDefaultAction(ui->actionFont);
    connect(ui->actionFont, SIGNAL(triggered()),this, SLOT(chooseFontButton()));
    QPixmap pFont(":/images/font.png");
    ui->actionFont->setIcon(QIcon(pFont));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::load()
{
    if(item->Category() == "---"){
        ui->boxCategory->setCurrentIndex(0);
    } else if(item->Category() == "Запись"){
        ui->boxCategory->setCurrentIndex(1);
    } else if(item->Category() == "План"){
        ui->boxCategory->setCurrentIndex(2);
    } else if(item->Category() == "Идея"){
        ui->boxCategory->setCurrentIndex(3);
    } else if(item->Category() == "Отчёт"){
        ui->boxCategory->setCurrentIndex(4);
    } else if(item->Category() == "Спорт"){
        ui->boxCategory->setCurrentIndex(5);
    } else if(item->Category() == "Учёба"){
        ui->boxCategory->setCurrentIndex(6);
    } else if(item->Category() == "Сон"){
        ui->boxCategory->setCurrentIndex(7);
    } else if(item->Category() == "Осознание"){
        ui->boxCategory->setCurrentIndex(8);
    }

    ui->edtTitle->setText(item->Title());
    ui->edtText->setHtml(item->Note());
}

void Dialog::save()
{
    if(this->edit){
        item->setDate(QDateTime::currentDateTime());
    } else {
        item->setDate(item->Date());
    }

    if(ui->boxCategory->currentIndex() == 0){
        item->setCategory("---");
    } else if(ui->boxCategory->currentIndex() == 1){
        item->setCategory("Запись");
    } else if(ui->boxCategory->currentIndex() == 2){
        item->setCategory("План");
    } else if(ui->boxCategory->currentIndex() == 3){
        item->setCategory("Идея");
    }else if(ui->boxCategory->currentIndex() == 4){
        item->setCategory("Отчёт");
    } else if(ui->boxCategory->currentIndex() == 5){
        item->setCategory("Спорт");
    } else if(ui->boxCategory->currentIndex() == 6){
        item->setCategory("Учёба");
    } else if(ui->boxCategory->currentIndex() == 7){
        item->setCategory("Сон");
    } else if(ui->boxCategory->currentIndex() == 8){
        item->setCategory("Осознание");
    }

    item->setTitle(ui->edtTitle->text());
    item->setNote(ui->edtText->toHtml());
}

void Dialog::chooseFontButton()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, QFont("Helvetica [Cronyx]", 10), this);
    if (ok) {
        ui->edtText->setFont(font);
    } else {
        QMessageBox::information(this,"Сообщение!","Вы не выбрали шрифт!");
    }
}

void Dialog::copy()
{
    ui->edtText->copy();
}

void Dialog::cut()
{
    ui->edtText->cut();
}

void Dialog::paste()
{
    ui->edtText->paste();
}

void Dialog::undo()
{
    ui->edtText->undo();
}

void Dialog::redo()
{
    ui->edtText->redo();
}

void Dialog::on_btnClose_clicked()
{
    close();
}

void Dialog::on_btnAdd_clicked()
{
    save();
    close();
}
