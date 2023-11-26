#include "Cpu0TargetObjectFile.h"
#include "Cpu0Subtarget.h"
#include "Cpu0TargetMachine.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCSectionELF.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/Target/TargetMachine.h"


using namespace llvm;


static cl::opt<unsigned> SSThreshold("cpu0-ssection-threshold",cl::Hidden,cl::desc("Small data and bss section threshold size (default=8)"),cl::init(8));


void Cpu0TargetObjectFile::Initialize(MCContext &Ctx,const TargetMachine &TM)
{
    TargetLoweringObjectFileELF::Initialize(Ctx,TM);
    InitializeELF(TM.Options.UseInitArray);

    SmallDataSection = getContext().getELFSection(
        ".sdata",ELF::SHT_PROGBITS,ELF::SHF_WRITE | ELF::SHF_ALLOC
    );

    SmallBSSSection = getContext().getELFSection(
        ".sbss",ELF::SHT_NOBITS,ELF::SHF_WRITE | ELF::SHF_ALLOC
    );
    this->TM = &static_cast<const Cpu0TargetMachine &>(TM);
}

static bool IsInSmallSection(uint64_t Size) {
  return Size > 0 && Size <= SSThreshold;
}

bool Cpu0TargetObjectFile::
IsGlobalInSmallSection(const GlobalObject *GO, const TargetMachine &TM) const {
  if (GO->isDeclaration() || GO->hasAvailableExternallyLinkage())
    return IsGlobalInSmallSectionImpl(GO, TM);

  return IsGlobalInSmallSection(GO, TM, getKindForGlobal(GO, TM));
}

bool Cpu0TargetObjectFile::
IsGlobalInSmallSection(const GlobalObject *GO, const TargetMachine &TM,
                       SectionKind Kind) const {
  return (IsGlobalInSmallSectionImpl(GO, TM) &&
          (Kind.isData() || Kind.isBSS() || Kind.isCommon() ||
           Kind.isReadOnly()));
}

bool Cpu0TargetObjectFile::
IsGlobalInSmallSectionImpl(const GlobalObject *GO,
                           const TargetMachine &TM) const {
  const Cpu0Subtarget &Subtarget =
      *static_cast<const Cpu0TargetMachine &>(TM).getSubtargetImpl();

  if (!Subtarget.useSmallSection())
    return false;

  const GlobalVariable *GVA = dyn_cast<GlobalVariable>(GO);
  if (!GVA)
    return false;

  Type *Ty = GVA->getValueType();
  return IsInSmallSection(GVA->getParent()->getDataLayout().getTypeAllocSize(Ty));
}

MCSection *Cpu0TargetObjectFile::
SelectSectionForGlobal(const GlobalObject *GO, SectionKind Kind,
                       const TargetMachine &TM) const {
  if (Kind.isBSS() && IsGlobalInSmallSection(GO, TM, Kind))
    return SmallBSSSection;
  if (Kind.isData() && IsGlobalInSmallSection(GO, TM, Kind))
    return SmallDataSection;
  if (Kind.isReadOnly() && IsGlobalInSmallSection(GO, TM, Kind))
    return SmallDataSection;

  return TargetLoweringObjectFileELF::SelectSectionForGlobal(GO, Kind, TM);
}