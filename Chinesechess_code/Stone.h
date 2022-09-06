#ifndef STONE_H
#define STONE_H

#include <QRect>
#include <QPainter>

class Stone
{
public:
    Stone();
    ~Stone();

    enum TYPE{CHE, MA, PAO, BING, JIANG, SHI, XIANG}; //为每个棋子定义其类型

    void init(int id);  //初始化所有棋子

    int _row;         //所在行
    int _col;         //所在列
    TYPE _type;       //棋子类型
    bool _dead;       //棋子存活状态
    bool _red;        //棋子颜色
    int _id;          //棋子id

    QString name();    //画棋子时调用放回棋子中文字符

    void rotate();
};

#endif // STONE_H
