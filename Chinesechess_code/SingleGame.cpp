#include "SingleGame.h"
#include <QTimer>
#include <QDebug>
#include<QPushButton>
#include<QHBoxLayout>

SingleGame::SingleGame(int l , QWidget *parent ) : Board(parent)
{
    _level = l;
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addStretch();
    layout->addWidget(button1);
    layout->addStretch();
//    QSpacerItem* h1= new QSpacerItem(1,1,QSizePolicy::Expanding, QSizePolicy::Minimum );
//    layout->addSpacerItem( h1 );
    layout->addWidget(button2);
    layout->addStretch();

    Wid->setParent(this);


    //Wid->setFixedSize(300,20);

    QHBoxLayout *mainlayout = new QHBoxLayout();
    mainlayout->addStretch(7);
    mainlayout->addLayout(layout);
    mainlayout->addStretch(3);
    Wid->setLayout(mainlayout);
    Wid->show();
    /*for(int i=0;i<7;i++)
        for(int j=0;j<10;j++)
            for(int k=0;k<9;k++)
                xyscore[i][j][k]=0;*/

}

SingleGame::~SingleGame()
{

}

void SingleGame::back()
{
    if(_bRedTurn)
    {
        backOne();
        backOne();
    }
}

void SingleGame::click(int id, int row, int col)
{
    if(_bRedTurn)
    {
        Board::click(id, row, col);
        if(!_bRedTurn)
        {
            QTimer::singleShot(100, this, SLOT(computerMove()));
        }
    }
}

void SingleGame::computerMove()
{
    Step* step = MaxMove();
    moveStone(step->_moveid, step->_killid, step->_rowTo, step->_colTo);
    delete step;
    update();
}

Step* SingleGame::MaxMove()
{
    Step* ret = NULL;
    QVector<Step*> steps;
    getAllPossibleMove(steps);
    //qDebug()<<"steps:"<<steps.count()<<endl;
    int maxInAllMinScore = -3000000;

    int now=score();
    while(steps.count())
    {
        Step* step = steps.last();
        steps.removeLast();

        fakeMove(step);
        int nowscore=now;
        nowscore+=Bchange(step);
        int minScore = Minmove(this->_level-1, maxInAllMinScore,nowscore);
        unfakeMove(step);

        if(minScore > maxInAllMinScore)
        {
            if(ret) delete ret;

            ret = step;
            maxInAllMinScore = minScore;
        }
        else
        {
            delete step;
        }
    }
    return ret;
}

int SingleGame::score()
{
  //  enum TYPE{CHE, MA, PAO, BING, JIANG, SHI, XIANG};
    int scoreBlack = 0;
    int scoreRed = 0;
    for(int i=0; i<16; ++i)
    {
        if(_s[i]._dead) continue;
        //scoreRed += s[_s[i]._type];
        scoreRed += xyscore[_s[i]._type][_s[i]._row][_s[i]._col];
        //scoreRed += ss[_s[i]._type];
    }
    for(int i=16; i<32; ++i)
    {
        if(_s[i]._dead) continue;
        //scoreBlack += s[_s[i]._type];
        scoreBlack += xyscore[_s[i]._type][9-_s[i]._row][8-_s[i]._col];
        //scoreBlack += ss[_s[i]._type];
    }
    //int RJIANGDEAD=0,BJIANGDEAD=0;
    //if(_s[4]._dead) RJIANGDEAD=300000;
    //if(_s[20]._dead) BJIANGDEAD=300000;
    return scoreBlack - scoreRed;
}

int SingleGame::Minmove(int level, int curMin,int now)
{
    //qDebug() <<level<<endl;
    if(level == 0)
        return now;

    QVector<Step*> steps;
    getAllPossibleMove(steps);
    //qDebug()<<"steps:"<<steps.count()<<endl;
    int minInAllMaxScore = 3000000;

    while(steps.count())
    {
        Step* step = steps.last();
        steps.removeLast();

        fakeMove(step);
        int nowscore=now;
        nowscore+=Rchange(step);
        int maxScore = Maxmove(level-1, minInAllMaxScore,nowscore);
        unfakeMove(step);
        delete step;

        if(maxScore <= curMin)
        {
            while(steps.count())
            {
                Step* step = steps.last();
                steps.removeLast();
                delete step;
            }
            return maxScore;
        }

        if(maxScore < minInAllMaxScore)
        {
            minInAllMaxScore = maxScore;
        }


    }
    return minInAllMaxScore;
}

