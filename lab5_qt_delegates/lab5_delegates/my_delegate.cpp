#include "my_delegate.h"
#include <QSpinBox>
#include <QPainter>
#include <QPalette>

myDelegate::myDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}
QWidget * myDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.data().canConvert<QString>())
    {
    QSpinBox* spinBox = new QSpinBox(parent);
    //spinBox ->setRange(0,100);
    QObject::connect(spinBox, SIGNAL(editingFinished()), this, SLOT(commitAndCloseEditor()));
    return spinBox;
    }else
    {
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
}


void myDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    if(index.data().canConvert<QString>())
    {
    currentValue = qvariant_cast<QString>(index.data());
    QSpinBox * spinBox = qobject_cast<QSpinBox*>(editor);
    spinBox -> setRange(0, currentValue.length());
    spinBox -> setValue(currentValue.length());
    }else
    {
        QStyledItemDelegate::setEditorData(editor,index);
    }
}
void myDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(index.data().canConvert<QString>())
    {
    QSpinBox * spinBox = qobject_cast<QSpinBox*>(editor);
    spinBox ->interpretText();
    int value = spinBox->value();
    //QString tmp = crrentValue.toUpper();
    model -> setData(index, QVariant::fromValue(currentValue.left(value)));
    }
}

void myDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.data().canConvert<QString>())
    {
        QStyleOptionViewItem myOption = option;
        QString value = qvariant_cast<QString>(index.data());
        /*if(myOption.state & QStyle::State_Selected)
        {
            painter-> fillRect(option.rect, myOption.palette.highlight());
        }*/
        if(value.length()%2)
        {
            //myOption.palette.setCurrentColorGroup(QPalette::AlternateBase);
            paintText(painter, myOption, value, QPalette::AlternateBase);
        }
        else
        {
            //myOption.palette.setCurrentColorGroup(QPalette::Base);
            paintText(painter, myOption, value, QPalette::Base);
        }

    }else
    {
        QStyledItemDelegate::paint(painter, option, index);
    }

}

void myDelegate::commitAndCloseEditor()
{
    QSpinBox* spinBox = qobject_cast<QSpinBox*> (sender());
    emit commitData(spinBox);
    emit closeEditor(spinBox);
}

QSize myDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.data().canConvert<QString>())
    {
        QString string = qvariant_cast<QString>(index.data());
        return QSize(200,30);
    }else
    {
        return QStyledItemDelegate::sizeHint(option, index);
    }
}


void myDelegate::paintText(QPainter* painter, const QStyleOptionViewItem& option, const QString& inp
                           ,QPalette::ColorRole role) const
{
    painter -> save();
    painter -> setRenderHint(QPainter::Antialiasing, true);
    if(role == QPalette::Base)
    painter ->setBrush(QBrush(Qt::gray, Qt::SolidPattern));
    if(role == QPalette::AlternateBase)
    painter ->setBrush(QBrush(Qt::green, Qt::SolidPattern));
    painter ->drawRect(option.rect);
    painter ->setBrush(Qt::NoBrush);
    painter ->setPen(QPen(Qt::black, 4, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
    painter ->drawText(option.rect, option.displayAlignment, inp.toUpper());
    painter ->restore();
}


