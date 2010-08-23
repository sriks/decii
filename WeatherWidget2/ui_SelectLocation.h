/********************************************************************************
** Form generated from reading UI file 'SelectLocation.ui'
**
** Created: Thu Jun 24 20:39:49 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELECTLOCATION_H
#define UI_SELECTLOCATION_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_selectlocation
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QLabel *label_2;
    QPushButton *pushButton_ok;
    QPushButton *pushButton_cancel;
    QListWidget *listWidget;

    void setupUi(QWidget *selectlocation)
    {
        if (selectlocation->objectName().isEmpty())
            selectlocation->setObjectName(QString::fromUtf8("selectlocation"));
        selectlocation->resize(249, 278);
        selectlocation->setAutoFillBackground(true);
        gridLayout = new QGridLayout(selectlocation);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetMinimumSize);
        label = new QLabel(selectlocation);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 2);

        lineEdit = new QLineEdit(selectlocation);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        gridLayout->addWidget(lineEdit, 1, 0, 1, 2);

        label_2 = new QLabel(selectlocation);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        pushButton_ok = new QPushButton(selectlocation);
        pushButton_ok->setObjectName(QString::fromUtf8("pushButton_ok"));

        gridLayout->addWidget(pushButton_ok, 3, 0, 1, 1);

        pushButton_cancel = new QPushButton(selectlocation);
        pushButton_cancel->setObjectName(QString::fromUtf8("pushButton_cancel"));

        gridLayout->addWidget(pushButton_cancel, 3, 1, 1, 1);

        listWidget = new QListWidget(selectlocation);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        gridLayout->addWidget(listWidget, 4, 0, 1, 2);


        retranslateUi(selectlocation);

        QMetaObject::connectSlotsByName(selectlocation);
    } // setupUi

    void retranslateUi(QWidget *selectlocation)
    {
        selectlocation->setWindowTitle(QApplication::translate("selectlocation", "SelectLocation", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("selectlocation", "Location", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("selectlocation", "Example: Hyderabad,India", 0, QApplication::UnicodeUTF8));
        pushButton_ok->setText(QApplication::translate("selectlocation", "Ok", 0, QApplication::UnicodeUTF8));
        pushButton_cancel->setText(QApplication::translate("selectlocation", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class selectlocation: public Ui_selectlocation {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTLOCATION_H
