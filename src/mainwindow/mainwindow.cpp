#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDate>
#include <QDebug>
#include <QHeaderView>
#include <QMessageBox>
#include <QJsonArray>
#include <QJsonDocument>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createUI();
    setWindowTitle("Дневник");


    ui->boxCategory->addItems(QStringList() << "---" << "Запись" <<
                              "Напоминание" << "Отчёт" << "Спорт" << "Учёба"
                              << "Сон" << "Осознание");

    ui->btnSearchByCategory->setDefaultAction(ui->actionSearchByCategory);//привязали к toolbuttonЖаЖ:
    connect(ui->actionSearchByCategory, SIGNAL(triggered()),
                this, SLOT(searchByCategory()));
    QPixmap p(":/images/search.png");
    ui->actionSearchByCategory->setIcon(QIcon(p));

    QPixmap pix(":/images/diaryLabel.png");
    ui->label->setPixmap(pix.scaled(215,183));

    ui->rDay->setChecked(true);
    M->searchByDate("day", QDate::currentDate());
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::createUI()
{
    M = new Model(this);
    ui->tableView->setModel(M);

    connect(this, SIGNAL(sendIndextoModel(QModelIndex)),
            M, SLOT(acceptIndexfromView(QModelIndex)));

    connect(M, SIGNAL(sendCount(int)),
            this, SLOT(acceptCount(int)));

    ui->tableView->addActions(M->allActions);
    ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableView->setColumnHidden(3, true);
    M->sortItems();
}


void MainWindow::acceptCount(int count)
{
    setWindowTitle("Дневник: [" + QString::number(count) + "] записей");
}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    emit sendIndextoModel(index);
}

//Поиск по тексту
void MainWindow::on_edtSearch_returnPressed()
{
    M->search(ui->edtSearch->text());
}

//Поиск по категории
void MainWindow::searchByCategory()
{
    M->searchByCategory(ui->boxCategory->currentText());
}

//Поиск по дате
void MainWindow::on_dateEdit_clicked(const QDate &date)
{
    QString choose;

    if(ui->rYear->isChecked()){
        choose = "year";
        qDebug() << "Ghbd";
    } else if(ui->rMonth->isChecked()){
        choose = "month";
    } else if(ui->rDay->isChecked()){
        choose = "day";
    }

    M->searchByDate(choose, date);
}


void MainWindow::on_actionRefreshList_triggered()
{
    M->RenewList();
}

