#include "Cpu0AsmPrinter.h"

#include "InstPrinter/Cpu0InstPrinter.h"
#include "MCTargetDesc/Cpu0BaseInfo.h"
#include "Cpu0.h"
#include "Cpu0InstrInfo.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/ADT/Twine.h"
#include "llvm/CodeGen/MachineConstantPool.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineMemOperand.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Mangler.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Target/TargetLoweringObjectFile.h"
#include "llvm/Target/TargetOptions.h"

using namespace llvm;

#define DBEUG_TYPE "cpu0-asm-printer"

bool Cpu0AsmPrinter::runOnMachineFunction(MachineFunction &MF) {
  Cpu0MFI = MF.getInfo<Cpu0MachineFunctionInfo>();
  AsmPrinter::runOnMachineFunction(MF);
  return true;
}
void Cpu0AsmPrinter::EmitInstruction(const MachineInstr *MI)
{
    if(MI->isDebugValue())
    {
        SmallString<128> Str;
        raw_svector_ostream OS(Str);

        PrinterDebugValueComment(MI,OS);
        return;
    }

    MachineBasicBlock::const_instr_iterator I = MI->getIterator();
    MachineBasicBlock::const_instr_iterator E = MI->getParent()->instr_end();

    do{
        if(I->isPseudo())
            llvm_unreachable("Pseudo opcode found in EmitInstruction()");

            MCInst TmpInstr0;
            MCInstLowering.Lower(&*I,TmpInstr0);
            OutStreamer->EmitInstruction(TmpInstr0,getSubtargetInfo());
    }while((++I != E)&& I->isInsideBundle());
}

void Cpu0AsmPrinter::printSavedRegsBitmask(raw_ostream &O)
{
    unsigned CPUBitmask = 0;
    int CPUTopSavedRegOff;

    const MachineFrameInfo &MFI = MF->getFrameInfo();
    const TargetRegisterInfo *TRI = MF->getSubtarget().getRegisterInfo();
    const std::vector<CalleeSavedInfo> &CSI = MFI.getCalleeSavedInfo();
    unsigned CPURegSize = TRI->getRegSizeInBits(Cpu0::CPURegsRegClass)/8;

    for(const auto &I:CSI)
    {
        unsigned Reg = I.getReg();
        unsigned RegNum = TRI->getEncodingValue(Reg);
        CPUBitmask |= (1 << RegNum);
    }

    CPUTopSavedRegOff = CPUBitmask ? -CPURegSize : 0;
    
    O << "\t.mask \t";printHex32(CPUBitmask,O);
    O << ',' << CPUTopSavedRegOff << '\n';
}


void Cpu0AsmPrinter::printHex32(unsigned Value, raw_ostream &O) {
  O << "0x";
  for (int i = 7; i >= 0; i--)
    O.write_hex((Value & (0xF << (i*4))) >> (i*4));
}

void Cpu0AsmPrinter::emitFrameDirective() {
  const TargetRegisterInfo &RI = *MF->getSubtarget().getRegisterInfo();

  unsigned stackReg  = RI.getFrameRegister(*MF);
  unsigned returnReg = RI.getRARegister();
  unsigned stackSize = MF->getFrameInfo().getStackSize();

  if (OutStreamer->hasRawTextSupport())
    OutStreamer->EmitRawText("\t.frame\t$" +
           StringRef(Cpu0InstPrinter::getRegisterName(stackReg)).lower() +
           "," + Twine(stackSize) + ",$" +
           StringRef(Cpu0InstPrinter::getRegisterName(returnReg)).lower());
}

const char *Cpu0AsmPrinter::getCurrentABIString() const {
  switch (static_cast<Cpu0TargetMachine &>(TM).getABI().GetEnumValue()) {
  case Cpu0ABIInfo::ABI::O32:  return "abiO32";
  case Cpu0ABIInfo::ABI::S32:  return "abiS32";
  default: llvm_unreachable("Unknown Cpu0 ABI");
  }
}


void Cpu0AsmPrinter::EmitFunctionEntryLabel() {
  if (OutStreamer->hasRawTextSupport())
    OutStreamer->EmitRawText("\t.ent\t" + Twine(CurrentFnSym->getName()));
  OutStreamer->EmitLabel(CurrentFnSym);
}

void Cpu0AsmPrinter::EmitFunctionBodyStart() {
  MCInstLowering.Initialize(&MF->getContext());

  emitFrameDirective();

  if (OutStreamer->hasRawTextSupport()) {
    SmallString<128> Str;
    raw_svector_ostream OS(Str);
    printSavedRegsBitmask(OS);
    OutStreamer->EmitRawText(OS.str());
    OutStreamer->EmitRawText(StringRef("\t.set\tnoreorder"));
    OutStreamer->EmitRawText(StringRef("\t.set\tnomacro"));
    if (Cpu0MFI->getEmitNOAT())
      OutStreamer->EmitRawText(StringRef("\t.set\tnoat"));
  }
}

void Cpu0AsmPrinter::EmitFunctionBodyEnd() {
  // There are instruction for this macros, but they must
  // always be at the function end, and we can't emit and
  // break with BB logic.
  if (OutStreamer->hasRawTextSupport()) {
    if (Cpu0MFI->getEmitNOAT())
      OutStreamer->EmitRawText(StringRef("\t.set\tat"));
    OutStreamer->EmitRawText(StringRef("\t.set\tmacro"));
    OutStreamer->EmitRawText(StringRef("\t.set\treorder"));
    OutStreamer->EmitRawText("\t.end\t" + Twine(CurrentFnSym->getName()));
  }
}


void Cpu0AsmPrinter::EmitStartOfAsmFile(Module &M) {
  // FIXME: Use SwitchSection.

  // Tell the assembler which ABI we are using
  if (OutStreamer->hasRawTextSupport())
    OutStreamer->EmitRawText("\t.section .mdebug." +
                            Twine(getCurrentABIString()));

  // return to previous section
  if (OutStreamer->hasRawTextSupport())
    OutStreamer->EmitRawText(StringRef("\t.previous"));
}



void Cpu0AsmPrinter::PrinterDebugValueComment(const MachineInstr *MI,
                                           raw_ostream &OS) {
  // TODO: implement
  OS << "PrinterDebugValueComment()";
}

// Force static initialization.
extern "C" void LLVMInitializeCpu0AsmPrinter() {
  RegisterAsmPrinter<Cpu0AsmPrinter> X(getTheCpu0Target());
  RegisterAsmPrinter<Cpu0AsmPrinter> Y(getTheCpu0elTarget());
}