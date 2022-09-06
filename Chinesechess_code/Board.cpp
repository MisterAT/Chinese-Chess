#include "Board.h"
#include"allpushbutton.h"
#include"start_interface.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <cmath>
#include<QHBoxLayout>
#include<QLabel>
#include<QPropertyAnimation>
#include<QMessageBox>
#include <QDebug>
#include <qdebug.h>

#define GetRowCol(__row, __col, __id) \
    int __row = _s[__id]._row;\
    int __col = _s[__id]._col

Board::Board(QWidget *parent) : QFrame(parent)
{
    this->_r = 20;
    setMinimumSize(_r*18+1, _r*20+1);
    init(true);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addStretch();
    layout->addWidget(button1);
    layout->addStretch();
    layout->addWidget(button2);
    layout->addStretch();
    Wid->setParent(this);
    QHBoxLayout *mainlayout = new QHBoxLayout();
    mainlayout->addStretch(7);
    mainlayout->addLayout(layout);
    mainlayout->addStretch(3);
    Wid->setLayout(mainlayout);
    Wid->show();


    this->resize(1000,800);
    this->setMinimumSize(1000,800);
    Wid->resize(_r*30,_r*20);

//    AllPushbutton *back_step = new AllPushbutton(this);
//    QPixmap pix;
//    pix.load(":/new/prefix/resource/huiqi.png.png");
//    back_step->setFixedSize(pix.width(),pix.height());
//    back_step->setStyleSheet("QPushButton{border:0px;}");
//    back_step->setIcon(pix);
//    back_step->setIconSize(QSize(pix.width(),pix.height()));
//    back_step->move(this->width()*0.75,this->height()*0.3);

//    QMessageBox::question(this,"象棋", "是否继续游戏", QString("重玩"), QString("退出"));


}

Board::~Board()
{

}

void Board::init(bool bRedSide)
{
    for(int i=0; i<32; ++i)
    {
        _s[i].init(i);
    }
    if(bRedSide)
    {
        for(int i=0; i<32; ++i)
        {
            _s[i].rotate();
        }
    }

    _rowFrom=-1;
    _colFrom=-1;
    _rowTo=-1;
    _colTo=-1;
    _selectid = -1;
    _bRedTurn = true;
    _bSide = bRedSide;
    update();
}

