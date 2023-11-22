#ifndef LLVM_LIB_TARGET_CPU0_CPU0INSTRINFO_H
#define LLVM_LIB_TARGET_CPU0_CPU0INSTRINFO_H

#include "Cpu0.h"
#include "Cpu0RegisterInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/TargetInstrInfo.h"


#define GET_INSTRINFO_HEADER
#include "Cpu0GenInstrInfo.inc"


namespace llvm{
    class Cpu0InstrInfo : public Cpu0GenInstrInfo
    {
        virtual void anchor();
        protected:
            const Cpu0Subtarget &Subtarget;
        public:
            explicit Cpu0InstrInfo(const Cpu0Subtarget &STI);
            static const Cpu0InstrInfo* create(Cpu0Subtarget &STI);
            virtual const Cpu0RegisterInfo &getRegisterInfo() const = 0;
            unsigned GetInstSizeInBytes(const MachineInstr &MI) const;
    };

    const Cpu0InstrInfo *createCpu0SEInstrInfo(const Cpu0Subtarget &STI);
}

#endif