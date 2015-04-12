#include "CommSNES.h"
#include <iostream>

/*KeyCode::KeyCode(unsigned char* value) {
  unsigned char* charHolder = new unsigned char[CODE_LENGTH];
  copy(charHolder, value);
  val = (uint64_t*) charHolder;
  }*/

KeyCode::KeyCode(uint64_t value) {
  val = new uint64_t;
  *val = value;
}

KeyCode::~KeyCode() {
  delete val;
}

uint64_t KeyCode::value() const {
  return *val;
}

void KeyCode::copy(unsigned char* container, unsigned char* data) {
  for(int address = 0; address < CODE_LENGTH; ++address) {
    container[address] = data[address];
  }
}

bool KeyCode::operator ==(const KeyCode& k) const {
  return *val == k.value();
}

bool KeyCode::operator !=(const KeyCode& k) const {
  return *val != k.value();
}

bool KeyCode::operator ==(unsigned char* value) const {
  return *val == *((uint64_t*) value);
}

bool KeyCode::operator !=(unsigned char* value) const {
  return *val != *((uint64_t*) value);
}

KeyCode& KeyCode::operator =(unsigned char* value) {
  *val = *((uint64_t*) value);
  return *this;
}