void Board::paintEvent(QPaintEvent *)
{

    int r = (this->geometry().width()/22)<this->geometry().height()/22?this->geometry().width()/22:this->geometry().height()/22;;
    _r = r;
    _off = QPoint(r*2, r*2);

    Wid->move(_r*20,_r*8);

    button1->resize(_r*5,_r*2);
    button2->resize(_r*5,_r*2);
    button1->move(0,0);
    button2->move(0,_r*3);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    /*绘制棋盘*/
    QPixmap pix;
    pix.load(":/new/prefix/resource/beijing.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
    painter.translate(0, 0);
    pix.load(":/new/prefix/resource/BOARD.BMP");//加载图片地址 可以是资源文件
    painter.drawPixmap(_r/2, _r/2, _r*19, _r*21, pix);//绘制图片 横坐标、纵坐标、宽度、高度
    painter.save();
    drawPlate(painter);
    painter.restore();

    painter.save();
    drawPlace(painter);
    painter.restore();

    painter.save();
    drawInitPosition(painter);
    painter.restore();

    painter.save();
    drawStone(painter);
    painter.restore();


}

void Board::drawStone(QPainter &p)
{
    for(int i=0; i<32; i++)
    {
        drawStone(p, i);
    }
}

bool Board::isDead(int id)
{
    if(id == -1)return true;
    return _s[id]._dead;
}

void Board::drawStone(QPainter &painter, int id)
{
    if(isDead(id)) return;

    /*QColor color;
    if(red(id)) color = Qt::red;
    else color = Qt::black;

    p.setPen(QPen(QBrush(color), _r/10));

    if(id == _selectid) p.setBrush(Qt::gray);
    else p.setBrush(Qt::yellow);

    p.drawEllipse(cell(id));

    p.setFont(QFont("宋体", _r*3/5, _r*1.2));
    p.drawText(cell(id), name(id), QTextOption(Qt::AlignCenter));*/
    QPixmap pix;
    painter.translate(0, 0);
    //pix.load("C:\\Users\\macbook\\Desktop\\123.png");//加载图片地址 可以是资源文件
    if(red(id)){
        switch(_s[id]._type){
        case Stone::CHE:pix.load(":/new/prefix/resource/RCHE.png");break;
        case Stone::MA:pix.load(":/new/prefix/resource/RMA.png");break;
        case Stone::PAO:pix.load(":/new/prefix/resource/RPAO.png");break;
        case Stone::XIANG:pix.load(":/new/prefix/resource/RXIANG.png");;break;
        case Stone::SHI:pix.load(":/new/prefix/resource/RSHI.png");break;
        case Stone::BING:pix.load(":/new/prefix/resource/RBING.png");break;
        case Stone::JIANG:pix.load(":/new/prefix/resource/RJIANG.png");break;
        }
    }
    else{
        switch(_s[id]._type){
        case Stone::CHE:pix.load(":/new/prefix/resource/BCHE.png");break;
        case Stone::MA:pix.load(":/new/prefix/resource/BMA.png");break;
        case Stone::PAO:pix.load(":/new/prefix/resource/BPAO.png");break;
        case Stone::XIANG:pix.load(":/new/prefix/resource/BXIANG.png");;break;
        case Stone::SHI:pix.load(":/new/prefix/resource/BSHI.png");break;
        case Stone::BING:pix.load(":/new/prefix/resource/BBING.png");break;
        case Stone::JIANG:pix.load(":/new/prefix/resource/BJIANG.png");break;
        }
    }
    int dx=center(id).rx();
    int dy=center(id).ry();
    painter.drawPixmap(dx-_r*1.15, dy-_r*1.15, _r*2.3, _r*2.3, pix);//绘制图片 横坐标、纵坐标、宽度、高度
    QPixmap pixx;
    pixx.load(":/new/prefix/resource/select.png");
    if(id==_selectid){

        painter.drawPixmap(dx-_r*1.8, dy-_r*1.8, _r*3.6, _r*3.6, pixx);
    }
    if(_rowFrom!=-1&&_colFrom!=-1){
        int dxx=center(_rowFrom,_colFrom).rx();
        int dyy=center(_rowFrom,_colFrom).ry();
        painter.drawPixmap(dxx-_r*1.8, dyy-_r*1.8, _r*3.6, _r*3.6, pixx);
    }
    if(_rowTo!=-1&&_colTo!=-1){
        int dxxx=center(_rowTo,_colTo).rx();
        int dyyy=center(_rowTo,_colTo).ry();
        painter.drawPixmap(dxxx-_r*1.8, dyyy-_r*1.8, _r*3.6, _r*3.6, pixx);
    }

}



bool Board::red(int id)
{
    return _s[id]._red;
}

void Board::drawInitPosition(QPainter &p, int row, int col)
{
    QPoint pt = center(row, col);
    QPoint off = QPoint(_r/6, _r/6);
    int len = _r/3;

    QPoint ptStart;
    QPoint ptEnd;

    if(col != 0)
    {
        /* 左上角 */
        ptStart = QPoint(pt.x() - off.x(), pt.y() - off.y());
        ptEnd = ptStart + QPoint(-len, 0);
        p.drawLine(ptStart, ptEnd);
        ptEnd = ptStart + QPoint(0, -len);
        p.drawLine(ptStart, ptEnd);

        /* 左下角 */
        ptStart = QPoint(pt.x() - off.x(), pt.y() + off.y());
        ptEnd = ptStart + QPoint(-len, 0);
        p.drawLine(ptStart, ptEnd);
        ptEnd = ptStart + QPoint(0, +len);
        p.drawLine(ptStart, ptEnd);
    }

    if(col != 8)
    {
        p.setPen(QPen(Qt::black, _r/9, Qt::SolidLine));
        /* 右下角 */
        ptStart = QPoint(pt.x() + off.x(), pt.y() + off.y());
        ptEnd = ptStart + QPoint(+len, 0);
        p.drawLine(ptStart, ptEnd);
        ptEnd = ptStart + QPoint(0, +len);
        p.drawLine(ptStart, ptEnd);

        /* 右上角 */
        ptStart = QPoint(pt.x() + off.x(), pt.y() - off.y());
        ptEnd = ptStart + QPoint(+len, 0);
        p.drawLine(ptStart, ptEnd);
        ptEnd = ptStart + QPoint(0, -len);
        p.drawLine(ptStart, ptEnd);
    }
}

void Board::drawInitPosition(QPainter &p)
{
    drawInitPosition(p, 3, 0);
    drawInitPosition(p, 3, 2);
    drawInitPosition(p, 3, 4);
    drawInitPosition(p, 3, 6);
    drawInitPosition(p, 3, 8);

    drawInitPosition(p, 6, 0);
    drawInitPosition(p, 6, 2);
    drawInitPosition(p, 6, 4);
    drawInitPosition(p, 6, 6);
    drawInitPosition(p, 6, 8);

    drawInitPosition(p, 2, 1);
    drawInitPosition(p, 2, 7);

    drawInitPosition(p, 7, 1);
    drawInitPosition(p, 7, 7);
}

void Board::drawPlace(QPainter &p)
{
    p.drawLine(center(0, 3), center(2, 5));
    p.drawLine(center(2, 3), center(0, 5));

    p.drawLine(center(9, 3), center(7, 5));
    p.drawLine(center(7, 3), center(9, 5));
}

void Board::drawPlate(QPainter &p)
{
    for(int i=0; i<10; ++i)
    {
        if(i==0 || i==9)
        {
            p.setPen(QPen(Qt::black, _r/7, Qt::SolidLine));
        }
        else
        {
            p.setPen(QPen(Qt::black, _r/15, Qt::SolidLine));
        }
        p.drawLine(center(i, 0), center(i, 8));
    }

    for(int i=0; i<9; ++i)
    {
        if(i==0 || i==8)
        {
            p.setPen(QPen(Qt::black, _r/7, Qt::SolidLine));
            p.drawLine(center(0, i), center(9, i));
        }
        else
        {
            p.setPen(QPen(Qt::black, _r/15, Qt::SolidLine));
            p.drawLine(center(0, i), center(4, i));
            p.drawLine(center(5, i), center(9, i));
        }
    }
}

QPoint Board::center(int row, int col)
{
    QPoint pt(_r*col*2, _r*row*2);
    return pt + _off;
}

QPoint Board::center(int id)
{
    return center(_s[id]._row, _s[id]._col);
}


bool Board::getClickRowCol(QPoint pt, int &r, int &c)
{
    r=0,c=0;
    for(int i=1;i<=8;i++){
        if(pow(pt.x()-(i+1)*_r*2,2)<pow(pt.x()-(c+1)*_r*2,2))
            c=i;
    }
    for(int i=1;i<=9;i++){
        if(pow(pt.y()-(i+1)*_r*2,2)<pow(pt.y()-(r+1)*_r*2,2))
            r=i;
    }
    QPoint p = center(r,c);
    int dx = p.x() - pt.x();
    int dy = p.y() - pt.y();
    if(dx*dx+dy*dy<_r*_r)
        return true;
    return false;
}

void Board::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button() != Qt::LeftButton)
    {
        return;
    }

    click(ev->pos());
}

