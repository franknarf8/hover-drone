#ifndef TESTRECEIVER_H
#define TESTRECEIVER_H

#include <QObject>
#include "CommSNES.h"

class TestReceiver : public QObject
{
    Q_OBJECT
public:
    explicit TestReceiver(QObject *parent = 0);
    ~TestReceiver();

public slots:
    void logState(CTRLSTATE);
};

#endif // TESTRECEIVER_H
