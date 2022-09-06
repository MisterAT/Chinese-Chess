#ifndef START_INTERFACE_H
#define START_INTERFACE_H

#include <QMainWindow>
#include"Board.h"
#include"SingleGame.h"

class start_interface : public QMainWindow
{
    Q_OBJECT
public:
    explicit start_interface(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);    //对界面进行设计
    SingleGame *boardScence = NULL;    //进入入门难度的界面按钮
    SingleGame *yibanScence = NULL;    //进入一般难度的界面按钮
    Board *players =NULL;              //进入好友对战的界面按钮

signals:

};

#endif // START_INTERFACE_H
