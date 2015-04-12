#ifndef COMMSNES_H
#define COMMSNES_H

#include <QRunnable>
#include <QObject>
#include <cstdint>

#define CODE_LENGTH 8
#define STDDELAY 100

enum CTRLSTATE {UNKNOWN, NOINPUT, UP, DOWN, LEFT, RIGHT};

class KeyCode {
public:
  //KeyCode(unsigned char* value);
  KeyCode(uint64_t value);
  ~KeyCode();

  bool operator ==(const KeyCode& k) const;
  bool operator !=(const KeyCode& k) const;
  bool operator ==(unsigned char* value) const;
  bool operator !=(unsigned char* value) const;
  KeyCode& operator =(unsigned char* value);
  uint64_t value() const;
private:
  uint64_t* val;

  static void copy(unsigned char* container, unsigned char* data);
};

class ControllerInput : public QObject, public QRunnable {
    Q_OBJECT
public:
    ControllerInput();
    ~ControllerInput();
    void run();
 signals:
    void stateChanged(CTRLSTATE);
};

#endif
