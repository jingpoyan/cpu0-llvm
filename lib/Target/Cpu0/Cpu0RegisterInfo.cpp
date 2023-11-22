#include "Cpu0RegisterInfo.h"

#include "Cpu0.h"
#include "Cpu0Subtarget.h"
#include "Cpu0MachineFunctionInfo.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

#define GET_REGINFO_TARGET_DESC
#include "Cpu0GenRegisterInfo.inc"

#define DEBUG_TYPE "cpu0-reg-info"

using namespace llvm;


Cpu0RegisterInfo::Cpu0RegisterInfo(const Cpu0Subtarget &ST) : Cpu0GenRegisterInfo(Cpu0::LR),Subtarget(ST){}

const MCPhysReg* Cpu0RegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const
{
    return CSR_032_SaveList;
}

const uint32_t *
Cpu0RegisterInfo::getCallPreservedMask(const MachineFunction &MF,
                                       CallingConv::ID) const {
  return CSR_032_RegMask;
}

BitVector Cpu0RegisterInfo::getReservedRegs(const MachineFunction &MF) const
{
    static const uint16_t ReserverdCPURegs[] = {
        Cpu0::ZERO,Cpu0::AT,Cpu0::SP,Cpu0::LR,Cpu0::PC
    };
    
    BitVector Reserved(getNumRegs());

    for(unsigned I=0;I<array_lengthof(ReserverdCPURegs);++I)
    {
        Reserved.set(ReserverdCPURegs[I]);
    }

    return Reserved;
}

void Cpu0RegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,int SPAdj,unsigned FIOperandNum,RegScavenger *RS) const
{

}

bool Cpu0RegisterInfo::requiresRegisterScavenging(const MachineFunction &MF) const
{
    return true;
}

bool Cpu0RegisterInfo::trackLivenessAfterRegAlloc(const MachineFunction &MF) const 
{
    return true;
}

unsigned Cpu0RegisterInfo::
getFrameRegister(const MachineFunction &MF) const {
  const TargetFrameLowering *TFI = MF.getSubtarget().getFrameLowering();
  return TFI->hasFP(MF) ? (Cpu0::FP) :
                          (Cpu0::SP);
}