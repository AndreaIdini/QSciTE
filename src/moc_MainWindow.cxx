/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.hpp'
**
** Created: Tue Feb 25 21:03:42 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "MainWindow.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      31,   22,   11,   11, 0x0a,
      49,   11,   11,   11, 0x2a,
      60,   11,   11,   11, 0x0a,
      80,   11,   11,   11, 0x2a,
      91,   11,   11,   11, 0x0a,
     113,   11,   11,   11, 0x2a,
     126,   11,   11,   11, 0x0a,
     141,   11,   11,   11, 0x0a,
     148,   11,   11,   11, 0x0a,
     155,   11,   11,   11, 0x0a,
     162,   11,   11,   11, 0x0a,
     180,   11,   11,   11, 0x2a,
     191,   11,   11,   11, 0x0a,
     206,   11,   11,   11, 0x0a,
     216,   11,   11,   11, 0x0a,
     230,   11,   11,   11, 0x0a,
     250,   11,   11,   11, 0x0a,
     271,   11,   11,   11, 0x0a,
     292,   11,   11,   11, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0newFile()\0fileName\0"
    "openFile(QString)\0openFile()\0"
    "saveFile(EditorPtr)\0saveFile()\0"
    "saveAsFile(EditorPtr)\0saveAsFile()\0"
    "closeFile(int)\0undo()\0redo()\0find()\0"
    "findNext(QString)\0findNext()\0"
    "findPrevious()\0replace()\0replaceNext()\0"
    "setSingleSplitter()\0setDoubleHSplitter()\0"
    "setDoubleVSplitter()\0editorModified(bool)\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->newFile(); break;
        case 1: _t->openFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->openFile(); break;
        case 3: _t->saveFile((*reinterpret_cast< EditorPtr(*)>(_a[1]))); break;
        case 4: _t->saveFile(); break;
        case 5: _t->saveAsFile((*reinterpret_cast< EditorPtr(*)>(_a[1]))); break;
        case 6: _t->saveAsFile(); break;
        case 7: _t->closeFile((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->undo(); break;
        case 9: _t->redo(); break;
        case 10: _t->find(); break;
        case 11: _t->findNext((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 12: _t->findNext(); break;
        case 13: _t->findPrevious(); break;
        case 14: _t->replace(); break;
        case 15: _t->replaceNext(); break;
        case 16: _t->setSingleSplitter(); break;
        case 17: _t->setDoubleHSplitter(); break;
        case 18: _t->setDoubleVSplitter(); break;
        case 19: _t->editorModified((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
