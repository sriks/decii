#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
    class Widget;
}

class QWebView;
class Widget : public QWidget {
    Q_OBJECT
public:
    Widget(QWidget *parent = 0);
    ~Widget();

protected slots:
    void on_webview_loadFinished(bool);

protected:
    void changeEvent(QEvent *e);

private:

    QWebView* iWebView;
private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
