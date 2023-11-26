#ifndef LLVM_LIB_TARGET_CPU0_CPU0TARGETSTREAMER_H
#define LLVM_LIB_TARGET_CPU0_CPU0TARGETSTREAMER_H

#include "llvm/MC/MCELFStreamer.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCStreamer.h"

namespace llvm {
class Cpu0TargetStreamer : public MCTargetStreamer {
public:
  Cpu0TargetStreamer(MCStreamer &S);
};

// This part is for ascii assembly output
class Cpu0TargetAsmStreamer : public Cpu0TargetStreamer {
  formatted_raw_ostream &OS;

public:
  Cpu0TargetAsmStreamer(MCStreamer &S, formatted_raw_ostream &OS);
};

}

#endif