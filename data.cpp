#include "data.h"

#include <QtSql>

Data::Data(QObject *parent) : QObject(parent){}

Data::Data(QObject *parent, QSqlQuery &qry) :
    QObject(parent)
{
    this->id = qry.value("id").toInt();
    this->date = qry.value("date").toDateTime();
    this->category = qry.value("category").toString();
    this->title = qry.value("title").toString();
    this->note = qry.value("note").toString();
}

QVariant Data::Id()
{
    return this->id;
}

bool Data::isNew()
{
   if(!id.isValid())return true;//Id не валидный, значит нулевой
    if(id.isNull()) return true;//Указатель нулевой, значит новый элемент
    return false;
}

QDateTime Data::Date()
{
    return this->date;
}

void Data::setDate(QDateTime date)
{
    this->date = date;
}

QString Data::Category()
{
    return this->category;
}

void Data::setCategory(QString category)
{
    this->category = category;
}

QString Data::Title()
{
   return this->title;
}

void Data::setTitle(QString title)
{
    this->title = title;
}

QString Data::Note()
{
    return this->note;
}

void Data::setNote(QString text)
{
    this->note = text;
}

QString Data::Text()
{
    QString text = this->title + " " + this->note;
    return text;
}
