/*
 * Made By Zbigniew Witold Curylo
 *
 *
 *
 * */




#include "main_widget.h"

long main_widget::numberOfHits_m = 0;
long main_widget::numberOfFailures_m = 0;


main_widget::main_widget(QWidget *parent) :
    QWidget(parent)
{
    setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
    setMinimumSize(300,400);
    resize(500,500);
    for(int i = 0; i < static_cast<int>(settings::BUTTONS_NUMBER); ++i)
    {
        my_push_button* button_ptr = new my_push_button;
        connect(button_ptr,SIGNAL(hitResult(bool)),this,SLOT(handleHitEvent(bool)));
        buttons_m.push_back(button_ptr);
    }
    timer_m = new QTimer(this);
    connect(timer_m,SIGNAL(timeout()),SLOT(update()));
    QHBoxLayout* topLayout = new QHBoxLayout;
    topLayout ->addWidget(buttons_m[0]);
    topLayout ->addWidget(buttons_m[1]);
    topLayout ->addWidget(buttons_m[2]);
    QHBoxLayout* middleLayout = new QHBoxLayout;
    middleLayout ->addWidget(buttons_m[7]);
    middleLayout ->addStretch();
    middleLayout ->addWidget(buttons_m[3]);
    QHBoxLayout* bottomLayout = new QHBoxLayout;
    bottomLayout ->addWidget(buttons_m[6]);
    bottomLayout ->addWidget(buttons_m[5]);
    bottomLayout ->addWidget(buttons_m[4]);
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout ->addLayout(topLayout);
    mainLayout ->addLayout(middleLayout);
    mainLayout ->addLayout(bottomLayout);
    setLayout(mainLayout);
    counter_m = 0;
}

void main_widget::handleHitEvent(bool inp)
{
    QString tmp;
   if(ready_m)
   {
    if(inp)
    {
        ++numberOfHits_m;
        tmp.setNum(numberOfHits_m);
        emit hit(tmp);
    }else
    {
        ++numberOfFailures_m;
        tmp.setNum(numberOfFailures_m);
        emit miss(tmp);
    }
   }
}

void main_widget::update()
{
    int i = counter_m % static_cast<long>(settings::BUTTONS_NUMBER);
    buttons_m[i] -> setState(my_push_button::stateColor::NORMAL);
    ++counter_m;
    int j = counter_m % static_cast<long>(settings::BUTTONS_NUMBER);
    buttons_m[j] -> setState(my_push_button::stateColor::SELECTED);
}

void main_widget::setActive(bool ready)
{
    ready_m = !ready;
    if(ready_m)
    {
        timer_m -> start(500);
    }else
    {
        timer_m->stop();
    }
}

void main_widget::reset()
{
    numberOfFailures_m = 0;
    numberOfHits_m = 0;
    QString tmp;
    tmp.setNum(0);
    emit hit(tmp);
    emit miss(tmp);
}


