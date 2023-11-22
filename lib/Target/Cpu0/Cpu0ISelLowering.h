#ifndef LLVM_LIB_TARGET_CPU0_CPU0ISELLOWERING_H
#define LLVM_LIB_TARGET_CPU0_CPU0ISELLOWERING_H

#include "MCTargetDesc/Cpu0ABIInfo.h"
#include "Cpu0.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/TargetLowering.h"
#include "llvm/IR/Function.h"
#include <deque>


namespace llvm{
    namespace Cpu0ISD{
        enum NodeType
        {
            FIRST_NUMBER= ISD::BUILTIN_OP_END,
            JmpLink,
            TailCall,
            Hi,
            Lo,
            GPRel,
            ThreadPointer,
            Ret,
            EH_RETURN,
            DivRem,
            DivRemU,
            Wrapper,
            DynAlloc,
            Sync
        };
    }

    class Cpu0MachineFunctionInfo;
    class Cpu0Subtarget;

    class Cpu0TargetLowering : public TargetLowering{
        public:
            explicit Cpu0TargetLowering(const Cpu0TargetMachine &TM,const Cpu0Subtarget &STI);

            static const Cpu0TargetLowering* create(const Cpu0TargetMachine &TM,const Cpu0Subtarget &STI);

            const char* getTargetNodeName(unsigned Opcode) const override;
        protected:
            struct ByValArgInfo
            {
                unsigned FirstIdx;
                unsigned NumRegs;
                unsigned Address;
                ByValArgInfo() : FirstIdx(0),NumRegs(0),Address(0){} 
            };
            
            const Cpu0Subtarget &Subtarget;
            const Cpu0ABIInfo &ABI;
        private:
            SDValue LowerGlobalAddress(SDValue Op,SelectionDAG &DAG) const;
            SDValue LowerFormalArguments(SDValue Chain,
                               CallingConv::ID CallConv, bool IsVarArg,
                               const SmallVectorImpl<ISD::InputArg> &Ins,
                               const SDLoc &dl, SelectionDAG &DAG,
                               SmallVectorImpl<SDValue> &InVals) const override;    
            SDValue LowerReturn(SDValue Chain,
                                CallingConv::ID CallConv, bool IsVarArg,
                                const SmallVectorImpl<ISD::OutputArg> &Outs,
                                const SmallVectorImpl<SDValue> &OutVals,
                                const SDLoc &dl, SelectionDAG &DAG) const override;                
};
    const Cpu0TargetLowering* createCpu0SETargetLowering(const Cpu0TargetMachine &TM,const Cpu0Subtarget &STI);
}

#endif