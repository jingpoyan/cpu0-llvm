#ifndef LLVM_LIB_TARGET_CPU0_CPU0SEFRAMELOWERING_H
#define LLVM_LIB_TARGET_CPU0_CPU0SEFRAMELOWERING_H

#include "Cpu0FrameLowering.h"

namespace llvm{
    class Cpu0SEFrameLowering : public Cpu0FrameLowering{
        public:
            explicit Cpu0SEFrameLowering(const Cpu0Subtarget &STI);

            void emitPrologue(MachineFunction &MF,MachineBasicBlock &MBB) const override;
            void emitEpilogue(MachineFunction &MF,MachineBasicBlock &MBB) const override;
            bool hasReservedCallFrame(const MachineFunction &MF) const override;
            void determineCalleeSaves(MachineFunction &MF, BitVector &SavedRegs,
                                        RegScavenger *RS) const override;
    };
}

#endif