#include "my_push_button.h"
#include<QtGui>

my_push_button::my_push_button(QWidget *parent) :
    QWidget(parent), state_m(stateColor::NORMAL)
{
        //setAttribute(Qt::WA_);
}

void my_push_button::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(state_m == stateColor::NORMAL)
        {
            emit hitResult(false);
            state_m = stateColor::SELECTED;
            repaint();
        }
        if(state_m == stateColor::SELECTED)
        {
            emit hitResult(true);
            state_m=stateColor::NORMAL;
            repaint();
        }
    }
}

void my_push_button::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, 4, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
    QPoint point1(10,10);
    QPoint point2(10,width()-10);
    QPoint point3(height()-10,width()-10);
    QPoint point4(height()-10,10);
    QVector<QPoint> path;
    path.append(point1);
    path.append(point2);
    path.append(point3);
    path.append(point4);
    painter.drawPolyline(path);
    if(state_m == stateColor::NORMAL)
    {
        painter.setBrush(QBrush(Qt::blue,Qt::SolidPattern));
        painter.drawRect(20,20,width()-40, height()-40);
    }else
    {
        painter.setBrush(QBrush(Qt::green,Qt::SolidPattern));
        painter.drawRect(20,20,width()-40, height()-40);
    }
}

QSize my_push_button::sizeHint() const
{
    return QSize(50,50);
}

void my_push_button::setState(stateColor inp)
{
    state_m = inp;
    repaint();
}

