#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRegExp>
#include <QJsonObject>

#include "model/model.h"
#include "dialog/additemdialog.h"

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

    Model *M;
    QJsonObject m_currentJsonObject;

public:
    MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

private:
    void createUI();

private slots:
    void on_tableView_clicked(const QModelIndex &index);//получаем id и отправляем в модель
    void on_edtSearch_returnPressed();
    void acceptCount(int count);
    void searchByCategory();
    void on_actionRefreshList_triggered();
    void on_dateEdit_clicked(const QDate &date);

signals:
    void sendIndextoModel(QModelIndex);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
