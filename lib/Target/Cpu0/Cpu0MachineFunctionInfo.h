#ifndef LLVM_LIB_TARGET_CPU0_CPU0MACHINEFUNCTIONINFO_H
#define LLVM_LIB_TARGET_CPU0_CPU0MACHINEFUNCTIONINFO_H


#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineMemOperand.h"
#include "llvm/CodeGen/PseudoSourceValue.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/Target/TargetMachine.h"
#include <map>



namespace llvm{
    class Cpu0MachineFunctionInfo : public MachineFunctionInfo{
        public:

            Cpu0MachineFunctionInfo(MachineFunction &MF) : MF(MF),VarArgsFrameIndex(0),MaxCallFrameSize(0)
            {

            }
            ~Cpu0MachineFunctionInfo();

            int getVarArgsFrameIndex() const {
                return VarArgsFrameIndex;
            }

            void setVarArgsFrameIndex(int Index)
            {
                VarArgsFrameIndex = Index;
            }

        private:
            virtual void anchor();

            MachineFunction &MF;
            int VarArgsFrameIndex;
            unsigned MaxCallFrameSize;
    };
}

#endif