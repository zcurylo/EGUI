#ifndef MY_DELEGATE_H
#define MY_DELEGATE_H

#include <QStyledItemDelegate>
#include <QStyleOptionViewItem>

class myDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit myDelegate(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget* editor, const QModelIndex& index)const;

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
signals:

public slots:

private slots:
    void commitAndCloseEditor();

private:
    mutable QString currentValue;
    void paintText(QPainter* painter, const QStyleOptionViewItem& option, const QString& inp
                   , QPalette::ColorRole inp2) const;
};

#endif // MY_DELEGATE_H
