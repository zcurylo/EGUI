/*
 * Made By Zbigniew Witold Curylo
 *
 *
 *
*/


#include"main_widget.h"
#include <QApplication>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSlider>
#include <QSpinBox>
#include <QPushButton>

int main(int argc, char * tab[])
{
    QApplication app(argc, tab);

        QWidget* window = new QWidget;
    window -> setWindowTitle(QObject::tr("Main Application window"));

        main_widget* main_button = new main_widget();
        //QObject::connect(main_button, SIGNAL(clicked()), &app, SLOT(quit()));

        QVBoxLayout* v_layout = new QVBoxLayout;
        QHBoxLayout* h_layout = new QHBoxLayout;

        QPushButton* pause_button = new QPushButton("PAUSE");
        pause_button ->setCheckable(true);
        QPushButton* reset_button = new QPushButton("RESET");
        QPushButton* quit_button = new QPushButton("QUIT");
        QLabel* hitsNumber = new QLabel;
        QLabel * missesNumber = new QLabel;
        QObject::connect(main_button,SIGNAL(hit(QString)), hitsNumber, SLOT(setText(QString)));
        QObject::connect(main_button,SIGNAL(miss(QString)), missesNumber, SLOT(setText(QString)));
        QObject::connect(quit_button, SIGNAL(clicked()), &app, SLOT(quit()));
        QObject::connect(pause_button,SIGNAL(toggled(bool)),main_button,SLOT(setActive(bool)));
       // QObject::connect(pause_button,SIGNAL(released()),main_button,SLOT(setActive(true)));
        QObject::connect(reset_button, SIGNAL(clicked()), main_button, SLOT(reset()));
        v_layout->addWidget(new QLabel("SUCCESSSES"));
        v_layout ->addWidget(hitsNumber);
        v_layout ->addWidget(new QLabel("FAILURES"));
        v_layout ->addWidget(missesNumber);
        v_layout ->addStretch();
        v_layout -> addWidget(pause_button);
        v_layout -> addWidget(reset_button);
        v_layout -> addWidget(quit_button);
        h_layout -> addWidget(main_button);
        h_layout -> addLayout(v_layout);

        window -> setLayout(h_layout);
        window -> show();
        main_button -> setActive(false);
        return app.exec();
}

//#include "main.moc"
