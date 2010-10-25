#include <QHBoxLayout>
#include "qtscrollwheel.h"
#include "QtSvgButton"
#include "widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
//    ui->setupUi(this);

    QtScrollWheel* scrollWheel = new QtScrollWheel(this);
    scrollWheel->setSkin("Beryl");
    scrollWheel->setMinimum(0);
    scrollWheel->setMaximum(999);
    scrollWheel->setSingleStep(1);
    scrollWheel->resize(140,240);

    QtSvgButton* okButton = new QtSvgButton;
    okButton->setText("OK");
    okButton->setSkin("BerylSquare");

    okButton->resize(100,100);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(scrollWheel);
    layout->addWidget(okButton);
    setLayout(layout);

}

Widget::~Widget()
{
//    delete ui;
}