bool Board::canSelect(int id)
{
    return _bRedTurn == _s[id]._red;
}

void Board::trySelectStone(int id)
{
    if(id == -1)
        return;

    if(!canSelect(id)) return;

    _selectid = id;
    _rowFrom=-1;
    _colFrom=-1;
    update();
}

bool Board::sameColor(int id1, int id2)
{
    if(id1 == -1 || id2 == -1) return false;

    return red(id1) == red(id2);
}

bool Board::IsOut(int row, int col )
{
    if(row>0 && row < 9 ){
        if(col > 0 && col < 8)return false;
    }
    return true;
}

int Board::sameColorStone_EmptyGrid(int id1, int id2)
{
    if(red(id1)==red(id2))return 1;
    else if(id2==-1) return 2;
    else return 3;
}

void Board::tryMoveStone(int killid, int row, int col)
{
    if(killid != -1 && sameColor(killid, _selectid))
    {
        trySelectStone(killid);
        return;
    }

    bool ret = canMove(_selectid, killid, row, col);
    if(!ret) return;
    //qDebug()<<"ret:"<<ret<<endl;
    if(ret)
    {
        //qDebug()<<"inret:"<<ret<<endl;
        moveStone(_selectid, killid, row, col);
        _selectid = -1;
        update();
    }
}

int Board::getStoneCountAtLine(int row1, int col1, int row2, int col2)
{
    int ret = 0;
    if(row1 != row2 && col1 != col2)
        return -1;
    if(row1 == row2 && col1 == col2)
        return -1;

    if(row1 == row2)
    {
        int min = col1 < col2 ? col1 : col2;
        int max = col1 < col2 ? col2 : col1;
        for(int col = min+1; col<max; ++col)
        {
            if(getStoneId(row1, col) != -1) ++ret;
        }
    }
    else
    {
        int min = row1 < row2 ? row1 : row2;
        int max = row1 < row2 ? row2 : row1;
        for(int row = min+1; row<max; ++row)
        {
            if(getStoneId(row, col1) != -1) ++ret;
        }
    }

    return ret;
}

