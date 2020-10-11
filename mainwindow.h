#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "model.h"
#include "dialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Model *M;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    fillBoxes();
    createUI();

private slots:
    void on_btnAll_clicked();
    void on_btnSearch_clicked();
    void on_btnAddNote_clicked();
    void on_tableView_clicked(const QModelIndex &index);
    void on_edtPattern_m_textChanged(const QString &str);

    //ToolButtons
    void clearSearchItems();


signals:
    void sendIndextoModel(QModelIndex index);
    void sendPattern(QString);//для подстветки
    void shutdown();

private:
    Ui::MainWindow *ui;

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
