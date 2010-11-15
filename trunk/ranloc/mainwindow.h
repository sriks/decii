#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class LocationEngine;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected slots:
    void requestNewLocations();
    void handleAnotherLocation();
    void handleMoreAboutLocation();

private:
    Ui::MainWindow *ui;

private:
    LocationEngine* mLocationEngine;
};

#endif // MAINWINDOW_H
