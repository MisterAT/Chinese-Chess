#ifndef ALLPUSHBUTTON_H
#define ALLPUSHBUTTON_H

#include <QPushButton>

class AllPushbutton : public QPushButton
{
    Q_OBJECT
public:
    AllPushbutton(QWidget *);
    AllPushbutton(QString normalImg, QString pressImg = "");     //参数1：按钮正常状态的图片，参数2：按钮按下时的图片
    QString normalImgPath;    //按扭正常状态的图片地址
    QString pressImgPath;     //按扭点击状态的图片地址
    void zoom1();         //但按钮被点击时，有一个弹跳的功能
    void zoom2();



signals:

};

#endif // ALLPUSHBUTTON_H
