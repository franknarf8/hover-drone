#include "CommSNES.h"
#include <QThreadPool>
#include <iostream>

#include "hidapi.h"

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

ControllerInput::ControllerInput() {
    QThreadPool::globalInstance()->start(this);
}

ControllerInput::~ControllerInput() {
    QThread::currentThread()->exit();
}

void ControllerInput::run() {
    int res;
    unsigned char data[8] = {0};
    KeyCode key(0x0);
    KeyCode lastKey(0x0);
    KeyCode noInput(0x00000F8080807F7F);
    //unsigned char noInput[8] = {127, 127, 128, 128, 128, 15, 0};
    KeyCode upInput(0x00000F808080007F);
    //unsigned char upInput[8] = {127, 0, 128, 128, 128, 15, 0, 0};
    KeyCode leftInput(0x00000F8080807F00);
    //unsigned char leftInput[8] = {0, 127, 128, 128, 128, 15, 0, 0};
    KeyCode downInput(0x00000F808080FF7F);
    //unsigned char downInput[8] = {127, 255, 128, 128, 128, 15, 0, 0};
    KeyCode rightInput(0x00000F8080807FFF);
    //unsigned char rightInput[8] = {255, 127, 128, 128, 128, 15, 0, 0};
    hid_device *handle = NULL;
    // Initialize the hidapi library
    res = hid_init();

    hid_device_info *list = hid_enumerate(2079, 58369);

    if(!list) {
        std::cout << "Unable to reach device!";
        return;
    }

    // Open the device using the VID, PID,
    // and optionally the Serial number.
    //handle = hid_open(cur_dev.vendor_id, cur_dev.product_id, 0);
    handle = hid_open_path(list->path);

    if(!handle) {
        std::cout << "Unable to reach device!";
        return;
    }

    while(true) {
        // Read requested state
        res = hid_read(handle, data, CODE_LENGTH);
    
        if(lastKey != data) {
            lastKey = data;
            if(lastKey == noInput) {
                emit stateChanged(CTRLSTATE::NOINPUT);
            } else if(lastKey == leftInput) {
                emit stateChanged(CTRLSTATE::LEFT);
            } else if(lastKey == upInput) {
                emit stateChanged(CTRLSTATE::UP);
            } else if(lastKey == downInput) {
                emit stateChanged(CTRLSTATE::DOWN);
            } else if(lastKey == rightInput) {
                emit stateChanged(CTRLSTATE::RIGHT);
            } else {
                emit stateChanged(CTRLSTATE::UNKNOWN);
            }
        }
    //QThread::currentThread()->msleep(STDDELAY);
    }
  
    // Finalize the hidapi library
    res = hid_exit();
    //emit stateChanged(value);
}
