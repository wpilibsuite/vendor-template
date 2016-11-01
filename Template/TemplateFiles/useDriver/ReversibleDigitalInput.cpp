#include "ReversibleDigitalInput.h"

#include "HAL/HAL.h"

namespace $vendorreplace$ {

ReversibleDigitalInput::ReversibleDigitalInput(int channel) {
  int32_t status = 0;
  m_handle = $vendorallcaps$_InitializeReversibleDigitalInput(channel, &status);
  if (status != 0) {
    wpi_setErrorWithContextRange(status, 0, HAL_GetNumDigitalChannels(),
                                 channel, HAL_GetErrorMessage(status));
    m_handle = HAL_kInvalidHandle;
    return;
  }
}

ReversibleDigitalInput::~ReversibleDigitalInput() {
  $vendorallcaps$_FreeReversibleDigitalInput(m_handle);
}

void ReversibleDigitalInput::SetReverseInput(bool reverse) {
  if (StatusIsFatal()) return;
  int32_t status = 0;
  $vendorallcaps$_SetReverseReversibleDigitalInput(m_handle, reverse, &status);
  wpi_setErrorWithContext(status, HAL_GetErrorMessage(status));
}

bool ReversibleDigitalInput::Get() {
  if (StatusIsFatal()) return false;
  int32_t status = 0;
  auto retVal = $vendorallcaps$_GetReversibleDigitalInputValue(m_handle, &status);
  wpi_setErrorWithContext(status, HAL_GetErrorMessage(status));
  return retVal;
}

}
