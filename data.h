#ifndef DATA_H
#define DATA_H

#include <QObject>
#include <QDateTime>
#include <QVariant>

class QSqlQuery;

class Data : public QObject
{
    Q_OBJECT

    QVariant id;//идентификатор, инкрементируемый в базе
    QDateTime date;//дата
    QString category;//категория
    QString title;//заголовок
    QString note;//Запись

public:
    Data(QObject *parent = 0);
    Data(QObject *parent, QSqlQuery &qry);
    virtual ~Data(){}

    QVariant Id();
    bool isNew();

    QDateTime Date();
    void setDate(QDateTime date);

    QString Category();
    void setCategory(QString category);

    QString Title();
    void setTitle(QString title);

    QString Note();
    void setNote(QString note);

    QString Text();
};

#endif // DATA_H
