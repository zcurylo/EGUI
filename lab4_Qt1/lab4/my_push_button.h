/*
 *  Made By Zbigniew Witold Curylo
 * */


#ifndef MY_PUSH_BUTTON_H
#define MY_PUSH_BUTTON_H

#include <QWidget>
#include <QColor>
#include <QSize>

class my_push_button : public QWidget
{
    Q_OBJECT

public:
    explicit my_push_button(QWidget *parent = 0);
    enum class stateColor: int {NORMAL = 0, SELECTED = 1};
    virtual QSize sizeHint() const;
signals:
    void hitResult(bool result)const;
public slots:
    void setState(stateColor inp);

protected:
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
        my_push_button::stateColor state_m;
};

#endif // MY_PUSH_BUTTON_H
