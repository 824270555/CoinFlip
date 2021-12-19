#ifndef MYCOIN_H
#define MYCOIN_H

#include <QPushButton>
#include <QTimer>

class MyCoin : public QPushButton
{
    Q_OBJECT
public:
    explicit MyCoin(QWidget *parent = 0);

    MyCoin(QString btnImg);

    int posX;
    int posY;
    bool flag;

    void changeFlag();
    QTimer *timer1; //正转反
    QTimer *timer2; //反转正
    int min = 1;
    int max = 8;

    bool isAnimation = false;

    void mousePressEvent(QMouseEvent *e);

    bool isWin = false;

signals:

public slots:
};

#endif // MYCOIN_H
