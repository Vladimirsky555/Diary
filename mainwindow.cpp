#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QColorDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Дневник");

    M = new Model(this);
    M->setEditStrategy(QSqlTableModel::OnFieldChange);

    ui->dateEdit_m->setDate(QDate::currentDate());

    QPixmap pix(":/images/diaryLabel.png");
    ui->lblPix->setPixmap(pix.scaledToWidth(200));

    ui->btnClear->setDefaultAction(ui->actionClear);
    connect(ui->actionClear, SIGNAL(triggered()),
            this, SLOT(clearSearchItems()));


    ui->cbxCategory->addItems(QStringList() <<
                              "---" <<
                              "Запись" <<
                              "План" <<
                              "Идея" <<
                              "Отчёт" <<
                              "Спорт" <<
                              "Учёба"<<
                              "Сон" <<
                              "Осознание");


    createUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

MainWindow::createUI()
{
    ui->tableView->setModel(M);

    connect(this, SIGNAL(sendIndextoModel(QModelIndex)),
            M, SLOT(acceptIndexfromView(QModelIndex)));

    connect(this, SIGNAL(shutdown()),
            M, SLOT(shutdown()));

    connect(this, SIGNAL(sendPattern(QString)),
            M, SLOT(acceptPattern(QString)));

    ui->tableView->addActions(M->allActions);
    ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->setColumnHidden(4, true);
//    ui->tableView->setAlternatingRowColors(true);
}

void MainWindow::on_btnAll_clicked()
{
    M->selectAll();
}


void MainWindow::on_btnAddNote_clicked()
{
    M->addData();
}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    emit sendIndextoModel(index);
}

void MainWindow::clearSearchItems()
{
    ui->rYear_m->setChecked(true);
    ui->cbxCategory->setCurrentIndex(0);
    ui->edtPattern_m->setText("");
}


void MainWindow::on_btnSearch_clicked()
{
    QDate date;
    QString category;
    QString note;
    int flag;

    bool _date;
    bool _category;
    bool _note;

    M->clearItems();
    M->selectAll();//Заполняем массив items, иначе не будет результатов поиска

    date = ui->dateEdit_m->date();

    if(ui->rYear_m->isChecked()){
        flag = 1;
    } else if(ui->rMonth_m->isChecked()){
        flag = 2;
    } else if(ui->rDay_m->isChecked()){
        flag = 3;
    }

    if(ui->cbxCategory->currentIndex() == 0){
        category = "---"; _category = false;
    } else if(ui->cbxCategory->currentIndex() == 1){
        category = "Запись"; _category = true;
    } else if(ui->cbxCategory->currentIndex() == 2){
        category = "План"; _category = true;
    } else if(ui->cbxCategory->currentIndex() == 3){
        category = "Идея"; _category = true;
    }else if(ui->cbxCategory->currentIndex() == 4){
        category = "Отчёт"; _category = true;
    } else if(ui->cbxCategory->currentIndex() == 5){
        category = "Спорт"; _category = true;
    } else if(ui->cbxCategory->currentIndex() == 6){
        category = "Учёба"; _category = true;
    } else if(ui->cbxCategory->currentIndex() == 7){
        category = "Сон"; _category = true;
    } else if(ui->cbxCategory->currentIndex() == 8){
        category = "Осознание"; _category = true;
    }

    //Определяем какие параметры использовать
    if(ui->cbDate->isChecked()){
        _date = true;
    } else {
        _date = false;
    }

    note = ui->edtPattern_m->text();
    if(note == ""){
        _note = false;
    } else {
        _note = true;
    }

    M->search(date, _date, flag, category, _category, note, _note);
}

void MainWindow::on_edtPattern_m_textChanged(const QString &str)
{
    emit sendPattern(str);//отправляем поисковое слово для подстветки
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    emit shutdown();
    QWidget::closeEvent(event);
}

