#include "model.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>


Model::Model(QObject *parent) :
    QSqlTableModel(parent)
{
    items = new QList<Data*>;
    s_items = new QList<Data*>;


    //Список экшенов
        {
            QAction *A = actShowItem = new QAction(tr("Просмотр"), this);
            QPixmap p(":/images/eye.png"); A->setIcon(QIcon(p));
            A->setShortcut(tr("Ctrl+S"));
            connect(A, SIGNAL(triggered()), this, SLOT(show_item()));
            allActions << A;
        }{
        QAction *A = actEditItem = new QAction(tr("Редактировать"), this);
                QPixmap p(":/images/edit.png"); A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+E"));
        connect(A, SIGNAL(triggered()), this, SLOT(edit_item()));
        allActions << A;
    }{
        QAction *A = actDeleteItem = new QAction(tr("Удалить"), this);
                QPixmap p(":/images/delete.png"); A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+D"));
        connect(A, SIGNAL(triggered()), this, SLOT(delete_item()));
        allActions << A;
    }
}

void Model::selectAll()
{
    items->clear();
    QSqlQuery qry;
    qry.prepare(
                "SELECT              \n"
                "   id,               \n"
                "   date,             \n"
                "   category,           \n"
                "   title,           \n"
                "   note                  \n"
                "   FROM diary_db          \n"
                "   ORDER BY date DESC;   \n"
                );

    if(qry.exec()){
        while(qry.next()){
            Data *item = new Data(this, qry);
            addItem(item);
        }
    } else {
        qCritical() << "Cannot execute query!";
        QSqlError err = qry.lastError();
        qCritical() << err.nativeErrorCode();
        qCritical() << err.databaseText().toUtf8().data();
        qCritical() << err.driverText().toUtf8().data();
        qDebug() << qry.executedQuery();
    }

    setQuery(qry);
}

void Model::addItem(Data *item)
{
    if(item != NULL){
        items->append(item);
    }
    return;
}

void Model::addData()
{
    Data *item = new Data();

    Dialog d(item, true);
    d.save();
    d.exec();

    //Защита от добавления, если не обозначена категория
    if(item->Category() == "---"){
        delete item;
        return;
    }

    beginResetModel();
    endResetModel();

    save_to_db(item);//В базу данных
    addItem(item);//В модель
}

void Model::setItems()
{
    this->items->clear();
    for(int i = 0; i < s_items->count(); i++){
        this->items->append(s_items->at(i));
    }
}

void Model::clearItems()
{
    items->clear();
}

bool Model::checkAllTexts(QString pattern, QString text)
{
    QRegExp rx(pattern);
    if(!checkRegExp(rx))return false;
    int pos = 0;
    while((pos = rx.indexIn(text, pos)) != -1){
        pos += rx.matchedLength();
        return true;
    }

    return false;
}


bool Model::save_to_db(Data *item)
{
    QSqlQuery query;
    query.setForwardOnly(true);

    query.prepare("INSERT INTO diary_db (date, category, title, note)"
                  "VALUES (:date, :category, :title, :note)");

    query.bindValue(":date", item->Date());
    query.bindValue(":category", item->Category());
    query.bindValue(":title", item->Title());
    query.bindValue(":note", item->Note());

    if(query.exec()) return true;

    qCritical() << query.lastError().databaseText();
    qCritical() << query.lastError().driverText();
    qCritical() << query.lastError().nativeErrorCode();

    return false;
}

bool Model::update_in_db(Data *item)
{
    QSqlQuery query;
    query.setForwardOnly(true);
    query.prepare("UPDATE diary_db SET  "
                  " id = :id, "
                  " date = :date, "
                  " category = :category, "
                  " title = :title, "
                  " note = :note "
                  "WHERE id = :id; "
                  );

    query.bindValue(":id", item->Id());
    query.bindValue(":date", item->Date());
    query.bindValue(":category", item->Category());
    query.bindValue(":title", item->Title());
    query.bindValue(":note", item->Note());

    if(query.exec()) {
        return true;
    } else {
        qCritical() << query.lastError().databaseText();
        qCritical() << query.lastError().driverText();
        qCritical() << query.lastError().nativeErrorCode();
        return false;
    }
}

bool Model::delete_from_db(Data *item)
{
    QSqlQuery query;
    query.setForwardOnly(true);
    query.prepare("DELETE FROM diary_db WHERE id = :ID ;");
    query.bindValue(":ID", item->Id());

    if(!query.exec()){
        qCritical() << query.lastError().databaseText().toUtf8().data();
        qCritical() << query.lastError().driverText();
        qCritical() << query.lastError().nativeErrorCode();
        return false;
    } else {
        return true;
    }
    return false;
}

void Model::acceptIndexfromView(QModelIndex index)
{
    this->currentIndex = index;
}

