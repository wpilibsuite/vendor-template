#pragma once

#include "DigitalInput.h"

#include <memory>

namespace $vendorreplace$ {

class ReversibleDigitalInput : SensorBase {
 public:
  ReversibleDigitalInput(int channel);
  
  void SetReverseInput(bool reverse);
  bool Get();
 private:
  bool m_reverse;
  std::unique_ptr<DigitalInput> m_digitalInput;
};
}
