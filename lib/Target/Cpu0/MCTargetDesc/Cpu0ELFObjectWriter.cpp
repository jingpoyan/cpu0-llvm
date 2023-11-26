#include "MCTargetDesc/Cpu0BaseInfo.h"
#include "MCTargetDesc/Cpu0FixupKinds.h"
#include "MCTargetDesc/Cpu0MCTargetDesc.h"

#include "llvm/BinaryFormat/ELF.h"
#include "llvm/MC/MCAssembler.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCSection.h"
#include "llvm/MC/MCValue.h"
#include "llvm/Support/ErrorHandling.h"
#include <list>


using namespace llvm;


namespace {
    class Cpu0ELFObjectWriter : public MCELFObjectTargetWriter
    {
      public:
        Cpu0ELFObjectWriter(uint8_t OSABI);
        ~Cpu0ELFObjectWriter() override;
        unsigned getRelocType(MCContext &Ctx,const MCValue &Target,const MCFixup &Fixup,bool IsPCRel) const override;
        bool needsRelocateWithSymbol(const MCSymbol &Sym,unsigned Type) const override;
    };
}

Cpu0ELFObjectWriter::Cpu0ELFObjectWriter(uint8_t OSABI) : MCELFObjectTargetWriter(false,OSABI,ELF::EM_CPU0,false)
{

}

Cpu0ELFObjectWriter::~Cpu0ELFObjectWriter(){}

unsigned Cpu0ELFObjectWriter::getRelocType(MCContext &Ctx,const MCValue &Target,const MCFixup &Fixup,bool IsPCRel) const
{
    unsigned Type = (unsigned)ELF::R_CPU0_NONE;
    unsigned Kind = (unsigned)Fixup.getKind();
    switch(Kind) {
    default:
        llvm_unreachable("invalid fixup kind!");
    case FK_Data_4:
        Type = ELF::R_CPU0_32;
        break;
    case Cpu0::fixup_Cpu0_32:
        Type = ELF::R_CPU0_32;
        break;
    case Cpu0::fixup_Cpu0_GPREL16:
        Type = ELF::R_CPU0_GPREL16;
        break;
    case Cpu0::fixup_Cpu0_GOT:
        Type = ELF::R_CPU0_GOT16;
        break;
    case Cpu0::fixup_Cpu0_HI16:
        Type = ELF::R_CPU0_HI16;
        break;
    case Cpu0::fixup_Cpu0_LO16:
        Type = ELF::R_CPU0_LO16;
        break;
    case Cpu0::fixup_Cpu0_GOT_HI16:
        Type = ELF::R_CPU0_GOT_HI16;
        break;
    case Cpu0::fixup_Cpu0_GOT_LO16:
        Type = ELF::R_CPU0_GOT_LO16;
        break;
    }

    return Type;
}

bool Cpu0ELFObjectWriter::needsRelocateWithSymbol(const MCSymbol &Sym,unsigned Type) const
{
    switch (Type)
    {
    default:
        return true;
    case ELF::R_CPU0_GOT16:
        return true;
    case ELF::R_CPU0_HI16:
    case ELF::R_CPU0_LO16:
    case ELF::R_CPU0_32:
        return true;
    case ELF::R_CPU0_GPREL16:
        return false;
    }
}

std::unique_ptr<MCObjectTargetWriter> llvm::createCpu0ELFObjectWriter(const Triple &TT)
{
    uint8_t OSABI = MCELFObjectTargetWriter::getOSABI(TT.getOS());
    return llvm::make_unique<Cpu0ELFObjectWriter>(OSABI);
}