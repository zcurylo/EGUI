/****************************************************************************
** Meta object code from reading C++ file 'main_widget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../lab4/main_widget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'main_widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_main_widget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,
      26,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      46,   40,   12,   12, 0x0a,
      62,   12,   12,   12, 0x0a,
      70,   12,   12,   12, 0x08,
      91,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_main_widget[] = {
    "main_widget\0\0hit(QString)\0miss(QString)\0"
    "ready\0setActive(bool)\0reset()\0"
    "handleHitEvent(bool)\0update()\0"
};

void main_widget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        main_widget *_t = static_cast<main_widget *>(_o);
        switch (_id) {
        case 0: _t->hit((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->miss((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->setActive((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->reset(); break;
        case 4: _t->handleHitEvent((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->update(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData main_widget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject main_widget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_main_widget,
      qt_meta_data_main_widget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &main_widget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *main_widget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *main_widget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_main_widget))
        return static_cast<void*>(const_cast< main_widget*>(this));
    return QWidget::qt_metacast(_clname);
}

int main_widget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void main_widget::hit(QString _t1)const
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(const_cast< main_widget *>(this), &staticMetaObject, 0, _a);
}

// SIGNAL 1
void main_widget::miss(QString _t1)const
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(const_cast< main_widget *>(this), &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
