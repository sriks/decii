// Author : Srikanth Sombhatla

#include <QtGui/QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsBlurEffect>
#include <QGraphicsProxyWidget>
#include <QPalette>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QGraphicsView view;
    QPalette viewPallete(view.palette());
    viewPallete.setBrush(QPalette::Base,QBrush(QColor(0,0,0)));
    view.setPalette(viewPallete);
    QGraphicsScene scene;
    QMessageBox* msgBox = new QMessageBox(&view);
    msgBox->setText("Do u see backlight effect?");
    QGraphicsProxyWidget* proxyDlg = scene.addWidget(msgBox);

    const int offset = 0;
    QGraphicsRectItem backLight;
    QRect backLightRect(proxyDlg->boundingRect().toRect());
    backLightRect.setSize(QSize(backLightRect.width()+offset,
                                backLightRect.height()+offset));
    backLightRect.moveCenter(msgBox->rect().center());

    backLight.setRect(backLightRect);
    backLight.setZValue(proxyDlg->zValue()-1);
    backLight.setBrush(QBrush(QColor(Qt::green)));
    backLight.setPen(QPen(Qt::NoPen));
    QGraphicsBlurEffect blurEffect;
    blurEffect.setBlurRadius(20);
    backLight.setGraphicsEffect(&blurEffect);

    // Reposition dlg and backlight to screen center
    proxyDlg->setPos(view.rect().center());
    backLight.setPos(view.rect().center());

    scene.addItem(&backLight);
    view.setScene(&scene);
    view.showMaximized();
    return a.exec();
}