void Model::acceptPattern(QString pattern)
{
    this->pattern = pattern;
}

void Model::shutdown()
{
    emit shutdown_w();
}

bool Model::checkRegExp(QRegExp rx)
{
    if(rx.isValid() && !rx.isEmpty() && !rx.exactMatch("")){
        return true;
    } else {
        QMessageBox::information(NULL,"Информсообщение",
                                 trUtf8("Некорректный шаблон регулярного выражения!"));
        return false;
    }
}

void Model::edit_item()
{
    Data *item = getItem(currentIndex);
    if(!item)return;

    Dialog d(item, false);
    d.save();
    d.exec();
    items->replace(currentIndex.row(), item);
    update_in_db(item);

    beginResetModel();
    endResetModel();
}

void Model::delete_item()
{
    Data *item = getItem(currentIndex);
    if(!item)return;

    int n = QMessageBox::warning(0, "Предупреждение", "Неужели удалить эту запись?",
                                 "Да", "Нет", QString(), 0, 1);
    if(!n) {
        beginRemoveRows(QModelIndex(), currentIndex.row(), currentIndex.row());
        items->removeOne(item);
        delete item;
        endRemoveRows();

        delete_from_db(item);//Из базы также удаляем

        items->clear();
        selectAll();
    }
}

void Model::show_item()
{
    Data* item = getItem(currentIndex);

    if(!item){
        QMessageBox::information(NULL, "Сообщение", "Вы не выбрали ни одного элемента. "
                                       "Для выбора элемента кликните левой кнопокой мыши по нему.");
        return;
    }

    ShowItemWindow *show = new ShowItemWindow(item, pattern);

    connect(this, SIGNAL(shutdown_w()),
            show, SLOT(shutdown()));

    show->show();
}

Data *Model::getItem(const QModelIndex &index) const
{
    int id = index.row();
    if(id < 0 || id >= items->size()) return 0;
    return items->at(id);
}

QVariant Model::dataDisplay(const QModelIndex &index) const
{
    Data *item = items->at(index.row());
    switch (index.column()) {
    case 0: return item->Id();
    case 1: return item->Date().isValid() ? item->Date().toString("dd.MM.yyyy") : "";
    case 2: return item->Category();
    case 3: return item->Title();
    case 4: return item->Note();
    default: return QVariant();
    }
}

QVariant Model::dataBackground(const QModelIndex &index) const
{
    Data *item = items->at(index.row());
    QColor result;

    if(!item->isNew()) {
        if(item->Category() == "Сон"){
            result = QColor(32, 178, 170);
        } else if(item->Category() == "Учёба" ){
            result = QColor(144, 238, 144);
        } else if(item->Category() == "Запись" ){
            result = QColor(255, 255, 0);
        } else if(item->Category() == "Отчёт" ){
            result = QColor(250, 235, 215);
        } else if(item->Category() == "План" ){
            result = QColor(245, 222, 179);
        } else if(item->Category() == "Идея" ){
            result = QColor(135, 206, 235);
        } else if(item->Category() == "Спорт" ){
            result = QColor(255, 222, 173);
        } else if(item->Category() == "Осознание" ){
            result = QColor(240, 255, 255);
        } else {
            result = QColor("white");
        }
    } else {
        result = QColor("white");
    }


    result.setAlphaF(0.2);
    return result;
}

QVariant Model::dataToolTip(const QModelIndex &index) const
{
    Data *item = getItem(index);
    if(!item) return QVariant();

    switch (index.column()) {
    case 1:
    {
        if(!item->Date().isValid())return QVariant();
        return tr("%1").arg(item->Date().toString("dd.MM.yyyy"));
    }
    case 3: return item->Note();
    default: return QVariant();
    }
}

int Model::rowCount(const QModelIndex &parent) const
{
    //Если родитель существует, то кол-во строк 0
    if(!parent.isValid()){
        return items->count();
    } else {
        return 0;
    }
}

int Model::columnCount(const QModelIndex &parent) const
{
    if(!parent.isValid()){
        return 5;
    } else {
        return 0;
    }
}

QVariant Model::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:             return dataDisplay(index);
    case Qt::BackgroundRole:    return dataBackground(index);
    case Qt::ToolTipRole:             return dataToolTip(index);
        //    case Qt::TextAlignmentRole: return dataTextAlignment(index);
        //    case Qt::ForegroundRole:      return dataForeground(index);
        //    case Qt::FontRole:                  return dataFont(index);
    default:  return QVariant();
    }
}

