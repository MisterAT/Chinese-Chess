#include "Step.h"

Step::Step(QObject *parent) : QObject(parent)
{

}

Step::~Step()
{

}

const Step& Step::operator=(const Step& ste){
    _moveid=ste._moveid;
    _killid=ste._killid;
    _rowFrom=ste._rowFrom;
    _colFrom=ste._colFrom;
    _rowTo=ste._rowTo;
    _colTo=ste._colTo;
    return ste;
}
bool Step::operator==(const Step& ste){
    if(_moveid==ste._moveid&&_killid==ste._killid&&_rowFrom==ste._rowFrom&&_colFrom==ste._colFrom&&_rowTo==ste._rowTo&&_colTo==ste._colTo)
        return true;
    return false;
}
