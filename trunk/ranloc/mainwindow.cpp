#include <QUrl>
#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "locationengine.h"

const QString KHtmlFormat("<head><body>%1</body></head>");

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->locationLabel->resize(360,20);
    connect(ui->buttonNext,SIGNAL(clicked()),this,SLOT(handleAnotherLocation()));
//    QString style("background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #2198c0, stop: 1 #0d5ca6);");
//    ui->locationLabel->setStyleSheet(style);
    ui->locationLabel->resize(10,10);
    mLocationEngine = new LocationEngine(this);
    connect(mLocationEngine,SIGNAL(newLocationAvailable()),this,SLOT(handleAnotherLocation()));
//    requestNewLocations();
    handleAnotherLocation();
}

void MainWindow::requestNewLocations()
{
    mLocationEngine->newLocation(QUrl("http://ws.geonames.org/wikipediaBoundingBox?north=17&south=57&east=-22.4&west=55.2"));
}

void MainWindow::handleAnotherLocation()
{
    if(mLocationEngine->count() == mLocationEngine->currentIndex() || 0 == mLocationEngine->count())
    {
        requestNewLocations();
    }

    else
    {
    LocationDetails* details = mLocationEngine->nextLocation();
    qDebug()<<details->title;
    ui->locationLabel->setText(details->title);
    QString html(KHtmlFormat);
    html.arg(details->summary);
    ui->webView->setHtml(html);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
