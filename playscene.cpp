#include "playscene.h"
#include <QDebug>
#include <QMenuBar>
#include <QPainter>
#include "mypushbutton.h"
#include <QTimer>
#include <QLabel>
#include <mycoin.h>
#include <dataconfig.h>
#include <QPropertyAnimation>
#include <QSound>

PlayScene::PlayScene(QWidget *parent) : QMainWindow(parent)
{

}

PlayScene::PlayScene(int levelNum)
{
    this->levelIndex = levelNum;
    QString str = QString("点击了第%1关").arg(levelNum);
    qDebug() << str.toUtf8().data();

    this->setFixedSize(320, 588);
    this->setWindowIcon(QPixmap(":/res/Coin0001.png"));
    this->setWindowTitle("翻金币场景");

    QMenuBar *bar = menuBar();
    this->setMenuBar(bar);

    QMenu *startMenu =  bar->addMenu("开始");

    QAction *quitAction = startMenu->addAction("退出");

    connect(quitAction, &QAction::triggered, [=](){
        this->close();
    });

    MyPushButton *backBtn = new MyPushButton(":/res/BackButton.png", ":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width() - backBtn->width(), this->height() - backBtn->height());

    QSound *backSound = new QSound(":/res/BackButtonSound.wav", this);
    QSound *flipSound = new QSound(":/res/ConFlipSound.wav", this);
    QSound *winSound = new QSound(":/res/LevelWinSound.wav", this);

    connect(backBtn, &MyPushButton::clicked, [=](){
        qDebug() << "翻金币场景点击返回按钮";
        backSound->play();
        QTimer::singleShot(300, this, [=](){

            emit chooseSceneBack();
        });

    });



    QLabel *label = new QLabel;
    label->setParent(this);
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(20);
    label->setFont(font);
    label->setGeometry(30, this->height() - 50, 120, 50);
    label->setText(QString("Level: %1").arg(this->levelIndex));

    DataConfig config;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            this->gameArray[i][j] = config.mData[this->levelIndex][i][j];
        }
    }

    QLabel *winLabel = new QLabel;
    QPixmap temPix;
    temPix.load(":/res/LevelCompletedDialogBg.png");
    winLabel->setGeometry(0, 0, temPix.width(), temPix.height());
    winLabel->setPixmap(temPix);
    winLabel->setParent(this);
    winLabel->move((this->width() - temPix.width()) * 0.5, -temPix.height());



    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            QPixmap pix;
            pix.load(":/res/BoardNode(1).png");
            label = new QLabel;
            label->setGeometry(0, 0, pix.width(), pix.height());
            label->setPixmap(pix);
            label->setParent(this);
            label->move(57 + i*50, 200 + j*50);

            QString str;

            if (this->gameArray[i][j])
            {
                str = ":/res/Coin0001.png";
            }
            else
            {
                str = ":/res/Coin0008.png";
            }

            MyCoin *coin = new MyCoin(str);
            coin->setParent(this);
            coin->move(59 + i*50, 204 + j*50);

            coin->posX = i;
            coin->posY = j;
            coin->flag = this->gameArray[i][j];


            coinBtn[i][j] = coin;

            connect(coin, &MyCoin::clicked, [=](){
                //if (coin->isAnimation == false)  也可以实现动画的完整新
                {
                    flipSound->play();
                    //解决翻金币的同时还可以点击其他金币
                    for (int i = 0; i < 4; i++)
                    {
                        for (int j = 0; j < 4; j++)
                        {
                            coinBtn[i][j]->isWin = true;
                        }
                    }

                    coin->changeFlag();
                    this->gameArray[i][j] = this->gameArray[i][j] == 1? 0 : 1;//维护点击的按钮

                    QTimer::singleShot(300, this, [=](){
                        if (coin->posX + 1 <= 3)
                        {
                            coinBtn[coin->posX+1][coin->posY]->changeFlag();
                            this->gameArray[coin->posX+1][coin->posY] = this->gameArray[coin->posX+1][coin->posY] == 1? 0 : 1;//维护被点击按钮的周围按钮
                        }
                        if (coin->posX - 1 >= 0)
                        {
                            coinBtn[coin->posX-1][coin->posY]->changeFlag();
                            this->gameArray[coin->posX-1][coin->posY] = this->gameArray[coin->posX-1][coin->posY] == 1? 0 : 1;//维护被点击按钮的周围按钮
                        }

                        if (coin->posY + 1 <= 3)
                        {
                            coinBtn[coin->posX][coin->posY + 1]->changeFlag();
                            this->gameArray[coin->posX][coin->posY + 1] = this->gameArray[coin->posX][coin->posY + 1] == 1? 0 : 1;//维护被点击按钮的周围按钮
                        }
                        if (coin->posY - 1 >= 0)
                        {
                            coinBtn[coin->posX][coin->posY - 1]->changeFlag();
                            this->gameArray[coin->posX][coin->posY - 1] = this->gameArray[coin->posX][coin->posY - 1] == 1? 0 : 1;//维护被点击按钮的周围按钮
                        }
                        //解决翻金币的同时还可以点击其他金币
                        for (int i = 0; i < 4; i++)
                        {
                            for (int j = 0; j < 4; j++)
                            {
                                coinBtn[i][j]->isWin = false;
                            }
                        }

                        this->isWin = true;
                        for (int i = 0; i < 4; i++)
                        {
                            for (int j = 0; j < 4; j++)
                            {
                                if (!coinBtn[i][j]->flag)
                                {
                                    this->isWin = false;
                                    break;
                                }
                            }
                        }

                        if (this->isWin)
                        {
                            qDebug() << "胜利";
                            winSound->play();
                            for (int i = 0; i < 4; i++)
                            {
                                for (int j = 0; j < 4; j++)
                                {
                                    coinBtn[i][j]->isWin = true;
                                }
                            }

                            QPropertyAnimation *animation = new QPropertyAnimation(winLabel, "geometry");
                            animation->setDuration(1000);
                            animation->setStartValue(QRect(winLabel->x(), winLabel->y(), winLabel->width(), winLabel->height()));
                            animation->setEndValue(QRect(winLabel->x(), winLabel->y() + 114, winLabel->width(), winLabel->height()));
                            animation->setEasingCurve(QEasingCurve::OutBounce);
                            animation->start();
                        }

                    });
                }
            });
        }
    }

}

void PlayScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0, 0, this->width(), this->height(), pix);



    pix.load(":/res/Title.png");
    pix = pix.scaled(pix.width() * 0.5, pix.height() * 0.5);
    painter.drawPixmap(10, 30, pix);
}

