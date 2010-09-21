#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUrl>

namespace Ui {
    class MainWindow;
}

class QSystemTrayIcon;
class RSSManager;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void handleUpdateAvailable(QUrl sourceUrl, int updateditems);
    void addFeedUrl();
    void noticationActivated();
private:
    Ui::MainWindow *ui;
    RSSManager* mRSSManager;
    QList<QUrl> mSourceList;
    QSystemTrayIcon* mTray;
    QUrl mCurrentNotificationUrl;
};

#endif // MAINWINDOW_H
