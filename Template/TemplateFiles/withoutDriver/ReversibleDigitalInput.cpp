#include "ReversibleDigitalInput.h"

namespace $vendorreplace$ {
  
using namespace frc;

ReversibleDigitalInput::ReversibleDigitalInput(int channel) : m_reverse(true) {
  m_digitalInput = std::make_unique<DigitalInput>(channel);
}

void ReversibleDigitalInput::SetReverseInput(bool reverse) {
  m_reverse = reverse;
}

bool ReversibleDigitalInput::Get() {
  bool val = m_digitalInput->Get();
  return m_reverse ? !val : val;
}

}
