#include "mainscene.h"
#include "ui_mainscene.h"
#include <QPixmap>
#include <QPainter>
#include "mypushbutton.h"
#include <QTimer>
#include <QSound>

MainScene::MainScene(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainScene)
{
    ui->setupUi(this);
    this->setFixedSize(320, 588);
    this->setWindowIcon(QPixmap(":/res/Coin0001.png"));
    this->setWindowTitle("猴哥带你翻金币");

    QSound *startSound = new QSound(":/res/TapButtonSound.wav", this);

    connect(ui->actionQuit, &QAction::triggered, [=](){
       this->close();
    });

    MyPushButton *startBtn =  new MyPushButton(":/res/MenuSceneStartButton.png");
    startBtn->setParent(this);
    startBtn->move(this->width() * 0.5 - startBtn->width() * 0.5, this->height() * 0.7);

    chooseScene = new ChooseLevelScene;

    connect(startBtn, &MyPushButton::clicked, [=](){
        //qDebug() << "点击了开始";
        startSound->play();

        startBtn->zoom1();
        startBtn->zoom2();

        QTimer::singleShot(500, [=](){
            this->hide();
            chooseScene->setGeometry(this->geometry());
            chooseScene->show();

        });

    });

    connect(chooseScene, &ChooseLevelScene::chooseSceneBack, [=](){
        chooseScene->hide();
        this->setGeometry(chooseScene->geometry());
        this->show();

    });

}

MainScene::~MainScene()
{
    delete ui;
}

void MainScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0, 0, this->width(), this->height(), pix);



    pix.load(":/res/Title.png");
    pix = pix.scaled(pix.width() * 0.5, pix.height() * 0.5);
    painter.drawPixmap(10, 30, pix);


}


