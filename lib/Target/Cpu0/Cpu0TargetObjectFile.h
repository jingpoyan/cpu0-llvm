#ifndef LLVM_LIB_TARGET_CPU0_CPU0TARGETOBJECTFILE_H
#define LLVM_LIB_TARGET_CPU0_CPU0TARGETOBJECTFILE_H

#include "Cpu0TargetMachine.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"

namespace llvm{
    class Cpu0TargetMachine;
    class Cpu0TargetObjectFile : public TargetLoweringObjectFileELF {
        MCSection *SmallDataSection;
        MCSection *SmallBSSSection;
        const Cpu0TargetMachine* TM;

        public:
            void Initialize(MCContext &Ctx,const TargetMachine &TM) override;
    };
}
#endif