/****************************************************************************
** Meta object code from reading C++ file 'my_push_button.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../lab4/my_push_button.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'my_push_button.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_my_push_button[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      23,   16,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      43,   39,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_my_push_button[] = {
    "my_push_button\0\0result\0hitResult(bool)\0"
    "inp\0setState(stateColor)\0"
};

void my_push_button::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        my_push_button *_t = static_cast<my_push_button *>(_o);
        switch (_id) {
        case 0: _t->hitResult((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->setState((*reinterpret_cast< stateColor(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData my_push_button::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject my_push_button::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_my_push_button,
      qt_meta_data_my_push_button, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &my_push_button::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *my_push_button::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *my_push_button::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_my_push_button))
        return static_cast<void*>(const_cast< my_push_button*>(this));
    return QWidget::qt_metacast(_clname);
}

int my_push_button::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void my_push_button::hitResult(bool _t1)const
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(const_cast< my_push_button *>(this), &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
