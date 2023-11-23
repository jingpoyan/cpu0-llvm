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

            Cpu0MachineFunctionInfo(MachineFunction &MF) : MF(MF),VarArgsFrameIndex(0),SRetReturnReg(0),CallsEhReturn(false),CallsEhDwarf(false),MaxCallFrameSize(0),EmitNOAT(false)
            {

            }

            ~Cpu0MachineFunctionInfo();

            unsigned getSRetReturnReg() const 
            {
                return SRetReturnReg;
            }
            
            void setSRetReturnReg(unsigned Reg)
            {
                SRetReturnReg = Reg;
            }


            int getVarArgsFrameIndex() const 
            {
                return VarArgsFrameIndex;
            }

            void setVarArgsFrameIndex(int Index)
            {
                VarArgsFrameIndex = Index;
            }

            bool hasByvalArg() const
            {
                return HasByvalArg;
            }
            
            void setFormalArgInfo(unsigned Size,bool HasByval)
            {
                IncomingArgSize = Size;
                HasByvalArg = HasByval;
            }

            bool getEmitNOAT() const
            {
                return EmitNOAT;
            }

            void setEmitNOAT()
            {
                EmitNOAT = true;
            }

        private:
            virtual void anchor();

            MachineFunction &MF;
            int VarArgsFrameIndex;
            unsigned MaxCallFrameSize;
            bool EmitNOAT;

            unsigned SRetReturnReg;
            bool HasByvalArg;
            unsigned IncomingArgSize;
            bool CallsEhReturn;
            bool CallsEhDwarf;
            bool EhDataRegFI[2];
    };
}

#endif