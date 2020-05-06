#ifndef STORAGE_H
#define STORAGE_H

#include <QObject>
#include <QAction>
#include <QDate>
#include <QDateTime>
#include <QAbstractTableModel>


/**************************************************/
class Data : public QObject
{
    Q_OBJECT

    QDateTime date;
    QString category;
    QString title;
    QString text;

public:
    Data(QObject *parent = 0);
    Data(QByteArray arr);
    virtual ~Data(){}

    QDateTime Date();
    void setDate(QDateTime);
    QString Category();
    void setCategory(QString category);
    QString Title();
    void setTitle(QString title);
    QString Text();
    void setText(QString text);

    QString getView();
    QByteArray saveIt();
};
/*******************************************************/


/*******************************************************/
//Вспомогательный класс для списка дочерних элементов (если такие будут)
//Используется также для списка корневых элементов (каталог)
class List : public QList<Data*> {
public:
    List() : QList<Data*>(){}
};
/*******************************************************/


/**************************************************/

class Model : public QAbstractTableModel
{
    Q_OBJECT

    List currentItems;//для модели
    QModelIndex currentIndex;
    int searchCount;
    QString pattern;

public:
     List searchItems;//образец массива
     List baseItems;
    //Экшены
    QList<QAction*> allActions;
    QAction *actShowItem;
    QAction *actEditItem;
    QAction *actAddItem;
    QAction *actDeleteItem;

private:
    bool checkRegExp(QRegExp rx);

public:
    Model(QObject *parent = 0);
    virtual ~Model();

    List getItems();
    Data *getItem(int id);
    Data *getItemByTitle(QString title);
    Data *getItemByDate(QDate date);
    void setItems(List currentItems);
    int getCount();
    void clearList();
    void sortItems();        
    void addItem(Data *item);

    void search(QString searchText);
    void searchByCategory(QString category);
    void searchByDate(QString choose, QDate date);
    void RenewList();


    void saveToFile();
    void loadFromFile();

protected slots:
    void on_show_item();
    void on_edit_item();
    void on_delete_item();
    void on_add_item();

public slots:
    void acceptIndexfromView(QModelIndex I);

signals:
    void sendCount(int);

    // QAbstractItemModel interface
public:
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;


    //Вспомогательные функции
protected:
    virtual Data *getData(const QModelIndex &I)const;
    virtual QVariant dataDisplay(const QModelIndex &index) const;
    virtual QVariant dataTextAlignment(const QModelIndex &index) const;
    virtual QVariant dataFont(const QModelIndex &I) const;
    virtual QVariant dataForeground(const QModelIndex &I) const;
    virtual QVariant dataToolTip(const QModelIndex &I) const;
    virtual QVariant dataBackgroundColor(const QModelIndex &I) const;
};

/**************************************************/



#endif // STORAGE_H
