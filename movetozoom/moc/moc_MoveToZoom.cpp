/****************************************************************************
** Meta object code from reading C++ file 'MoveToZoom.h'
**
** Created: Fri Apr 1 17:51:30 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../MoveToZoom.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MoveToZoom.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MoveToZoom[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   12,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      31,   11,   11,   11, 0x0a,
      59,   45,   11,   11, 0x0a,
     109,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MoveToZoom[] = {
    "MoveToZoom\0\0factor\0zoom(qreal)\0"
    "startSensor()\0sensorReading\0"
    "handleSensorReading(const QAccelerometerReading*)\0"
    "captureInitialReading()\0"
};

const QMetaObject MoveToZoom::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MoveToZoom,
      qt_meta_data_MoveToZoom, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MoveToZoom::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MoveToZoom::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MoveToZoom::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MoveToZoom))
        return static_cast<void*>(const_cast< MoveToZoom*>(this));
    return QObject::qt_metacast(_clname);
}

int MoveToZoom::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: zoom((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 1: startSensor(); break;
        case 2: handleSensorReading((*reinterpret_cast< const QAccelerometerReading*(*)>(_a[1]))); break;
        case 3: captureInitialReading(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void MoveToZoom::zoom(qreal _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
