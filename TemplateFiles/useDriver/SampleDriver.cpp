#include "SampleDriver.h"

#include "FRC_NetworkCommunication/FRCComm.h"

#include <cstring>

extern "C" {
  int SAMPLE_WriteStringToConsole(const char* toWrite) {
    FRC_NetworkCommunication_sendConsoleLine(toWrite);
    return std::strlen(toWrite);
  }
}