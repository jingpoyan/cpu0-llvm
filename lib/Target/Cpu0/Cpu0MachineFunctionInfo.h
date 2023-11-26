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

            Cpu0MachineFunctionInfo(MachineFunction &MF) : MF(MF), SRetReturnReg(0), CallsEhReturn(false), VarArgsFrameIndex(0), CallsEhDwarf(false), MaxCallFrameSize(0), EmitNOAT(false)
            {

            }

            ~Cpu0MachineFunctionInfo();

            bool globalBaseRegFixed() const;
            bool globalBaseRegSet() const;
            unsigned getGlobalBaseReg();


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


            unsigned getIncomingArgSize() const 
            {
                 return IncomingArgSize; 
            }

            bool callsEhReturn() const 
            { 
                return CallsEhReturn; 
            }


            void setCallsEhReturn() 
            {
                 CallsEhReturn = true; 
            }

            bool callsEhDwarf() const 
            {
                 return CallsEhDwarf; 
            }

            void setCallsEhDwarf() 
            {
                 CallsEhDwarf = true; 
            }


            void createEhDataRegsFI();

            int getEhDataRegFI(unsigned Reg) const 
            {
                 return EhDataRegFI[Reg]; 
            }

            unsigned getMaxCallFrameSize() const 
            {
                 return MaxCallFrameSize; 
            }

            void setMaxCallFrameSize(unsigned S) 
            {
                 MaxCallFrameSize = S; 
            }

        private:
            virtual void anchor();

            MachineFunction &MF;
            int VarArgsFrameIndex;
            unsigned MaxCallFrameSize;
            bool EmitNOAT;
            unsigned GlobalBaseReg;
            int GPFI;

            unsigned SRetReturnReg;
            bool HasByvalArg;
            unsigned IncomingArgSize;
            bool CallsEhReturn;
            bool CallsEhDwarf;
            bool EhDataRegFI[2];
    };
}

#endif