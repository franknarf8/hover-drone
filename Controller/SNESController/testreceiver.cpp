#include "testreceiver.h"
#include <QDebug>

TestReceiver::TestReceiver(QObject *parent) : QObject(parent)
{

}

TestReceiver::~TestReceiver()
{

}

void TestReceiver::logState(CTRLSTATE state) {
    switch(state) {
    case CTRLSTATE::NOINPUT:
        qDebug() << "No input";
        break;
    case CTRLSTATE::UP:
        qDebug() << "Up";
        break;
    case CTRLSTATE::DOWN:
        qDebug() << "Down";
        break;
    case CTRLSTATE::LEFT:
        qDebug() << "Left";
        break;
    case CTRLSTATE::RIGHT:
        qDebug() << "Right";
        break;
    default:
        qDebug() << "Unknown";
    }
}
