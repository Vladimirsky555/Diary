#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

    QString pattern;

public:    
    Highlighter(QObject *parent = 0);

    void highlightBlock(const  QString &text);
    void setPattern(QString str);
};

#endif // HIGHLIGHTER_H
