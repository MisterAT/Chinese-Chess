#include "start_interface.h"
#include <QMenuBar>
#include<QPainter>
#include<QTimer>
#include<QDebug>
#include"allpushbutton.h"
#include "SingleGame.h"
#include <QSound>

start_interface::start_interface(QWidget *parent) : QMainWindow(parent)
{
    this->setFixedSize(550,788);
    this->setWindowIcon(QPixmap(":/new/prefix/resource/denglu.png"));
    this->setWindowTitle("中国象棋");
    QMenuBar * menu = menuBar();                                      //创建菜单栏
    setMenuBar(menu);                                               //添加菜单栏
    QMenu * start_menu = menu->addMenu("开始");                      //添加菜单选项
    QAction * quitAction = start_menu->addAction("退出");
    connect(quitAction,&QAction::triggered,[=](){                    //点击退出选项时程序执行退出
        this-> close();
    });
    boardScence = new SingleGame(2);                                 //创建三个对应的游戏界面
    yibanScence = new SingleGame(4);
    players = new Board;
    AllPushbutton * auto_btn1 = new AllPushbutton(":/new/prefix/resource/rumen2.png");  //创建界面按钮
    auto_btn1->setParent(this);                                                         //设置按钮的父母
    auto_btn1 -> move(this->width()*0.5-auto_btn1->width()*0.5,this->height()*0.5);     //设置按钮位置

    AllPushbutton * auto_btn2 = new AllPushbutton(":/new/prefix/resource/yiban2.png");
    auto_btn2->setParent(this);
    auto_btn2 -> move(this->width()*0.5-auto_btn2->width()*0.5,this->height()*0.6);

    AllPushbutton * play_btn = new AllPushbutton(":/new/prefix/resource/haoyou2.png");
    play_btn->setParent(this);
    play_btn-> move(this->width()*0.5-play_btn->width()*0.5,this->height()*0.7);
    QSound *startSound = new QSound(":/new/prefix/resource/music1.wav",this);
    connect(auto_btn1,&AllPushbutton::clicked,[=](){
        auto_btn1->zoom1();
        auto_btn1->zoom2();
        QTimer::singleShot(500,this, [=](){     //为了使弹跳动画效果能够执行，使转换界面延迟执行
            this->hide();                       //隐藏主界面
            startSound->setLoops(-1);
            startSound->play();
            boardScence->show();                //显示对应游戏界面
        });
    });
    connect(auto_btn2,&AllPushbutton::clicked,[=](){
        auto_btn2->zoom1();
        auto_btn2->zoom2();
        QTimer::singleShot(500,this, [=](){
            this->hide();
            startSound->setLoops(-1);
            startSound->play();
            yibanScence->show();
        });
    });
    connect(play_btn,&AllPushbutton::clicked,[=](){
        play_btn->zoom1();
        play_btn->zoom2();
        QTimer::singleShot(500,this, [=](){
            this->hide();
            startSound->setLoops(-1);
            startSound->play();
            players->show();
        });
    });
    connect(boardScence->button1,&QPushButton::clicked,[=](){
        boardScence->back();
    });
    connect(yibanScence->button1,&QPushButton::clicked,[=](){
        yibanScence->back();
    });
    connect(players->button1,&QPushButton::clicked,[=](){
        players->backOne();
    });
    connect(boardScence->button2,&QPushButton::clicked,[=](){
        qDebug()<< "返回了";
        boardScence->hide();             //隐藏游戏界面
        startSound->stop();
        this->show();                    //显示主界面
        boardScence->init(true);         //重新初始化棋局
        boardScence->_steps.clear();
        boardScence->_colTo = -1;
        boardScence->_rowTo = -1;
    });
    connect(yibanScence->button2,&QPushButton::clicked,[=](){
        qDebug()<< "返回了";
        yibanScence->hide();
        startSound->stop();
        this->show();
        yibanScence->init(true);
        yibanScence->_steps.clear();
        yibanScence->_colTo = -1;
        yibanScence->_rowTo = -1;
    });
    connect(players->button2,&QPushButton::clicked,[=](){
        players->hide();
        startSound->stop();
        this->show();
        players->init(true);
        players->_steps.clear();
        players->_colTo = -1;
        players->_rowTo = -1;
    });

}
void start_interface:: paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/new/prefix/resource/denglvye.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
    pix.load(":/new/prefix/resource/title.png");
    painter.drawPixmap(10,20,this->width()*0.8,this->height()*0.3,pix);
}
