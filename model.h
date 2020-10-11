#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QSqlTableModel>
#include <QAction>

#include "data.h"
#include "dialog.h"
#include "showitemwindow.h"

class Model : public QSqlTableModel
{

    Q_OBJECT

    QList<Data*> *items;
    QList<Data*> *s_items;
    QModelIndex currentIndex;
    QString pattern;//для подстветки

public:
    //Экшены
    QList<QAction*> allActions;
    QAction *actEditItem;
    QAction *actDeleteItem;
    QAction *actShowItem;

public:
    Model(QObject *parent = 0);
    ~Model(){}

    void selectAll();
    void addItem(Data *item);//В модель
    void addData();//В модель и базу

    void setItems();
    void clearItems();

    bool checkAllTexts(QString pattern, QString report);
    void search(QDate date, bool _date, int flag, QString category,
                bool _category, QString pattern, bool _note);

protected slots:
    void edit_item();//В модель и базу
    void delete_item();
    void show_item();

private slots:
    bool save_to_db(Data *item);
    bool update_in_db(Data *item);
    bool delete_from_db(Data* item);

    void acceptIndexfromView(QModelIndex index);
    void acceptPattern(QString pattern);//для подстветки
    void shutdown();

private:
    bool checkRegExp(QRegExp rx);

    //Вспомогательные функции
protected:
    virtual Data *getItem(const QModelIndex &index)const;
    virtual QVariant    dataDisplay(const QModelIndex &index) const;
    virtual QVariant    dataBackground(const QModelIndex &index) const;//Заливка ячейки
    virtual QVariant dataToolTip(const QModelIndex &index) const;

    // QAbstractItemModel interface
public:
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

signals:
    void shutdown_w();
};

#endif // MODEL_H
