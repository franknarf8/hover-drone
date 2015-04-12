#include <iostream>
#include <stdio.h>
#include "hidapi.h"

#include "CommSNES.h"

#define CODE_LENGTH 8

std::ostream& operator<< (std::ostream& stream, KeyCode);

int main() {
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
    return 1;
  }

  // Open the device using the VID, PID,
  // and optionally the Serial number.
  //handle = hid_open(cur_dev.vendor_id, cur_dev.product_id, 0);
  handle = hid_open_path(list->path);

  if(!handle) {
    std::cout << "Unable to reach device!";
    return 1;
  }

  while(true) {
    // Read requested state
    res = hid_read(handle, data, CODE_LENGTH);
    
    if(lastKey != data) {
      lastKey = data;
      //log controller state
      std::cout << "Etat:" << std::endl;
      if(lastKey == noInput) {
        std::cout << "No input" << std::endl;
      } else if(lastKey == leftInput) {
        std::cout << "Left" << std::endl;
      } else if(lastKey == upInput) {
        std::cout << "Up" << std::endl;
      } else if(lastKey == downInput) {
        std::cout << "Down" << std::endl;
      } else if(lastKey == rightInput) {
        std::cout << "Right" << std::endl;
      } else {
        std::cout << "No implemented!" << std::endl;
      }
    }
    //TODO: sleep
  }
  
  // Finalize the hidapi library
  res = hid_exit();
  return 0;
}

std::ostream& operator<< (std::ostream& stream, KeyCode key)  {
  stream << key.value();
  return stream;
}