int SingleGame::Rchange(Step *step){
    int change=0;
    change+=xyscore[_s[step->_moveid]._type][step->_rowFrom][step->_colFrom];
    change-=xyscore[_s[step->_moveid]._type][step->_rowTo][step->_colTo];
    if(step->_killid!=-1){
        change-=xyscore[_s[step->_killid-16]._type][9-_s[step->_killid]._row][8-_s[step->_killid]._col];
        //change-=ss[_s[step->_killid-16]._type];
        if(step->_killid==20) change-=300000;
    }
    return change;
}

int SingleGame::Bchange(Step *step){
    int change=0;
    change-=xyscore[_s[step->_moveid]._type][9-step->_rowFrom][8-step->_colFrom];
    change+=xyscore[_s[step->_moveid]._type][9-step->_rowTo][8-step->_colTo];
    if(step->_killid!=-1){
        change+=xyscore[_s[step->_killid]._type][step->_rowTo][step->_colTo];
        //change+=ss[_s[step->_killid]._type];
        if(step->_killid==4) change+=200000;
    }
    return change;
}
int SingleGame::Maxmove(int level, int curMax,int now)
{
    //qDebug() <<level <<endl;
    if(level == 0)
        return now;

    QVector<Step*> steps;
    getAllPossibleMove(steps);
    if(steps.count()==0) return now;
    //qDebug()<<"steps:"<<steps.count()<<endl;
    int maxInAllMinScore = -3000000;

    while(steps.count())
    {
        Step* step = steps.last();
        steps.removeLast();

        fakeMove(step);
        int nowscore=now;
        nowscore+=Bchange(step);
        int minScore = Minmove(level-1, maxInAllMinScore,nowscore);
        unfakeMove(step);
        delete step;

        if(minScore >= curMax)
        {
            while(steps.count())
            {
                Step* step = steps.last();
                steps.removeLast();
                delete step;
            }
            return minScore;
        }
        if(minScore > maxInAllMinScore)
        {
            maxInAllMinScore = minScore;
        }


    }
    return maxInAllMinScore;
}

void SingleGame::fakeMove(Step *step)
{
    killStone(step->_killid);
    moveStone(step->_moveid, step->_rowTo, step->_colTo);
}

