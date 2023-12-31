#include "Cpu0FrameLowering.h"

#include "Cpu0InstrInfo.h"
#include "Cpu0MachineFunctionInfo.h"
#include "Cpu0Subtarget.h"

#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Target/TargetOptions.h"


using namespace llvm;


const Cpu0FrameLowering *Cpu0FrameLowering::create(const Cpu0Subtarget &ST)
{
    return llvm::createCpu0SEFrameLowering(ST);
}

bool Cpu0FrameLowering::hasFP(const MachineFunction &MF) const{
    const MachineFrameInfo &MFI = MF.getFrameInfo();
    const TargetRegisterInfo *TRI = STI.getRegisterInfo();

    return MF.getTarget().Options.DisableFramePointerElim(MF) || MFI.hasVarSizedObjects() || MFI.isFrameAddressTaken() || TRI->needsStackRealignment(MF);
}