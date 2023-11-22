#ifndef LLVM_LIB_TARGET_CPU0_CPU0SELISELLOWERING_H
#define LLVM_LIB_TARGET_CPU0_CPU0SELISELLOWERING_H

#include "Cpu0ISelLowering.h"
#include "Cpu0RegisterInfo.h"

namespace llvm{
    class Cpu0SETargetLowering : public Cpu0TargetLowering{
        public:
            explicit Cpu0SETargetLowering(const Cpu0TargetMachine &TM,const Cpu0Subtarget &STI);
            SDValue LowerOperation(SDValue Op,SelectionDAG &DAG) const override;
    };
}


#endif