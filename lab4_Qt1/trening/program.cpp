
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
	
	QPushButton* main_button = new QPushButton("Quit");
	QObject::connect(main_button, SIGNAL(clicked()), &app, SLOT(quit()));

	QVBoxLayout* v_layout = new QVBoxLayout;
	QHBoxLayout* h_layout = new QHBoxLayout;
	
	QPushButton* pause_button = new QPushButton("PAUSE");
	QPushButton* reset_button = new QPushButton("RESET");
	QPushButton* quit_button = new QPushButton("QUIT");
	
	v_layout -> addWidget(pause_button);
	v_layout -> addWidget(reset_button);
	v_layout -> addWidget(quit_button);
	h_layout -> addWidget(main_button);
	h_layout -> addLayout(v_layout);
	
	window -> setLayout(h_layout);
	window -> show();
	return app.exec();
}