int Board::relation(int row1, int col1, int row, int col)
{
    return qAbs(row1-row)*10+qAbs(col1-col);
}

bool Board::canMoveChe(int moveid, int, int row, int col)
{
    GetRowCol(row1, col1, moveid);
    int ret = getStoneCountAtLine(row1, col1, row, col);
    if(ret == 0)
        return true;
    return false;
}

bool Board::canMoveMa(int moveid, int, int row, int col)
{
    GetRowCol(row1, col1, moveid);
    int r = relation(row1, col1, row, col);
    if(r != 12 && r != 21)
        return false;

    if(r == 12)
    {
        if(getStoneId(row1, (col+col1)/2) != -1)
            return false;
    }
    else
    {
        if(getStoneId((row+row1)/2, col1) != -1)
            return false;
    }

    return true;
}

bool Board::canMovePao(int moveid, int killid, int row, int col)
{
    GetRowCol(row1, col1, moveid);
    int ret = getStoneCountAtLine(row, col, row1, col1);
    if(killid != -1)
    {
        if(ret == 1) return true;
    }
    else
    {
        if(ret == 0) return true;
    }
    return false;
}

bool Board::canMoveBing(int moveid, int, int row, int col)
{
    GetRowCol(row1, col1, moveid);
    int r = relation(row1, col1, row, col);
    if(r != 1 && r != 10) return false;

    if(isBottomSide(moveid))
    {
        if(row > row1) return false;
        if(row1 >= 5 && row == row1) return false;
    }
    else
    {
        if(row < row1) return false;
        if(row1 <= 4 && row == row1) return false;
    }

    return true;
}

bool Board::canMoveJiang(int moveid, int killid, int row, int col)
{
    if(killid != -1 && _s[killid]._type == Stone::JIANG)
        return canMoveChe(moveid, killid, row, col);

    GetRowCol(row1, col1, moveid);
    int r = relation(row1, col1, row, col);
    if(r != 1 && r != 10) return false;

    if(col < 3 || col > 5) return false;
    if(isBottomSide(moveid))
    {
        if(row < 7) return false;
    }
    else
    {
        if(row > 2) return false;
    }
    return true;
}

bool Board::canMoveShi(int moveid, int, int row, int col)
{
    GetRowCol(row1, col1, moveid);
    int r = relation(row1, col1, row, col);
    if(r != 11) return false;

    if(col < 3 || col > 5) return false;
    if(isBottomSide(moveid))
    {
        if(row < 7) return false;
    }
    else
    {
        if(row > 2) return false;
    }
    return true;
}

bool Board::canMoveXiang(int moveid, int, int row, int col)
{
    GetRowCol(row1, col1, moveid);
    int r = relation(row1, col1, row, col);
    if(r != 22) return false;

    int rEye = (row+row1)/2;
    int cEye = (col+col1)/2;
    if(getStoneId(rEye, cEye) != -1)
        return false;

    if(isBottomSide(moveid))                    //不能走回去？
    {
        if(row < 4) return false;
    }
    else
    {
        if(row > 5) return false;
    }
    return true;
}

bool Board::canMove(int moveid, int killid, int row, int col)
{
    //return false;
    //qDebug()<<"cantmove"<<endl;
    if(sameColor(moveid, killid)) return false;
    GetRowCol(row1, col1, moveid);
    Step step;
    step._colFrom = col1;
    step._colTo = col;
    step._rowFrom = row1;
    step._rowTo = row;
    step._moveid = moveid;
    step._killid = killid;
    if(iscircular(step)==true){
        //qDebug()<<"cantmove"<<endl;
        return false;
    }

    switch (_s[moveid]._type)
    {
    case Stone::CHE:
        return canMoveChe(moveid, killid, row, col);

    case Stone::MA:
        return canMoveMa(moveid, killid, row, col);

    case Stone::PAO:
        return canMovePao(moveid, killid, row, col);

    case Stone::BING:
        return canMoveBing(moveid, killid, row, col);

    case Stone::JIANG:
        return canMoveJiang(moveid, killid, row, col);

    case Stone::SHI:
        return canMoveShi(moveid, killid, row, col);

    case Stone::XIANG:
        return canMoveXiang(moveid, killid, row, col);

    }
    //qDebug()<<"cantmove"<<endl;
    return false;
}

void Board::reliveStone(int id)
{
    if(id==-1) return;
    _s[id]._dead = false;
}

