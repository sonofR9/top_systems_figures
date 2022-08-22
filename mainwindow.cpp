#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    QPixmap *pixmap = new QPixmap(500,500);
    MyFigures::painter = new QPainter(pixmap);

    MyFigures::painter->setPen(QColor(255,255,255));

    MyRectangle rec_1(100,20, MY_PI/4, std::pair(60,60));
    rec_1.Draw();

    rec_1.Rotate(MY_PI/2);
    rec_1.Translate(50,50);
    rec_1.Draw();

    MyEllipse ell_1(40,20, MY_PI/6, std::pair(100,100));
    ell_1.Draw();

    ell_1.Rotate(MY_PI/2);
    ell_1.Translate(40,0);
    ell_1.Draw();

    MyTriangle tr_1(100,50,100,15,std::pair(100,100));
    tr_1.Draw();

    scene->addPixmap(*pixmap);
}

MainWindow::~MainWindow()
{
    delete ui;
}

