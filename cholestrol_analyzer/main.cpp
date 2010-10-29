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
    QGraphicsView view;
    QGraphicsScene scene;
    scene.addItem(dataEntryWidget);
    view.setScene(&scene);
    view.resize(360,640-200); //n8 size

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
    view.show();

    return a.exec();
}
