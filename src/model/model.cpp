#include "model.h"

#include <QFile>
#include <QFont>
#include <QColor>
#include <QIODevice>
#include <QDataStream>
#include <QMessageBox>
#include <QBrush>
#include <QDebug>

#include "dialog/additemdialog.h"
#include "dialog/showitemdialog.h"



//Класс Storage
/*********************************************************/
Model::Model(QObject *parent) :
    QAbstractTableModel(parent)
{
    this->loadFromFile();
    this->sortItems();

    //Экшены
    {
        QAction *A = actShowItem = new QAction(tr("Просмотр"), this);
        QPixmap p(":/images/show.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+O"));
        connect(A, SIGNAL(triggered()), this, SLOT(on_show_item()));
        allActions << A;
    }{
        QAction *A = actEditItem = new QAction(tr("Редактирование"), this);
        QPixmap p(":/images/edit.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+S"));
        connect(A, SIGNAL(triggered()), this, SLOT(on_edit_item()));
        allActions << A;
    }{
        QAction *A = actAddItem = new QAction(tr("Добавить"), this);
        QPixmap p(":/images/add1.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+N"));
        connect(A, SIGNAL(triggered()), this, SLOT(on_add_item()));
        allActions << A;
    }{
        QAction *A = actDeleteItem = new QAction(tr("Удалить"), this);
        QPixmap p(":/images/delete1.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+D"));
        connect(A, SIGNAL(triggered()), this, SLOT(on_delete_item()));
        allActions << A;
    }
}

