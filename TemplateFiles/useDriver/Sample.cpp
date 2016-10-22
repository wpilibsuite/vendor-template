#include "Sample.h"

#include "SampleDriver.h"

int WriteStringToConsole(llvm::StringRef toWrite) {
  return SAMPLE_WriteStringToConsole(toWrite.data());
}