void Board::killStone(int id)
{
    if(id==-1) return;
    _s[id]._dead = true;
}

bool Board::isBottomSide(int id)
{
    return _bSide == _s[id]._red;
}

void Board::moveStone(int moveid, int row, int col)
{
    _s[moveid]._row = row;
    _s[moveid]._col = col;

    _bRedTurn = !_bRedTurn;
}

void Board::saveStep(int moveid, int killid, int row, int col, QVector<Step*>& steps)
{
    GetRowCol(row1, col1, moveid);
    Step* step = new Step;
    step->_colFrom = col1;
    step->_colTo = col;
    step->_rowFrom = row1;
    step->_rowTo = row;
    step->_moveid = moveid;
    step->_killid = killid;

    steps.append(step);

}

void Board::moveStone(int moveid, int killid, int row, int col)
{
    //qDebug()<<1<<endl;
    saveStep(moveid, killid, row, col, _steps);
    GetRowCol(row1, col1, moveid);
    Step step;
    step._colFrom = col1;
    step._colTo = col;
    step._rowFrom = row1;
    step._rowTo = row;
    step._moveid = moveid;
    step._killid = killid;
    //qDebug()<<step._rowFrom<<" "<<step._colFrom<<" "<<step._rowTo<<" "<<step._colTo<<endl;
    pushmove(step);
    /*for(int i=0;i<8;i++){
        qDebug()<<"Step"<<i<<":"<<endl;
        qDebug()<<_circular[i]._rowFrom<<" "<<_circular[i]._colFrom<<endl;
        qDebug()<<_circular[i]._rowTo<<" "<<_circular[i]._colTo<<endl;
    }*/
    //qdebug()<<1<<endl;
    //qDebug()<<moveid<<" "<<_s[moveid]._row<<" "<<_s[moveid]._col<<" "<<row<<" "<<col;
    killStone(killid);
    _rowFrom=_s[moveid]._row;
    _colFrom=_s[moveid]._col;
    _rowTo=row;
    _colTo=col;
    moveStone(moveid, row, col);
    if(killid == 4){
        QMessageBox::information(this, "Game Over", "黑棋获胜！");
        if(QMessageBox::question(this,"象棋", "是否继续游戏", QString("重玩"), QString("退出")) == 0)
        {
            this->init(true);
            _steps.clear();
            _rowFrom = -1 ;
            _rowTo =-1;
            _colFrom = -1;
            _colTo = -1;
            update();

        }
        else exit(0);
    }
    else if(killid == 20){
        QMessageBox::information(this, "Game Over", "红棋获胜！");
        if(QMessageBox::question(this,"象棋", "是否继续游戏", QString("重玩"), QString("退出")) == 0)
        {
            this->init(true);
            _steps.clear();
            _rowFrom = -1 ;
            _rowTo =-1;
            _colFrom = -1;
            _colTo = -1;
            update();

        }
        else exit(0);
    }

}

void Board::click(int id, int row, int col)
{
    if(this->_selectid == -1)
    {
        trySelectStone(id);
    }
    else
    {
        tryMoveStone(id, row, col);
    }
}

void Board::click(QPoint pt)
{
    int row, col;
    bool bClicked = getClickRowCol(pt, row, col);
    if(!bClicked) return;

    int id = getStoneId(row, col);
    click(id, row, col);
}

int Board::getStoneId(int row, int col)
{
    for(int i=0; i<32; ++i)
    {
        if(_s[i]._row == row && _s[i]._col == col && !isDead(i))
            return i;
    }
    return -1;
}

void Board::back(Step *step)
{
    reliveStone(step->_killid);
    moveStone(step->_moveid, step->_rowFrom, step->_colFrom);
}

void Board::backOne()
{
    if(this->_steps.size() == 0) return;

    Step* step = this->_steps.last();
    _steps.removeLast();
    back(step);
    _rowFrom =-1 ;
    _rowTo = -1;
    _colFrom =-1;
    _colTo =-1;
    update();
    delete step;
}

void Board::back()
{
    backOne();
}

void Board::slotBack()
{
    back();
}

void Board::pushmove(Step& step){
    for(int i=0;i<7;i++){
        _circular[i]=_circular[i+1];
    }
    _circular[7]=step;
}

bool Board::iscircular(Step &step){
    if(_circular[0]==_circular[4]&&_circular[4]==step&&_circular[1]==_circular[5]&&_circular[2]==_circular[6]&&_circular[3]==_circular[7])
    {//qDebug()<<"false"<<endl;
        return true;}
    return false;
}







