#include "allpushbutton.h"
#include<QDebug>
#include<QPropertyAnimation>

AllPushbutton::AllPushbutton(QWidget *parent)
{
    this->setParent(parent);
}

AllPushbutton::AllPushbutton(QString normalImg, QString pressImg)
{
     this->normalImgPath = normalImg;
     this->pressImgPath = pressImg;
     QPixmap pix;
     bool che = pix.load(normalImg);
     if(!che)
     {
         qDebug()<< "图片加载失败";
         return;
     }
     this->setFixedSize(pix.width(),pix.height());          //设置按钮大小
     this->setStyleSheet("QPushButton{border:0px;}");       //设计图片的格式：边缘虚化
     this->setIcon(pix);                                    //为按钮加载图片
     this->setIconSize(QSize(pix.width(),pix.height()));    //设置按钮大小

}

 void AllPushbutton::zoom1()       //按钮向下弹跳动画的实现
 {
     QPropertyAnimation * animation = new QPropertyAnimation(this,"geometry");
     animation->setDuration(120);
     animation->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));  //设置按钮弹跳的初始位置
     animation->setEndValue(QRect(this->x(),this->y()+5,this->width(),this->height()));  //设置按钮弹跳的结束位置
     animation->setEasingCurve(QEasingCurve::OutBounce);
     animation->start();
 }


 void AllPushbutton:: zoom2()     //使按钮恢复到原位置
 {
     QPropertyAnimation * animation = new QPropertyAnimation(this,"geometry");
     animation->setDuration(120);
     animation->setStartValue(QRect(this->x(),this->y()+5,this->width(),this->height()));
     animation->setEndValue(QRect(this->x(),this->y(),this->width(),this->height()));
     animation->setEasingCurve(QEasingCurve::OutBounce);
     animation->start();

 }
