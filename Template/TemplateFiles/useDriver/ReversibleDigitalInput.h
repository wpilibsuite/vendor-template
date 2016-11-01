#pragma once

#include "ReversibleDigitalInputDriver.h"
#include "SensorBase.h"

namespace $vendorreplace$ {

class ReversibleDigitalInput : SensorBase {
 public:
  ReversibleDigitalInput(int channel);
  ~ReversibleDigitalInput();
  
  void SetReverseInput(bool reverse);
  bool Get();
 private:
  $vendorallcaps$_Handle m_handle;
};
}