QVariant Model::headerData(int section, Qt::Orientation orientation, int role) const
{
    //Для горизонтальных заголовков оставляем данные по умолчанию
    if(orientation != Qt::Horizontal){
        return QAbstractTableModel::headerData(section,orientation,role);
    }

    switch (role) {
    case Qt::DisplayRole:
        switch (section) {
        case 0: return tr("Индекс");
        case 1: return tr("Дата");
        case 2: return tr("Категория");
        case 3: return tr("Заголовок");
        case 4: return tr("Текст");
        default: return QVariant();
        }
    case Qt::TextAlignmentRole:
        return QVariant(Qt::AlignBaseline | Qt::AlignHCenter);
    case Qt::ForegroundRole:
    {
        QFont F;
        F.setBold(true);
        return F;
    }
    default: return QVariant();
    }
}

void Model::search(QDate date, bool _date, int flag, QString category,
                   bool _category, QString pattern, bool _note)
{
    s_items->clear();
    Data *item = new Data();

    //1) Дата 2) Категория 3) Текст
    if(_date && _category && _note){
        if(flag == 1){
            for(int i = 0; i < items->count(); i++){
                Data *item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy").right(4) == date.toString("dd.MM.yyyy").right(4)){
                    if(item->Category() == category){
                        if(checkAllTexts(pattern, item->Text())){
                            s_items->append(item);
                        }
                    }
                }
            }
        } else if(flag == 2){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy").right(7) == date.toString("dd.MM.yyyy").right(7)){
                    if(item->Category() == category){
                        if(checkAllTexts(pattern, item->Text())){
                            s_items->append(item);
                        }
                    }
                }
            }
        } else if(flag == 3){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->Date().date().toString("dd.MM.yyyy") == date.toString("dd.MM.yyyy")){
                    if(item->Category() == category){
                        if(checkAllTexts(pattern, item->Text())){
                            s_items->append(item);
                        }
                    }
                }
            }
        }//1) Дата 2) Категория
    } else if(_date && _category && !_note){
        if(flag == 1){
            for(int i = 0; i < items->count(); i++){
                Data *item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy").right(4) == date.toString("dd.MM.yyyy").right(4)){
                    if(item->Category() == category){
                        s_items->append(item);
                    }
                }
            }
        } else if(flag == 2){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy").right(7) == date.toString("dd.MM.yyyy").right(7)){
                    if(item->Category() == category){
                        s_items->append(item);
                    }
                }
            }
        } else if(flag == 3){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->Date().date().toString("dd.MM.yyyy") == date.toString("dd.MM.yyyy")){
                    if(item->Category() == category){
                        s_items->append(item);
                    }
                }
            }
        }//1) Дата 2)Текст
    } else if(_date && !_category && _note){
        if(flag == 1){
            for(int i = 0; i < items->count(); i++){
                Data *item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy").right(4) == date.toString("dd.MM.yyyy").right(4)){
                    if(checkAllTexts(pattern, item->Text())){
                        s_items->append(item);
                    }
                }
            }
        } else if(flag == 2){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy").right(7) == date.toString("dd.MM.yyyy").right(7)){
                    if(checkAllTexts(pattern, item->Text())){
                        s_items->append(item);
                    }
                }
            }
        } else if(flag == 3){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->Date().date().toString("dd.MM.yyyy") == date.toString("dd.MM.yyyy")){
                    if(checkAllTexts(pattern, item->Text())){
                        s_items->append(item);
                    }
                }
            }
        }//1) Дата
    }else if(_date && !_category && !_note){
        if(flag == 1){
            for(int i = 0; i < items->count(); i++){
                Data *item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy").right(4) == date.toString("dd.MM.yyyy").right(4)){
                    s_items->append(item);
                }
            }
        } else if(flag == 2){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->Date().toString("dd.MM.yyyy").right(7) == date.toString("dd.MM.yyyy").right(7)){
                    s_items->append(item);
                }
            }
        } else if(flag == 3){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->Date().date().toString("dd.MM.yyyy") == date.toString("dd.MM.yyyy")){
                    s_items->append(item);
                }
            }
        }//1) Категория 2) Текст
    } else if(!_date && _category && _note){
        for(int i = 0; i < items->count(); i++){
            Data *item = items->at(i);
            if(item->Category() == category){
                if(checkAllTexts(pattern, item->Text())){
                    s_items->append(item);
                }
            }
        }//1) Категория
    } else if(!_date && _category && !_note){
        for(int i = 0; i < items->count(); i++){
            Data *item = items->at(i);
            if(item->Category() == category){
                    s_items->append(item);
            }
        }//1) Текст
    }else if(!_date && !_category && _note){
        for(int i = 0; i < items->count(); i++){
            Data *item = items->at(i);
                if(checkAllTexts(pattern, item->Text())){
                    s_items->append(item);
                }
        }//Другое
    }else {
        qDebug() << "Недостаточно параметров";
    }

    if(s_items->count() == 0){
        items->clear();
        return;
    }

    beginResetModel();
    setItems();
    endResetModel();
}