void SingleGame::unfakeMove(Step *step)
{
    reliveStone(step->_killid);
    moveStone(step->_moveid, step->_rowFrom, step->_colFrom);
}
/*void SingleGame:: getCHEALLPossibleMove(int id, QVector<Step*>& steps)
{
        for(int col=_s[id]._col+1; col <= 8; ++col)
        {
            int killid=this->getStoneId(_s[id]._row,col);
            if(sameColorStone_EmptyGrid(id,killid)==1)break;
            else if(sameColorStone_EmptyGrid(id,killid)==2)saveStep(id, killid, _s[id]._row, col, steps);
            else {   saveStep(id, killid, _s[id]._row, col, steps);
                break;}
        }
        for(int col=_s[id]._col-1; col >= 0; --col)
        {
            int killid=this->getStoneId(_s[id]._row,col);
            if(sameColorStone_EmptyGrid(id,killid)==1)break;
            else if(sameColorStone_EmptyGrid(id,killid)==2)saveStep(id, killid, _s[id]._row, col, steps);
            else {   saveStep(id, killid, _s[id]._row, col, steps); break;}
        }
        for(int row = _s[id]._row+1; row <= 9; row++)
        {
            int killid=this->getStoneId(row,_s[id]._col);
            if(sameColorStone_EmptyGrid(id,killid)==1)break;
            else if(sameColorStone_EmptyGrid(id,killid)==2)saveStep(id, killid, row, _s[id]._col, steps);
            else {   saveStep(id, killid, row, _s[id]._col, steps);
                break;}
        }
        for(int row = _s[id]._row-1; row >= 0; row++)
        {
            int killid=this->getStoneId(row,_s[id]._col);
            if(sameColorStone_EmptyGrid(id,killid)==1)break;
            else if(sameColorStone_EmptyGrid(id,killid)==2)saveStep(id, killid, row, _s[id]._col, steps);
            else {   saveStep(id, killid, row, _s[id]._col, steps);
                break;}
        }
}*/
void SingleGame::getCHEALLPossibleMove(int id, QVector<Step *> &steps){
    int idrow=_s[id]._row;
    int idcol=_s[id]._col;
    for(int i=0;i<=9;i++){
        int killid = this->getStoneId(i, idcol);
        if(sameColor(id, killid)) continue;

        if(canMove(id, killid, i, idcol))
        {
            saveStep(id, killid, i, idcol, steps);
        }
    }
    for(int i=0;i<=8;i++){
        int killid = this->getStoneId(idrow, i);
        if(sameColor(id, killid)) continue;

        if(canMove(id, killid, idrow, i))
        {
            saveStep(id, killid, idrow, i, steps);
        }
    }
}
/*void SingleGame::getMAALLPossibleMove(int id, QVector<Step*>& steps)
{
    int killid;
    if(!IsOut(_s[id]._row+2, _s[id]._col+1) ){
    killid=this->getStoneId(_s[id]._row+2, _s[id]._col+1);
    if(!sameColor(id,killid) && canMoveMa(id, killid, _s[id]._row+2 ,_s[id]._col+1))saveStep(id, killid, _s[id]._row+2 ,_s[id]._col+1,steps);
}
    if(!IsOut(_s[id]._row+2, _s[id]._col-1) ){
    killid=this->getStoneId(_s[id]._row+2, _s[id]._col-1);
    if(!sameColor(id,killid) && canMoveMa(id, killid, _s[id]._row+2 ,_s[id]._col-1))saveStep(id, killid, _s[id]._row+2 ,_s[id]._col-1,steps);
}
    if(!IsOut(_s[id]._row+1, _s[id]._col+2) ){
    killid=this->getStoneId(_s[id]._row+1, _s[id]._col+2);
    if(!sameColor(id,killid) && canMoveMa(id, killid, _s[id]._row+1 ,_s[id]._col+2))saveStep(id, killid, _s[id]._row+1 ,_s[id]._col+2,steps);
}
    if(!IsOut(_s[id]._row+1, _s[id]._col-2) ){
    killid=this->getStoneId(_s[id]._row+1, _s[id]._col-2);
    if(!sameColor(id,killid) && canMoveMa(id, killid, _s[id]._row+1 ,_s[id]._col-2))saveStep(id, killid, _s[id]._row+1 ,_s[id]._col-2,steps);
}
    if(!IsOut(_s[id]._row-1, _s[id]._col+2) ){
    killid=this->getStoneId(_s[id]._row-1, _s[id]._col+2);
    if(!sameColor(id,killid) && canMoveMa(id, killid, _s[id]._row-1 ,_s[id]._col+2))saveStep(id, killid, _s[id]._row-1 ,_s[id]._col+2,steps);
}
    if(!IsOut(_s[id]._row-1, _s[id]._col-2) ){
    killid=this->getStoneId(_s[id]._row-1, _s[id]._col-2);
    if(!sameColor(id,killid) && canMoveMa(id, killid, _s[id]._row-1 ,_s[id]._col-2))saveStep(id, killid, _s[id]._row-1 ,_s[id]._col-2,steps);
}
    if(!IsOut(_s[id]._row-2, _s[id]._col+1) ){
    killid=this->getStoneId(_s[id]._row-2, _s[id]._col+1);
    if(!sameColor(id,killid) && canMoveMa(id, killid, _s[id]._row-2 ,_s[id]._col+1))saveStep(id, killid, _s[id]._row-2 ,_s[id]._col+1,steps);
}
    if(!IsOut(_s[id]._row-2, _s[id]._col-1) ){
    killid=this->getStoneId(_s[id]._row-2, _s[id]._col-1);
    if(!sameColor(id,killid) && canMoveMa(id, killid, _s[id]._row-2 ,_s[id]._col-1))saveStep(id, killid, _s[id]._row-2 ,_s[id]._col-1,steps);
}
}*/

