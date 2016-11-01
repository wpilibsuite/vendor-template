#include "ReversibleDigitalInputDriver.h"

#include "HAL/handles/IndexedClassedHandleResource.h"

#include "HAL/DIO.h"

namespace $vendorreplace$ {

ReversibleDigitalInputDriver::ReversibleDigitalInputDriver(int32_t channel, int32_t* status) : m_reverse(true) {
  m_digitalHandle = HAL_InitializeDIOPort(channel, true, status);
}

ReversibleDigitalInputDriver::~ReversibleDigitalInputDriver() {
  HAL_FreeDIOPort(m_digitalHandle);
}

void ReversibleDigitalInputDriver::SetReverseInput(HAL_Bool reverse, int32_t* status) {
  m_reverse = reverse;
}

HAL_Bool ReversibleDigitalInputDriver::Get(int32_t* status) {
  HAL_Bool val = HAL_GetDIO(m_digitalHandle, status);
  if (*status != 0) return false;
  return m_reverse ? !val : val;
}
}

using namespace $vendorreplace$;

static hal::IndexedClassedHandleResource<$vendorallcaps$_Handle, ReversibleDigitalInputDriver, 26,
                      hal::HAL_HandleEnum::Vendor>
    reversibleDigitalInputHandles;

extern "C" {
$vendorallcaps$_Handle $vendorallcaps$_InitializeReversibleDigitalInput(int32_t channel, int32_t* status) {
  auto input = std::make_shared<ReversibleDigitalInputDriver>(channel, status);
  if (*status != 0) return HAL_kInvalidHandle;
  auto handle = reversibleDigitalInputHandles.Allocate(channel, input, status);
  if (*status != 0) return HAL_kInvalidHandle;
  return handle;
}
void $vendorallcaps$_FreeReversibleDigitalInput($vendorallcaps$_Handle handle) {
  reversibleDigitalInputHandles.Free(handle);
}
void $vendorallcaps$_SetReverseReversibleDigitalInput($vendorallcaps$_Handle handle, HAL_Bool reverse, int32_t* status) {
  auto input = reversibleDigitalInputHandles.Get(handle);
  if (input == nullptr) {
    *status = HAL_HANDLE_ERROR;
    return;
  }
  input->SetReverseInput(reverse, status);
}
HAL_Bool $vendorallcaps$($vendorallcaps$_Handle handle, int32_t* status) {
  auto input = reversibleDigitalInputHandles.Get(handle);
  if (input == nullptr) {
    *status = HAL_HANDLE_ERROR;
    return false;
  }
  return input->Get(status);
}
}
