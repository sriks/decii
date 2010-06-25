#include <QWebView>
#include <QWebFrame>
#include <QDebug>

#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
iWebView = new QWebView(this);
iWebView->setObjectName("webview");
QMetaObject::connectSlotsByName(this);
iWebView->setUrl(QUrl("http://59.90.212.185:8787/publicview/?REG_NO=AP09AP8983&Submit=GO"));
}

Widget::~Widget()
{

}

void Widget::changeEvent(QEvent *e)
{

}


void Widget::on_webview_loadFinished(bool ok)
{
    qDebug()<<"on_webview_loadFinished load "<<ok;

    if(ok)
    {
        qDebug()<<iWebView->page()->mainFrame()->toPlainText();
    }
}
