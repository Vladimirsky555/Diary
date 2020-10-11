#include "application.h"

#include <QtSql>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>


Application::Application(int argc, char *argv[]) :
    QApplication(argc, argv)
{
    connectToDataBase();
}

void Application::connectToDataBase()
{
    /* Перед подключением к базе данных производим проверку на её существование.
     * В зависимости от результата производим открытие базы данных или её восстановление
     * */
    if(!QFile("diary.db").exists()){
        this->restoreDataBase();
    } else {
        this->openDataBase();
    }
}

bool Application::openDataBase()
{
    /* База данных открывается по заданному пути
        * и имени базы данных, если она существует
        * */

       db = QSqlDatabase::addDatabase("QSQLITE");
       db.setHostName("diary_db");
       db.setDatabaseName("diary.db");
       if(db.open()){
           return true;
       } else {
           return false;
       }
}

bool Application::restoreDataBase()
{
    if(this->openDataBase()){
        return (this->createTable()) ? true : false;
    } else {
        QMessageBox::critical(NULL, "Сообщение!",
                              "Не удалось восстановить базу данных!");
        return false;
    }
    return false;
}

bool Application::createTable()
{
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE  diary_db  ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "date TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
                    "category VARCHAR(255),"
                    "title VARCHAR(255),"
                    "note  TEXT"
                    " )"
                    )){
        QMessageBox::critical(NULL, "Сообщение!",
                              "Ошибка создания базы данных! " +
                              query.lastError().text());

        return false;
    } else {
        return true;
    }
    return false;
}
