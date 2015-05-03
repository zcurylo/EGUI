/*
 * Made by Zbigniew Witold Curylo
 * */


#ifndef MAIN_WIDGET_H
#define MAIN_WIDGET_H

#include<QWidget>
#include<vector>
#include"my_push_button.h"
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QTimer>
#include<QSizePolicy>

class main_widget : public QWidget
{
    Q_OBJECT
public:
    explicit main_widget(QWidget *parent = 0);
    enum class settings:int {BUTTONS_NUMBER = 8};
signals:
    void hit(QString) const;
    void miss(QString) const;
public slots:
    void setActive(bool ready);
    void reset();
private slots:
    void handleHitEvent(bool);
    void update();
private:
    QTimer* timer_m;
    static long numberOfHits_m;
    static long numberOfFailures_m;
    volatile bool ready_m;
   // bool reseted_m; // do not know if i need it actually
    long counter_m;
    /*QHBoxLayout topLayout;
    QHBoxLayout middleLayout;
    QHBoxLayout bottomLayout;
    QVBoxLayout mainLayout;*/


    std::vector<my_push_button*> buttons_m;
};

#endif // MAIN_WIDGET_H