void SingleGame::getMAALLPossibleMove(int id, QVector<Step *> &steps){
    int idrow=_s[id]._row;
    int idcol=_s[id]._col;

    for(int i=0;i<8;i++){
        int gorow=idrow+MAgo[i][0];
        int gocol=idcol+MAgo[i][1];
        if(gorow>9||gorow<0||gocol<0||gocol>8) continue;
        int killid = this->getStoneId(gorow, gocol);
        if(sameColor(id, killid)) continue;

        if(canMove(id, killid, gorow, gocol))
        {
            saveStep(id, killid, gorow, gocol, steps);
        }
    }
}
/*void SingleGame::getPAOALLPossibleMove(int id, QVector<Step*>& steps)
{
    int killid;
    for(int col=_s[id]._col+1; col <= 8; ++col)
    {
        killid=this->getStoneId(_s[id]._row,col);
        if(canMove(id,killid,_s[id]._row,col))saveStep(id, killid, _s[id]._row, col, steps);
    }
    for(int col=_s[id]._col-1; col >= 0; --col)
    {
        killid=this->getStoneId(_s[id]._row,col);
        if(canMove(id,killid,_s[id]._row,col))saveStep(id, killid, _s[id]._row, col, steps);
    }
    for(int row=_s[id]._row+1; row <= 9; ++row)
    {
        killid=this->getStoneId(row,_s[id]._col);
        if(canMove(id,killid,row,_s[id]._col))saveStep(id, killid, row, _s[id]._col, steps);
    }
    for(int row=_s[id]._row-1; row >= 0; --row)
    {
        killid=this->getStoneId(row,_s[id]._col);
        if(canMove(id,killid,row,_s[id]._col))saveStep(id, killid, row, _s[id]._col, steps);
    }
}*/

void SingleGame::getPAOALLPossibleMove(int id, QVector<Step *> &steps){
    int idrow=_s[id]._row;
    int idcol=_s[id]._col;
    for(int i=0;i<=9;i++){
        int killid = this->getStoneId(i, idcol);
        if(sameColor(id, killid)) continue;

        if(canMove(id, killid, i, idcol))
        {
            saveStep(id, killid, i, idcol, steps);
        }
    }
    for(int i=0;i<=8;i++){
        int killid = this->getStoneId(idrow, i);
        if(sameColor(id, killid)) continue;

        if(canMove(id, killid, idrow, i))
        {
            saveStep(id, killid, idrow, i, steps);
        }
    }
}

