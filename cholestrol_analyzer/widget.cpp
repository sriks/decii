#include <QHBoxLayout>
#include "qtscrollwheel.h"
#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
//    ui->setupUi(this);

    QtScrollWheel* scrollWheel = new QtScrollWheel(this);
    scrollWheel->setSkin("Beryl");
    scrollWheel->setMinimum(0);
    scrollWheel->setMaximum(999);
    scrollWheel->setSingleStep(1);
//    scrollWheel->resize(400,400);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(scrollWheel);
    setLayout(layout);

}

Widget::~Widget()
{
//    delete ui;
}
