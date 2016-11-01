#pragma once

#include "HAL/Types.h"

#define $vendorallcaps$_Handle HAL_Handle

namespace $vendorreplace$ {

class ReversibleDigitalInputDriver {
 public:
  ReversibleDigitalInputDriver(int32_t channel, int32_t* status);
  ~ReversibleDigitalInputDriver();
  
  void SetReverseInput(HAL_Bool reverse, int32_t* status);
  HAL_Bool Get(int32_t* status);
 private:
  HAL_Bool m_reverse;
  HAL_DigitalHandle m_digitalHandle;
};
}

extern "C" {
$vendorallcaps$_Handle $vendorallcaps$_InitializeReversibleDigitalInput(int32_t channel, int32_t* status);
void $vendorallcaps$_FreeReversibleDigitalInput($vendorallcaps$_Handle handle);
void $vendorallcaps$_SetReverseReversibleDigitalInput($vendorallcaps$_Handle handle, HAL_Bool reverse, int32_t* status);
HAL_Bool $vendorallcaps$_GetReversibleDigitalInputValue($vendorallcaps$_Handle handle, int32_t* status);
}
