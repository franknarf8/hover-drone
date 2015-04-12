#ifndef COMMSNES_H
#define COMMSNES_H

#include <cstdint>

#define CODE_LENGTH 8

//enum{}

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

//TODO thread independent class with a callback function which
//sends the enum state of the state machine's instance

#endif
