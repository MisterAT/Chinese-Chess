#ifndef STEP_H
#define STEP_H

#include <QObject>

class Step : public QObject
{
    Q_OBJECT
public:
    explicit Step(QObject *parent = 0);
    ~Step();

    int _moveid;    //移动的棋子
    int _killid;    //吃掉的棋子
    int _rowFrom;   //原来棋子所在行数
    int _colFrom;   //原来棋子所在列数
    int _rowTo;
    int _colTo;

    const Step& operator=(const Step& ste);
    bool operator==(const Step& ste);
signals:

public slots:
};

#endif // STEP_H
