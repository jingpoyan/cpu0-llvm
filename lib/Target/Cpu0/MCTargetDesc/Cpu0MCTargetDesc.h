#ifndef LLVM_LIB_TARGET_CPU0_MCTARGETDESC_CPU0_MCTARGETDESC_H
#define LLVM_LIB_TARGET_CPU0_MCTARGETDESC_CPU0_MCTARGETDESC_H


#include "llvm/Support/DataTypes.h"


namespace llvm {
    class Target;
    class Triple;

    Target &getTheCpu0Target();
    Target &getTheCpu0elTarget();
}

#define GET_REGINFO_ENUM
#include "Cpu0GenRegisterInfo.inc"

#define GET_INSTRINFO_ENUM
#include "Cpu0GenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "Cpu0GenSubtargetInfo.inc"


#endif