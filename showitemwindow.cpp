#include "showitemwindow.h"
#include "ui_showitemwindow.h"

ShowItemWindow::ShowItemWindow(Data *item, QString pattern, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShowItemWindow)
{
    ui->setupUi(this);
    setWindowTitle("Просмотр записи");

    this->item = item;

    //Подсветка поисковой фразы
    highlighter = new RegexpHighlighter(this);
    highlighter->setDocument(ui->edtText->document());
    highlighter->setPattern(pattern);
    highlighter->rehighlight();

    ui->dateEdit->setReadOnly(true);
    ui->edtTitle->setReadOnly(true);
    ui->edtCategory->setReadOnly(true);

    ui->dateEdit->setDateTime(item->Date());
    ui->edtCategory->setText(item->Category());
    ui->edtTitle->setText(item->Title());
    ui->edtText->setHtml(item->Note());
}

ShowItemWindow::~ShowItemWindow()
{
    delete ui;
}

void ShowItemWindow::shutdown()
{
    close();
}
