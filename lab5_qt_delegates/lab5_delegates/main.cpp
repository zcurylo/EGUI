#include<QApplication>
#include<QListView>
#include<QStringList>
#include<QStringListModel>
#include<QListView>
#include<QAbstractItemModel>
#include<QHBoxLayout>

#include "my_delegate.h"



int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QStringList words;
    words << "a"
            <<"bB"
            <<"ccC"
            <<"dddD"
            <<"eeeeE"
            <<"fffffF";
    QAbstractItemModel* model = new QStringListModel(words);
    QListView* viewLeft = new QListView;
    QListView* viewRight = new QListView;
    viewLeft ->setModel(model);
    viewRight ->setModel(model);
    viewRight ->setItemDelegate(new myDelegate);
    QHBoxLayout* layout = new QHBoxLayout;
    layout ->addWidget(viewLeft);
    layout ->addWidget(viewRight);
    QWidget* window = new QWidget;
    window -> setWindowTitle(QObject::tr("Main Application window"));
    window ->setLayout(layout);
    window ->show();
    return app.exec();
}
