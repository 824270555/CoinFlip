#include "chooselevelscene.h"
#include "mypushbutton.h"
#include <QMenuBar>
#include <QPainter>
#include <QDebug>
//#include <QPixmap>
#include <QLabel>
#include <QSound>
#include <QTimer>

ChooseLevelScene::ChooseLevelScene(QWidget *parent) : QMainWindow(parent)
{
    this->setFixedSize(320, 588);
    this->setWindowIcon(QIcon(":/res/Coin0001.png"));
    this->setWindowTitle("选择管卡场景");


    QMenuBar *bar = menuBar();
    this->setMenuBar(bar);

    QMenu *startMenu =  bar->addMenu("开始");

    QAction *quitAction = startMenu->addAction("退出");

    connect(quitAction, &QAction::triggered, [=](){
        this->close();
    });

    QSound *chooseSound = new QSound(":/res/TapButtonSound.wav", this);
    QSound *backSound = new QSound(":/res/BackButtonSound.wav", this);

    MyPushButton *backBtn = new MyPushButton(":/res/BackButton.png", ":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width() - backBtn->width(), this->height() - backBtn->height());


    connect(backBtn, &MyPushButton::clicked, [=](){
        qDebug() << "点击返回按钮";
        backSound->play();
        QTimer::singleShot(300, this, [=](){

            emit chooseSceneBack();
        });

    });

    for (int i = 0; i < 20; i++)
    {
        MyPushButton *menuBtn = new MyPushButton(":/res/LevelIcon.png");
        menuBtn->setParent(this);
        menuBtn->move(25 + i % 4 * 70, 130 + i / 4 * 70);

        connect(menuBtn, &MyPushButton::clicked, [=](){

            //qDebug() << i+1;
            chooseSound->play();

            play = new PlayScene(i+1);
            this->hide();
            play->setGeometry(this->geometry());
            play->show();

            connect(play, &PlayScene::chooseSceneBack, [=](){
                this->setGeometry(play->geometry());
                delete play;
                this->show();
                play = NULL;
            });
        });

        QLabel *label = new QLabel;
        label->setParent(this);
        label->setFixedSize(menuBtn->width(), menuBtn->height());
        label->setText(QString::number(i+1));
        label->move(25 + i % 4 * 70, 130 + i / 4 * 70);
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        label->setAttribute(Qt::WA_TransparentForMouseEvents);
    }

}

void ChooseLevelScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/OtherSceneBg.png");
    painter.drawPixmap(0, 0, this->width(), this->height(),pix);

    pix.load(":/res/Title.png");
    painter.drawPixmap((this->width() - pix.width()) * 0.5, 30, pix.width(), pix.height(), pix);
}

