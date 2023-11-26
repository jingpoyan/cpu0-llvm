#include "Cpu0SEFrameLowering.h"

#include "Cpu0MachineFunctionInfo.h"
#include "Cpu0SEInstrInfo.h"
#include "Cpu0Subtarget.h"

#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineRegionInfo.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/IR/DataLayout.h" 
#include "llvm/IR/Function.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Target/TargetOptions.h"


using namespace llvm;

Cpu0SEFrameLowering::Cpu0SEFrameLowering(const Cpu0Subtarget &STI)
  : Cpu0FrameLowering(STI, STI.stackAlignment()) { }

void Cpu0SEFrameLowering::emitPrologue(MachineFunction &MF,
                                       MachineBasicBlock &MBB) const 
{
  assert(&MF.front() == &MBB && "Shrink-wrapping not yet supported");
  MachineFrameInfo &MFI    = MF.getFrameInfo();
  Cpu0MachineFunctionInfo *Cpu0FI = MF.getInfo<Cpu0MachineFunctionInfo>();

  const Cpu0SEInstrInfo &TII =
    *static_cast<const Cpu0SEInstrInfo*>(STI.getInstrInfo());
  const Cpu0RegisterInfo &RegInfo =
    *static_cast<const Cpu0RegisterInfo*>(STI.getRegisterInfo());

  MachineBasicBlock::iterator MBBI = MBB.begin();
  DebugLoc dl = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();
  Cpu0ABIInfo ABI = STI.getABI();
  unsigned SP = Cpu0::SP;
  const TargetRegisterClass *RC = &Cpu0::GPROutRegClass;


  uint64_t StackSize = MFI.getStackSize();


  if (StackSize == 0 && !MFI.adjustsStack()) return;

  MachineModuleInfo &MMI = MF.getMMI();
  const MCRegisterInfo *MRI = MMI.getContext().getRegisterInfo();

  TII.adjustStackPtr(SP, -StackSize, MBB, MBBI);

  unsigned CFIIndex = MF.addFrameInst(
    MCCFIInstruction::createDefCfaOffset(nullptr, -StackSize));
  BuildMI(MBB, MBBI, dl, TII.get(TargetOpcode::CFI_INSTRUCTION))
      .addCFIIndex(CFIIndex);

  const std::vector<CalleeSavedInfo> &CSI = MFI.getCalleeSavedInfo();
  if (CSI.size()) {
    for (unsigned i = 0; i < CSI.size(); ++i)
      ++MBBI;

    for (std::vector<CalleeSavedInfo>::const_iterator I = CSI.begin(),
           E = CSI.end(); I != E; ++I) {
      int64_t Offset = MFI.getObjectOffset(I->getFrameIdx());
      unsigned Reg = I->getReg();
      {
        unsigned CFIIndex = MF.addFrameInst(MCCFIInstruction::createOffset(
            nullptr, MRI->getDwarfRegNum(Reg, 1), Offset));
        BuildMI(MBB, MBBI, dl, TII.get(TargetOpcode::CFI_INSTRUCTION))
            .addCFIIndex(CFIIndex);
      }
    }
  }
}

void Cpu0SEFrameLowering::emitEpilogue(MachineFunction &MF,
                                       MachineBasicBlock &MBB) const 
{
  MachineBasicBlock::iterator MBBI = MBB.getLastNonDebugInstr();
  MachineFrameInfo &MFI = MF.getFrameInfo();
  Cpu0MachineFunctionInfo *Cpu0FI = MF.getInfo<Cpu0MachineFunctionInfo>();

  const Cpu0SEInstrInfo &TII =
    *static_cast<const Cpu0SEInstrInfo*>(STI.getInstrInfo());
  const Cpu0RegisterInfo &RegInfo =
    *static_cast<const Cpu0SERegisterInfo*>(STI.getRegisterInfo());

  DebugLoc dl = MBBI->getDebugLoc();
  Cpu0ABIInfo ABI = STI.getABI();
  unsigned SP = Cpu0::SP;

  uint64_t StackSize = MFI.getStackSize();

  if (!StackSize)
    return;

  TII.adjustStackPtr(SP, StackSize, MBB, MBBI);
}

const Cpu0FrameLowering *llvm::createCpu0SEFrameLowering(const Cpu0Subtarget
                                                                   &ST) {
  return new Cpu0SEFrameLowering(ST);
}


bool Cpu0SEFrameLowering::hasReservedCallFrame(const MachineFunction &MF) const {
  const MachineFrameInfo &MFI = MF.getFrameInfo();


  return isInt<16>(MFI.getMaxCallFrameSize() + getStackAlignment()) &&
    !MFI.hasVarSizedObjects();
}

static void setAliasRegs(MachineFunction &MF, BitVector &SavedRegs, unsigned Reg) {
  const TargetRegisterInfo *TRI = MF.getSubtarget().getRegisterInfo();
  for (MCRegAliasIterator AI(Reg, TRI, true); AI.isValid(); ++AI)
    SavedRegs.set(*AI);
}

void Cpu0SEFrameLowering::determineCalleeSaves(MachineFunction &MF,
                                               BitVector &SavedRegs,
                                               RegScavenger *RS) const {
  TargetFrameLowering::determineCalleeSaves(MF, SavedRegs, RS);
  //Cpu0MachineFunctionInfo *Cpu0FI = MF.getInfo<Cpu0MachineFunctionInfo>();
  //MachineRegisterInfo &MRI = MF.getRegInfo();

  if (MF.getFrameInfo().hasCalls())
    setAliasRegs(MF, SavedRegs, Cpu0::LR);

  return;
}