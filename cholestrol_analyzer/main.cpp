#include <QtGui/QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QAction>
#include "widget.h"
#include "dcadataentrywidget.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DCADataEntryWidget* dataEntryWidget = new DCADataEntryWidget;
    dataEntryWidget->resize(360,640-200);
    QGraphicsView view;
    view.resize(360,640-200); //n8 size
    QGraphicsScene scene;
    scene.setSceneRect(0,0,360,640-200);
    scene.addItem(dataEntryWidget);
    view.setScene(&scene);


    // softkeys
    /* Add new softkey "MyOption" */
    QAction* newOption = new QAction("OK", &a);
    /* set soft key for Options */
    newOption->setSoftKeyRole(QAction::PositiveSoftKey);

    /* Add new softkey "MyOption" */
    QAction* newExit = new QAction("Cancel", &a);
    /* set soft key for Exit */
    newExit->setSoftKeyRole(QAction::NegativeSoftKey);

    QList<QAction *> softKeys;
    softKeys.append(newOption);
    softKeys.append(newExit);
    view.addActions(softKeys);
    view.showMaximized();

    return a.exec();
}
