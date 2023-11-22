#ifndef LLVM_LIB_TARGET_CPU0_CPU0FRAMELOWERING_H
#define LLVM_LIB_TARGET_CPU0_CPU0FRAMELOWERING_H
#include "Cpu0.h"
#include "llvm/CodeGen/TargetFrameLowering.h"



namespace llvm{
    class Cpu0Subtarget;

    class Cpu0FrameLowering : public TargetFrameLowering{
        protected:
            const Cpu0Subtarget &STI;
        public:
        explicit Cpu0FrameLowering(const Cpu0Subtarget &sti,unsigned Alignment) : TargetFrameLowering(StackGrowsDown,Alignment,0,Alignment),STI(sti){}

        static const Cpu0FrameLowering *create(const Cpu0Subtarget &ST);
        
        bool hasFP(const MachineFunction &MF) const override;
    };

    const Cpu0FrameLowering *createCpu0SEFrameLowering(const Cpu0Subtarget &ST);
}


#endif