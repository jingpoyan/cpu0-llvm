#ifndef LLVM_LIB_TARGET_CPU0_CPU0SEINSTRINFO_H
#define LLVM_LIB_TARGET_CPU0_CPU0SEINSTRINFO_H

#include "Cpu0InstrInfo.h"
#include "Cpu0SERegisterInfo.h"
#include "Cpu0MachineFunctionInfo.h"

namespace llvm{
    class Cpu0SEInstrInfo : public Cpu0InstrInfo
    {
        const Cpu0SERegisterInfo RI;

        public:
            explicit Cpu0SEInstrInfo(const Cpu0Subtarget &STI);
            const Cpu0RegisterInfo &getRegisterInfo() const override;    
            bool expandPostRAPseudo(MachineInstr &MI) const override;
        private:
            void expandRetLR(MachineBasicBlock &MBB,MachineBasicBlock::iterator I) const;
    };
}


#endif