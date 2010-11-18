#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class LocationEngine;
class QWebView;
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
    void handleErrorOccured(QString error);

private:
    void decorate(QWidget* widget);

private:
    Ui::MainWindow *ui;

private:
    QWebView* mImageWebView;
    LocationEngine* mLocationEngine;
};

#endif // MAINWINDOW_H
