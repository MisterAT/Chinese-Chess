#ifndef BOARD_H
#define BOARD_H

#include <QFrame>
#include "Stone.h"
#include "Step.h"
#include <QVector>
#include <QLabel>
#include <QPushButton>

class Board : public QFrame
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = 0);
    ~Board();


    Stone _s[32];
    Step _circular[8];
    int _r;
    QPoint _off;
    bool _bSide;
    int _rowFrom,_colFrom;
    int _rowTo,_colTo;
    QWidget* Wid = new QWidget;
    QVector<Step*> _steps;
    QPushButton *button1 = new QPushButton("悔棋");
    QPushButton *button2 = new QPushButton("返回");

    int _selectid;
    bool _bRedTurn;
    void init(bool bRedSide);

    void paintEvent(QPaintEvent *);
    void drawPlate(QPainter& p);
    void drawPlace(QPainter& p);
    void drawInitPosition(QPainter& p);
    void drawInitPosition(QPainter& p, int row, int col);
    void drawStone(QPainter& p);
    void drawStone(QPainter& p, int id);

    QPoint center(int row, int col);
    QPoint center(int id);
    bool getClickRowCol(QPoint pt, int& r, int& c);

    bool red(int id);
    bool sameColor(int id1, int id2);
    int sameColorStone_EmptyGrid(int id1, int id2);
    bool IsOut(int row, int col );
    int getStoneId(int row, int col);
    void killStone(int id);
    void reliveStone(int id);
    void moveStone(int moveid, int row, int col);
    bool isDead(int id);
    void mouseReleaseEvent(QMouseEvent *);
    void click(QPoint pt);
    virtual void click(int id, int row, int col);
    void trySelectStone(int id);
    void tryMoveStone(int killid, int row, int col);
    void moveStone(int moveid, int killid, int row, int col);
    void saveStep(int moveid, int killid, int row, int col, QVector<Step*>& steps);
    void backOne();
    void back(Step* step);
    virtual void back();
    void pushmove(Step& step);
    bool iscircular(Step& step);



    bool canMove(int moveid, int killid, int row, int col);
    bool canMoveChe(int moveid, int killid, int row, int col);
    bool canMoveMa(int moveid, int killid, int row, int col);
    bool canMovePao(int moveid, int killid, int row, int col);
    bool canMoveBing(int moveid, int killid, int row, int col);
    bool canMoveJiang(int moveid, int killid, int row, int col);
    bool canMoveShi(int moveid, int killid, int row, int col);
    bool canMoveXiang(int moveid, int killid, int row, int col);


    bool canSelect(int id);


    int getStoneCountAtLine(int row1, int col1, int row2, int col2);
    int relation(int row1, int col1, int row, int col);
    bool isBottomSide(int id);

signals:

public slots:
    void slotBack();

};

#endif // BOARD_H
