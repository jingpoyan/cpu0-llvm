#ifndef LLVM_LIB_TARGET_CPU0_CPU0SEREGISTERINFO_H
#define LLVM_LIB_TARGET_CPU0_CPU0SEREGISTERINFO_H

#include "Cpu0RegisterInfo.h"

namespace llvm
{
    class Cpu0SEInstrInfo;
    class Cpu0SERegisterInfo : public Cpu0RegisterInfo
    {
        public:
            Cpu0SERegisterInfo(const Cpu0Subtarget &Subtarget);
            const TargetRegisterClass *intRegClass(unsigned Size) const override;
    };
}

#endif