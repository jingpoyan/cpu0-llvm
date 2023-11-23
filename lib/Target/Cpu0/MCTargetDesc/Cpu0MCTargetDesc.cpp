#include "Cpu0MCTargetDesc.h"
#include "Cpu0MCAsmInfo.h"
#include "InstPrinter/Cpu0InstPrinter.h"

#include "llvm/MC/MachineLocation.h"
#include "llvm/MC/MCELFStreamer.h"
#include "llvm/MC/MCInstrAnalysis.h"
#include "llvm/MC/MCInstPrinter.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;


#define GET_INSTRINFO_MC_DESC
#include "Cpu0GenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "Cpu0GenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "Cpu0GenRegisterInfo.inc"


static StringRef selectionCpu0ArchFeature(const Triple &TT,StringRef CPU)
{
    std::string Cpu0ArchFeature;
    if(CPU.empty() || CPU == "generic")
    {
        if(TT.getArch() == Triple::cpu0 || TT.getArch() == Triple::cpu0el)
        {
            if(CPU.empty() || CPU == "cpu032II")
            {
                Cpu0ArchFeature = "+cpu032II";
            }else if(CPU == "cpu032I")
            {
                Cpu0ArchFeature = "+cpu032I";
            }
        }
    }
    return Cpu0ArchFeature;
}

static MCInstrInfo *createCpu0MCInstrInfo()
{
    MCInstrInfo *X = new MCInstrInfo();
    InitCpu0MCInstrInfo(X);
    return X;
}

static MCRegisterInfo *createCpu0MCRegisterInfo(const Triple &TT)
{
    MCRegisterInfo *X = new MCRegisterInfo();
    InitCpu0MCRegisterInfo(X,Cpu0::SW);
    return X;
}

static MCSubtargetInfo *createCpu0MCSubtargetInfo(const Triple &TT,
                                                  StringRef CPU, StringRef FS) {
  std::string ArchFS = selectionCpu0ArchFeature(TT, CPU);
  if (!FS.empty()) {
    if (!ArchFS.empty()) {
      ArchFS = ArchFS + "," + FS.str();
    } else {
      ArchFS = FS;
    }
  }
  return createCpu0MCSubtargetInfoImpl(TT, CPU, ArchFS);
}

static MCAsmInfo *createCpu0MCAsmInfo(const MCRegisterInfo &MRI,const Triple &TT)
{
    MCAsmInfo *MAI = new Cpu0MCAsmInfo(TT);

    unsigned SP = MRI.getDwarfRegNum(Cpu0::SP,true);
    MCCFIInstruction Inst = MCCFIInstruction::createDefCfa(nullptr,SP,0);
    MAI->addInitialFrameState(Inst);

    return MAI;
}

static MCInstPrinter *createCpu0MCInstPrinter(const Triple &TT,unsigned SyntaxVariant,const MCAsmInfo &MAI,const MCInstrInfo &MII,const MCRegisterInfo &MRI)
{
    return new Cpu0InstPrinter(MAI,MII,MRI);
}

namespace {
    class Cpu0MCInstrAnalysis : public MCInstrAnalysis
    {
        public:
            Cpu0MCInstrAnalysis(const MCInstrInfo *Info) : MCInstrAnalysis(Info)
            {

            }
    };
}

static MCInstrAnalysis *createCpu0MCInstrAnalysis(const MCInstrInfo *Info)
{
    return new Cpu0MCInstrAnalysis(Info);
}


extern "C" void LLVMInitializeCpu0TargetMC()
{
    Target &TheCpu0Target = getTheCpu0Target();
    Target &TheCpu0elTarget = getTheCpu0elTarget();
    for (Target *T : {&TheCpu0Target, &TheCpu0elTarget})
    {
        RegisterMCAsmInfoFn X(*T, createCpu0MCAsmInfo);

        TargetRegistry::RegisterMCInstrInfo(*T, createCpu0MCInstrInfo);

        TargetRegistry::RegisterMCRegInfo(*T, createCpu0MCRegisterInfo);

        TargetRegistry::RegisterMCSubtargetInfo(*T, createCpu0MCSubtargetInfo);

        TargetRegistry::RegisterMCInstrAnalysis(*T, createCpu0MCInstrAnalysis);

        TargetRegistry::RegisterMCInstPrinter(*T, createCpu0MCInstPrinter);
    }
}