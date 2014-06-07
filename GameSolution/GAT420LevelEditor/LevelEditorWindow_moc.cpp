/****************************************************************************
** Meta object code from reading C++ file 'LevelEditorWindow.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "LevelEditorWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LevelEditorWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_LevelEditorWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x08,
      29,   18,   18,   18, 0x08,
      42,   18,   18,   18, 0x08,
      57,   18,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_LevelEditorWindow[] = {
    "LevelEditorWindow\0\0loadObj()\0saveNative()\0"
    "saveNativeAs()\0loadNative()\0"
};

void LevelEditorWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        LevelEditorWindow *_t = static_cast<LevelEditorWindow *>(_o);
        switch (_id) {
        case 0: _t->loadObj(); break;
        case 1: _t->saveNative(); break;
        case 2: _t->saveNativeAs(); break;
        case 3: _t->loadNative(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData LevelEditorWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject LevelEditorWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_LevelEditorWindow,
      qt_meta_data_LevelEditorWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LevelEditorWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LevelEditorWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LevelEditorWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LevelEditorWindow))
        return static_cast<void*>(const_cast< LevelEditorWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int LevelEditorWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