#if 0
void SingleGame::getSHIALLPossibleMove(int id, QVector<Step*>& steps)
{
    int killid;
    if(!IsOut(_s[id]._row+1, _s[id]._col+1)){
    killid=this->getStoneId(_s[id]._row+1, _s[id]._col+1);
    if(!sameColor(id,killid) && canMoveShi(id, killid, _s[id]._row+1 ,_s[id]._col+1))saveStep(id, killid, _s[id]._row+1 ,_s[id]._col+1,steps);
    }
    if(!IsOut(_s[id]._row+1, _s[id]._col-1)){
    killid=this->getStoneId(_s[id]._row+1, _s[id]._col-1);
    if(!sameColor(id,killid) && canMoveShi(id, killid, _s[id]._row+1 ,_s[id]._col-1))saveStep(id, killid, _s[id]._row+1 ,_s[id]._col-1,steps);
    }
    if(!IsOut(_s[id]._row-1, _s[id]._col+1)){
    killid=this->getStoneId(_s[id]._row-1, _s[id]._col+1);
    if(!sameColor(id,killid) && canMoveShi(id, killid, _s[id]._row-1 ,_s[id]._col+1))saveStep(id, killid, _s[id]._row-1 ,_s[id]._col+1,steps);
    }
    if(!IsOut(_s[id]._row-1, _s[id]._col-1)){
    killid=this->getStoneId(_s[id]._row-1, _s[id]._col-1);
    if(!sameColor(id,killid) && canMoveShi(id, killid, _s[id]._row-1 ,_s[id]._col-1))saveStep(id, killid, _s[id]._row-1 ,_s[id]._col-1,steps);
    }
}
#endif
void SingleGame::getSHIALLPossibleMove(int id, QVector<Step *> &steps){
    int killid;
    if(!_s[id]._red){
#if 0
        for(int i=0;i<=2;i++)
            for(int j=3;j<=5;j++){
                int killid = this->getStoneId(i, j);
                if(sameColor(id, killid)) continue;

                if(canMove(id, killid, i, j))
                {
                    saveStep(id, killid, i, j, steps);
                }
            }
#endif
        if(_s[id]._row!=1){
            killid=this->getStoneId(1, 4);
            if(canMove(id,killid,1,4)) saveStep(id,killid,1,4,steps);
        }
        else{
            killid=this->getStoneId(0, 3);
            if(canMove(id,killid,0,3)) saveStep(id,killid,0,3,steps);
            killid=this->getStoneId(0, 5);
            if(canMove(id,killid,0,5)) saveStep(id,killid,0,5,steps);
            killid=this->getStoneId(2, 3);
            if(canMove(id,killid,2,3)) saveStep(id,killid,2,3,steps);
            killid=this->getStoneId(2, 5);
            if(canMove(id,killid,2,5)) saveStep(id,killid,2,5,steps);
        }
    }
    else{
#if 0
        for(int i=7;i<=9;i++)
            for(int j=3;j<=5;j++){
                int killid = this->getStoneId(i, j);
                if(sameColor(id, killid)) continue;

                if(canMove(id, killid, i, j))
                {
                    saveStep(id, killid, i, j, steps);
                }
            }
#endif
        if(_s[id]._row!=8){
            killid=this->getStoneId(8, 4);
            if(canMove(id,killid,8,4)) saveStep(id,killid,8,4,steps);
        }
        else{
            killid=this->getStoneId(9, 3);
            if(canMove(id,killid,9,3)) saveStep(id,killid,9,3,steps);
            killid=this->getStoneId(9, 5);
            if(canMove(id,killid,9,5)) saveStep(id,killid,9,5,steps);
            killid=this->getStoneId(7, 3);
            if(canMove(id,killid,7,3)) saveStep(id,killid,7,3,steps);
            killid=this->getStoneId(7, 5);
            if(canMove(id,killid,7,5)) saveStep(id,killid,7,5,steps);
        }
    }
}
#if 0
void SingleGame::getBINGALLPossibleMove(int id, QVector<Step*>& steps)
{
    int killid;
    if(!IsOut(_s[id]._row+1, _s[id]._col)){
    killid=this->getStoneId(_s[id]._row+1, _s[id]._col);
    if(!sameColor(id,killid) && canMoveBing(id, killid, _s[id]._row+1 ,_s[id]._col))saveStep(id, killid, _s[id]._row+1 ,_s[id]._col,steps);
    }
    if(!IsOut(_s[id]._row, _s[id]._col+1)){
    killid=this->getStoneId(_s[id]._row, _s[id]._col+1);
    if(!sameColor(id,killid) && canMoveBing(id, killid, _s[id]._row ,_s[id]._col+1))saveStep(id, killid, _s[id]._row ,_s[id]._col+1,steps);
    }
    if(!IsOut(_s[id]._row, _s[id]._col-1)){
    killid=this->getStoneId(_s[id]._row, _s[id]._col-1);
    if(!sameColor(id,killid) && canMoveBing(id, killid, _s[id]._row ,_s[id]._col-1))saveStep(id, killid, _s[id]._row ,_s[id]._col-1,steps);
    }
}
#endif
void SingleGame::getBINGALLPossibleMove(int id, QVector<Step *> &steps){
    int idrow=_s[id]._row;
    int idcol=_s[id]._col;
    int gorow,gocol,killid;
for(int i=0;i<4;i++){
    gorow=idrow+JIANGgo[i][0];
    gocol=idcol+JIANGgo[i][1];
    if(gorow>9||gorow<0||gocol<0||gocol>8) continue;
    killid = this->getStoneId(gorow, gocol);
    if(sameColor(id, killid)) continue;

    if(canMove(id, killid, gorow, gocol))
    {
        saveStep(id, killid, gorow, gocol, steps);
    }
}
    /*if(!_bRedTurn){
        if(idrow<=5){
            int gorow=idrow+BBINGgo[2][0];
            int gocol=idcol+BBINGgo[2][1];
            if(gorow>9||gorow<0||gocol<0||gocol>8) return;
            int killid = this->getStoneId(gorow, gocol);
            if(sameColor(id, killid)) return;

            if(canMove(id, killid, gorow, gocol))
            {
                saveStep(id, killid, gorow, gocol, steps);
        }
        else{
                int gorow,gocol,killid;
            for(int i=0;i<3;i++){
                gorow=idrow+BBINGgo[i][0];
                gocol=idcol+BBINGgo[i][1];
                if(gorow>9||gorow<0||gocol<0||gocol>8) continue;
                killid = this->getStoneId(gorow, gocol);
                if(sameColor(id, killid)) continue;

                if(canMove(id, killid, gorow, gocol))
                {
                    saveStep(id, killid, gorow, gocol, steps);
                }
            }
        }
    }
    }
    else{
        if(idrow>5){
            int gorow=idrow+RBINGgo[2][0];
            int gocol=idcol+RBINGgo[2][1];
            if(gorow>9||gorow<0||gocol<0||gocol>8) return;
            int killid = this->getStoneId(gorow, gocol);
            if(sameColor(id, killid)) return;

            if(canMove(id, killid, gorow, gocol))
            {
                saveStep(id, killid, gorow, gocol, steps);
        }
        else{
                int gorow,gocol,killid;
            for(int i=0;i<3;i++){
                gorow=idrow+RBINGgo[i][0];
                gocol=idcol+RBINGgo[i][1];
                if(gorow>9||gorow<0||gocol<0||gocol>8) continue;
                killid = this->getStoneId(gorow, gocol);
                if(sameColor(id, killid)) continue;

                if(canMove(id, killid, gorow, gocol))
                {
                    saveStep(id, killid, gorow, gocol, steps);
                }
            }
        }
    }
    }*/
}
#if 0
void SingleGame::getJIANGALLPossibleMove(int id, QVector<Step*>& steps)
{
    int killid;
    if(!IsOut(_s[id]._row+1, _s[id]._col)){
    killid=this->getStoneId(_s[id]._row+1, _s[id]._col);
    if(!sameColor(id,killid) && canMoveJiang(id, killid, _s[id]._row+1 ,_s[id]._col))saveStep(id, killid, _s[id]._row+1 ,_s[id]._col,steps);
    }
    if(!IsOut(_s[id]._row-1, _s[id]._col)){
    killid=this->getStoneId(_s[id]._row-1, _s[id]._col);
    if(!sameColor(id,killid) && canMoveJiang(id, killid, _s[id]._row-1 ,_s[id]._col))saveStep(id, killid, _s[id]._row-1 ,_s[id]._col,steps);
    }
    if(!IsOut(_s[id]._row, _s[id]._col+1)){
    killid=this->getStoneId(_s[id]._row, _s[id]._col+1);
    if(!sameColor(id,killid) && canMoveJiang(id, killid, _s[id]._row ,_s[id]._col+1))saveStep(id, killid, _s[id]._row ,_s[id]._col+1,steps);
    }
    if(!IsOut(_s[id]._row, _s[id]._col-1)){
    killid=this->getStoneId(_s[id]._row, _s[id]._col-1);
    if(!sameColor(id,killid) && canMoveJiang(id, killid, _s[id]._row ,_s[id]._col-1))saveStep(id, killid, _s[id]._row ,_s[id]._col-1,steps);
    }
}
#endif
void SingleGame::getJIANGALLPossibleMove(int id, QVector<Step *> &steps){
    int idrow=_s[id]._row;
    int idcol=_s[id]._col;

    if(!_s[id]._red){
        int gorow,gocol,killid;
        for(int i=0;i<4;i++){
            gorow=idrow+JIANGgo[i][0];
            gocol=idcol+JIANGgo[i][1];
            if(gorow>2||gorow<0||gocol<3||gocol>5) continue;
            killid = this->getStoneId(gorow, gocol);
            if(sameColor(id, killid)) continue;

            if(canMove(id, killid, gorow, gocol))
            {
                saveStep(id, killid, gorow, gocol, steps);
            }
        }
        if(canMove(id,4,_s[4]._row,_s[4]._col)){
                saveStep(id,4,_s[4]._row,_s[4]._col,steps);
        }
    }
    else{
        int gorow,gocol,killid;
        for(int i=0;i<4;i++){
            gorow=idrow+JIANGgo[i][0];
            gocol=idcol+JIANGgo[i][1];
            if(gorow>9||gorow<7||gocol<3||gocol>5) continue;
            killid = this->getStoneId(gorow, gocol);
            if(sameColor(id, killid)) continue;

            if(canMove(id, killid, gorow, gocol))
            {
                saveStep(id, killid, gorow, gocol, steps);
            }
        }
            if(canMove(id,20,_s[20]._row,_s[20]._col)){
                    saveStep(id,20,_s[20]._row,_s[20]._col,steps);
            }
    }
}
#if 0
void SingleGame::getXIANGALLPossibleMove(int id, QVector<Step*>& steps)
{
    int killid;
    if(!IsOut(_s[id]._row+2, _s[id]._col+2)){
    killid=this->getStoneId(_s[id]._row+2, _s[id]._col+2);
    if(canMoveJiang(id, killid, _s[id]._row+2 ,_s[id]._col+2))saveStep(id, killid, _s[id]._row+2 ,_s[id]._col+2,steps);
    }
    if(!IsOut(_s[id]._row+2, _s[id]._col-2)){
    killid=this->getStoneId(_s[id]._row+2, _s[id]._col-2);
    if(canMoveJiang(id, killid, _s[id]._row+2 ,_s[id]._col-2))saveStep(id, killid, _s[id]._row+2 ,_s[id]._col-2,steps);
    }
    if(!IsOut(_s[id]._row-2, _s[id]._col-2)){
    killid=this->getStoneId(_s[id]._row-2, _s[id]._col-2);
    if(canMoveJiang(id, killid, _s[id]._row-2 ,_s[id]._col-2))saveStep(id, killid, _s[id]._row-2 ,_s[id]._col-2,steps);
    }
    if(!IsOut(_s[id]._row-2, _s[id]._col+2)){
    killid=this->getStoneId(_s[id]._row-2, _s[id]._col+2);
    if(canMoveJiang(id, killid, _s[id]._row-2 ,_s[id]._col+2))saveStep(id, killid, _s[id]._row-2 ,_s[id]._col+2,steps);
    }
}
#endif
void SingleGame::getXIANGALLPossibleMove(int id, QVector<Step *> &steps){
    if(!_s[id]._red){

        for(int i=0;i<7;i++){
            int killid = this->getStoneId(BXIANGgo[i][0], BXIANGgo[i][1]);
            if(sameColor(id, killid)) continue;

            if(canMove(id, killid, BXIANGgo[i][0], BXIANGgo[i][1]))
            {
                saveStep(id, killid, BXIANGgo[i][0], BXIANGgo[i][1], steps);
            }
        }
    }
    else{

        for(int i=0;i<7;i++){
            int killid = this->getStoneId(RXIANGgo[i][0], RXIANGgo[i][1]);
            if(sameColor(id, killid)) continue;

            if(canMove(id, killid, RXIANGgo[i][0], RXIANGgo[i][1]))
            {
                saveStep(id, killid, RXIANGgo[i][0], RXIANGgo[i][1], steps);
            }
        }
    }
}

void SingleGame::getAllPossibleMove(QVector<Step *> &steps)
{
    int min, max;
    if(this->_bRedTurn)
    {
        min = 0, max = 16;
    }
    else
    {
        min = 16, max = 32;
    }
    for(int i=min;i<max; i++)
    {
        if(this->_s[i]._dead) continue;
        else switch (_s[i]._type){
        case Stone::CHE:getCHEALLPossibleMove(i, steps);break;
        case Stone::MA:getMAALLPossibleMove(i, steps);break;
        case Stone::XIANG:getXIANGALLPossibleMove(i, steps);break;
        case Stone::SHI:getSHIALLPossibleMove(i, steps);break;
        case Stone::PAO:getPAOALLPossibleMove(i, steps);break;
        case Stone::JIANG:getJIANGALLPossibleMove(i, steps);break;
        case Stone::BING:getBINGALLPossibleMove(i, steps);break;
        }
#if 0
        for(int row = 0; row<=9; ++row)
        {
            for(int col=0; col<=8; ++col)
            {
                int killid = this->getStoneId(row, col);
                if(sameColor(i, killid)) continue;

                if(canMove(i, killid, row, col))
                {
                    saveStep(i, killid, row, col, steps);
                }
            }
        }
    }
#endif
    }
}






















