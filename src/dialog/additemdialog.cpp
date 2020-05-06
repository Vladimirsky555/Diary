#include "additemdialog.h"
#include "ui_additemdialog.h"

#include <QMessageBox>
#include <QFontDialog>


AddItemDialog::AddItemDialog(Data *item, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddItemDialog)
{
    ui->setupUi(this);

    setWindowTitle("Добавление/Редактирование");

    highlighter = new Highlighter(this);
    highlighter->setDocument(ui->edtText->document());
    highlighter->setPattern(ui->edtPattern->text());

    this->item = item;
    this->emptyFields = false;
    load();

    ui->boxCategory->addItems(QStringList() << "---" << "Запись" <<
                              "Напоминание" << "Отчёт" << "Спорт" << "Учёба"
                              << "Сон" << "Осознание");

    if(item->Title() != "")
    ui->edtTitle->setReadOnly(true);
    //QPalette *palette = new QPalette();
    //palette->setColor(QPalette::Base,Qt::gray);
    //palette->setColor(QPalette::Text,Qt::darkGray);
    //ui->edtTitle->setPalette(*palette);

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

AddItemDialog::~AddItemDialog()
{
    delete ui;
}


void AddItemDialog::load()
{
    if(item){
        ui->edtCat->setText(item->Category());
        ui->boxCategory->setCurrentText(item->Category());
        ui->edtTitle->setText(item->Title());
        ui->edtText->setHtml(item->Text());
    }
}

bool AddItemDialog::save()
{   
//    item->setDate(item->Date());
    item->setCategory(ui->edtCat->text().simplified());
    item->setTitle(ui->edtTitle->text());
    item->setText(ui->edtText->toHtml());
    return true;
}



void AddItemDialog::defineBox()
{
    switch (ui->boxCategory->currentIndex()) {
    case 0: break;
    case 1: ui->edtCat->setText("Запись"); break;
    case 2: ui->edtCat->setText("Напоминание"); break;
    case 3: ui->edtCat->setText("Отчёт"); break;
    case 4: ui->edtCat->setText("Спорт"); break;
    case 5: ui->edtCat->setText("Учёба"); break;
    case 6: ui->edtCat->setText("Сон"); break;
    case 7: ui->edtCat->setText("Осознание"); break;
    }
}

bool AddItemDialog::checkRegExp(QRegExp rx)
{
    if(rx.isValid() && !rx.isEmpty() && !rx.exactMatch("")){
          return true;
      }else{
          QMessageBox::information(this,"Информсообщение",
                                   trUtf8("Некорректный шаблон регулярного выражения!"));
          return false;
    }
}

void AddItemDialog::setPattern(QString pattern)
{
    ui->edtPattern->setText(pattern);
}

void AddItemDialog::on_boxCategory_currentIndexChanged(int index)
{
    switch (index) {
    case 0: break;
    case 1: ui->edtCat->setText("Запись"); break;
    case 2: ui->edtCat->setText("Напоминание"); break;
    case 3: ui->edtCat->setText("Отчёт"); break;
    case 4: ui->edtCat->setText("Спорт"); break;
    case 5: ui->edtCat->setText("Учёба"); break;
    case 6: ui->edtCat->setText("Сон"); break;
    case 7: ui->edtCat->setText("Осознание"); break;
    }
}

void AddItemDialog::on_btnAdd_clicked()
{
    defineBox();

    if(ui->edtTitle->text() == "" ||
            ui->edtCat->text() == ""){
        emptyFields = false;
    }else{
        emptyFields = true;
    }

    save();
    close();
}



void AddItemDialog::on_btnClose_clicked()
{
    close();
}


void AddItemDialog::on_edtPattern_textChanged(const QString &str)
{
    highlighter->setPattern(str);
    highlighter->rehighlight();
}

void AddItemDialog::chooseFontButton()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, QFont("Helvetica [Cronyx]", 10), this);
    if (ok) {
        ui->edtText->setFont(font);
    } else {
        QMessageBox::information(this,"Сообщение!","Вы не выбрали шрифт!");
    }
}

void AddItemDialog::copy()
{
    ui->edtText->copy();
}

void AddItemDialog::cut()
{
    ui->edtText->cut();
}

void AddItemDialog::paste()
{
    ui->edtText->paste();
}

void AddItemDialog::undo()
{
    ui->edtText->undo();
}

void AddItemDialog::redo()
{
   ui->edtText->redo();
}