Model::~Model()
{
    beginResetModel();
    clearList();
    setItems(baseItems);
    endResetModel();
    this->saveToFile();
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

List Model::getItems()
{    
    return currentItems;
}

int Model::getCount()
{
    return currentItems.count();
}

void Model::clearList()
{
    currentItems.clear();
}


Data *Model::getItem(int id)
{
    return currentItems[id];
}

Data *Model::getItemByTitle(QString title)
{
    for(int i = 0; i < currentItems.count(); i++){
        if(currentItems.at(i)->Title() == title){
            return currentItems.at(i);
        }
    }
    return NULL;
}

Data *Model::getItemByDate(QDate date)
{
    for(int i = 0; i < currentItems.count(); i++){
        if(currentItems.at(i)->Date().date() == date){
            return currentItems.at(i);
        }
    }
    return NULL;
}



void Model::setItems(List items)
{
    this->currentItems.clear();
    for(int i = 0; i < items.count(); i++){
        this->currentItems.append(items.at(i));
    }
}

void Model::addItem(Data *item)
{
    currentItems.append(item);
    baseItems.append(item);
    emit sendCount(currentItems.count());
}


void Model::search(QString searchText)
{
    //         У нас 3 списка.
    //         Перед началом поиска обновляем tmpItems
    //         Ищем в Items и создаём список tmpItems
    //         Этот список ставим как Items, обновляем модель
    //         При следующем поиске или вызове всего списка устанавливаем
    //         как основной список уже tempItems, который не меняем

    pattern = searchText;

    setItems(baseItems);//устанавливаем исходные данные после поиска
    searchItems.clear();//очищаем временный список
    QString value;
    searchCount = 0;

    for(int i = 0; i < getCount(); i++){
        QRegExp rx(searchText);
        if(!checkRegExp(rx))return;
        int pos = 0;
        while((pos = rx.indexIn(getItem(i)->getView(), pos)) != -1){
            pos += rx.matchedLength();
            value = rx.cap(0);
            searchCount++;
            searchItems.append(getItem(i));
        }
    }

    beginResetModel();
    setItems(searchItems);
    sortItems();
    endResetModel();
}

void Model::searchByCategory(QString category)
{
    setItems(baseItems);//устанавливаем исходные данные после поиска
    searchItems.clear();//очищаем временный список

    for(int i = 0; i < getCount(); i++){
        if(getItem(i)->Category() == category){
            searchItems.append(getItem(i));
        }
    }

    beginResetModel();
    setItems(searchItems);
    sortItems();
    endResetModel();
}

void Model::searchByDate(QString choose, QDate date)
{
    setItems(baseItems);//устанавливаем исходные данные после поиска
    searchItems.clear();//очищаем временный список

    Data *D = NULL;

    if(choose == "year"){
        for(int i = 0; i < getCount(); i++){
            D = getItem(i);
            if(D->Date().date().toString(Qt::DefaultLocaleShortDate).right(4) ==
                    date.toString(Qt::DefaultLocaleShortDate).right(4)){
                searchItems.append(D);
            }
        }
    } else if(choose == "month"){
        for(int i = 0; i < getCount(); i++){
            D = getItem(i);
            if(D->Date().date().toString(Qt::DefaultLocaleShortDate).right(7) ==
                    date.toString(Qt::DefaultLocaleShortDate).right(7)){
                searchItems.append(D);
            }
        }
    } else if(choose == "day"){
        for(int i = 0; i < getCount(); i++){
            D = getItem(i);
            if(D->Date().date() == date){
                searchItems.append(D);
            }
        }
    }

    if(searchItems.count() == 0)return;

    beginResetModel();
    setItems(searchItems);
    sortItems();
    endResetModel();
}


void Model::RenewList()
{
    beginResetModel();
    clearList();
    setItems(baseItems);
    sortItems();
    endResetModel();
}


void Model::sortItems()
{
    for(int i = 0; i < currentItems.count(); i++)
    {
        for(int j = 0; j < currentItems.count()-1; j++)
        {
            QDateTime date1 = currentItems[j]->Date();
            QDateTime date2 = currentItems[j+1]->Date();

            if(date1.toString() != "" || date2.toString() != ""){
                if(date1 < date2)
                {
                    Data *tmp = new Data();
                    tmp->setDate(date1);
                    tmp->setCategory(currentItems[j]->Category());
                    tmp->setTitle(currentItems[j]->Title());
                    tmp->setText(currentItems[j]->Text());

                    currentItems[j] = currentItems[j+1];
                    currentItems[j+1] = tmp;
                }
            }
        }
    }
}

//Реализация чтения-записи в файл со ссылками
void Model::saveToFile()
{
    QFile f("data.db");
    f.open(QFile::WriteOnly|QFile::Truncate);

    QDataStream str(&f);

    for (int i = 0; i < currentItems.count(); i++)
        str << currentItems[i]->saveIt();

    f.close();
}

void Model::loadFromFile()
{
    QFile f("data.db");
    if (!f.exists()) return;

    f.open(QFile::ReadOnly);
    QByteArray data = f.readAll();
    f.close();

    QDataStream str(data);

    while (!str.atEnd()) {
        QByteArray arr;
        str >> arr;

        Data *item = new Data(arr);
        addItem(item);
    }
}

void Model::on_show_item()
{
    if(currentIndex.row() == -1){
        QMessageBox::information(NULL, "Информация",
                                 "Чтобы просмотреть или отредактировать запись, "
                                                     "необходимо кликнуть на неё левой кнопкой мыши!");
        return;
    }
    Data *D = getData(currentIndex);
    ShowItemDialog *d = new ShowItemDialog(D);
    d->setPattern(pattern);
    d->exec();
}


void Model::on_edit_item()
{
    if(currentIndex.row() == -1){
        QMessageBox::information(NULL, "Информация",
                                 "Чтобы просмотреть или отредактировать запись, "
                                                     "необходимо кликнуть на неё левой кнопкой мыши!");
        return;
    }

    Data *D = getData(currentIndex);
    AddItemDialog *dialog = new AddItemDialog(D);

    dialog->setPattern(pattern);
    dialog->exec();

    //Из-за того, что я не ввёл идентификаторы, то фактически идентификатором
    //является связка дата-заголовок
    //Дата в дневнике по умолчанию не меняется, а вот из-за отсутствия идентификатора
    //пришлось отказаться от изменения заголовков
    //Ещё это значит, что в дне не должно быть одинаковых заголовков,
    //а также то, что их невозможно отредактировать
    for(int i = 0; i < getCount(); i++){
        if(baseItems[i]->Title() == D->Title() &&
                baseItems[i]->Date() == D->Date())
            baseItems[i] = D;
    }

    beginResetModel();
    endResetModel();
}

void Model::on_add_item()
{
    Data *D = new Data();
    AddItemDialog d(D);
    d.save();
    d.exec();

    if(!d.emptyFields){
        QMessageBox::information(NULL,"Информация","Не все поля заполнены");
        delete D;
        return;
    }

    beginResetModel();
    endResetModel();
    addItem(D);
    searchByDate("day", QDate::currentDate());
}


void Model::on_delete_item()
{
    Data *D = getData(currentIndex);
    beginResetModel();
    currentItems.removeOne(D);//удаляем из временного списка
    baseItems.removeOne(D);//удаляем из основного списка
    endResetModel();
}

void Model::acceptIndexfromView(QModelIndex I)
{
    this->currentIndex = I;
}

Data *Model::getData(const QModelIndex &I) const
{
    int id = I.row();
    if(id < 0 || id >= currentItems.size()) return 0;
    return currentItems.at(id);
}


QVariant Model::dataDisplay(const QModelIndex &index) const
{
    Data *D = getData(index);
    switch (index.column()) {
    case 0: return D->Date();
    case 1: return D->Category();
    case 2: return D->Title();
    case 3: return D->Text();
    default: return QVariant();
    }
}

QVariant Model::dataTextAlignment(const QModelIndex &index) const
{
    int Result = Qt::AlignVCenter;

    if(index.column() == 0 || index.column() == 1){
        Result = Qt::AlignHCenter;
    } else if(index.column() == 2){
        Result = Qt::AlignLeft;
    }

    return Result;
}

QVariant Model::dataFont(const QModelIndex &I) const
{
    Data *D = getData(I);
    if(!D) return QVariant();
    QFont F;
//    if(D->Category() == "Отчёт") F.setStrikeOut(true);
//        F.setItalic(true);

//    if(D->Category() == "Осознание"){
//        F.setBold(true);
//    }

    return F;
}

QVariant Model::dataForeground(const QModelIndex &I) const
{
    QColor Result;
    Data *D = getData(I);
    if(!D) return QVariant();
    //Result = D->Category() == "Сон" ? QColor("blue") : QColor("black");
    //Result = D->Category() == "Учёба" ? QColor("green") : QColor("black");
//  if(!D->isActive()) Result.setAlphaF(0.50);
    return Result;
}




QVariant Model::dataToolTip(const QModelIndex &I) const
{
    Data *D = getData(I);
    if(!D) return QVariant();

    switch (I.column()) {
//    case 2: {
//        if(!D->getDate().isValid())return QVariant();
//        return tr("Valid to: %1").arg(D->getTo().toString("dd.MM.yyyy"));
//    }
//    default: return QVariant("Привет!");
      default: return D->Date().date().toString(Qt::ISODate);
    }
}

QVariant Model::dataBackgroundColor(const QModelIndex &I) const
{
    Data *D = getData(I);
    if(!D) return QVariant();
    if(D->Category() == "Сон"){
        QBrush background("#00FFFF");
        return background;
    } else if(D->Category() == "Учёба") {
        QBrush background("#9ACD32");
        return background;
    }else if(D->Category() == "Спорт") {
        QBrush background("#FFA07A");
        return background;
    }else if(D->Category() == "Запись") {
        QBrush background("#CD853F");
        return background;
    }else if(D->Category() == "Осознание") {
        QBrush background("#008080");
        return background;
    }else if(D->Category() == "Отчёт") {
        QBrush background("#9400D3");
        return background;
    } else {
        QBrush background(Qt::white);
        return background;
    }

    return QVariant();
}

int Model::rowCount(const QModelIndex &parent) const
{
    //Если родитель существует, то кол-во строк 0
    if(!parent.isValid()){
        return currentItems.count();
    } else {
        return 0;
    }
}

int Model::columnCount(const QModelIndex &parent) const
{
    if(!parent.isValid()){
        return 4;
    } else {
        return 0;
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
        case 0: return tr("Дата");
        case 1: return tr("Категория");
        case 2: return tr("Заголовок");
        case 3: return tr("Текст");
        default: return QVariant();
        }
    }

    return QVariant();
}

QVariant Model::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:         return dataDisplay(index);
    case Qt::TextAlignmentRole:   return dataTextAlignment(index);
    case Qt::ForegroundRole:      return dataForeground(index);
    case Qt::FontRole:            return dataFont(index);
    case Qt::BackgroundColorRole: return dataBackgroundColor(index);
    case Qt::ToolTipRole:         return dataToolTip(index);
    default:                      return QVariant();
    }
}





//Класс Data
/*********************************************************/
Data::Data(QObject *parent) : QObject(parent)
{
    this->date = QDateTime::currentDateTime().toLocalTime();
    this->category = "";
    this->title = "";
    this->text = "";
}

Data::Data(QByteArray arr)
{
    QDataStream str(&arr, QIODevice::ReadOnly);
    str >> this->date >> this->category >> this->title >> this->text;
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

QString Data::Text()
{
    return this->text;
}

void Data::setText(QString text)
{
    this->text = text;
}

QString Data::getView()
{
    return category + "" + title + "" + text;
}

QByteArray Data::saveIt()
{
    QByteArray arr;
    QDataStream str(&arr, QIODevice::WriteOnly);
    str << this->date << this->category <<
           this->title << this->text;
    return arr;
}


