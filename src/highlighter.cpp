#include "highlighter.h"

#include <QTextCharFormat>

Highlighter::Highlighter(QObject *parent) :
    QSyntaxHighlighter(parent)
{

}

void Highlighter::highlightBlock(const QString &text)
{
    QTextCharFormat format;

    QRegExp rx(pattern);

    if(!rx.isValid() || rx.isEmpty() || rx.exactMatch("")){
        setFormat(0, text.length(), format);
        return;
    }

    format.setBackground(Qt::yellow);
    format.setFontWeight(QFont::Bold);

    int index = rx.indexIn(text);

    while(index >= 0){
        int length = rx.matchedLength();
        setFormat(index, length, format);
        index = rx.indexIn(text, index + length);
    }
}

void Highlighter::setPattern(QString str)
{
    this->pattern = str;
}


