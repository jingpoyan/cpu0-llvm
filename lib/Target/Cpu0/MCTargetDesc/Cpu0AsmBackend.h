#ifndef LLVM_LIB_TARGET_CPU0_MCTARGETDESC_CPU0ASMBACKEND_H
#define LLVM_LIB_TARGET_CPU0_MCTARGETDESC_CPU0ASMBACKEND_H

#include "MCTargetDesc/Cpu0FixupKinds.h"
#include "llvm/ADT/Triple.h"
#include "llvm/MC/MCAsmBackend.h"

namespace llvm
{
    class MCAssembler;
    struct MCFixupKindInfo;
    class MCObjectWriter;
    class MCRegisterInfo;
    class MCSymbolELF;
    class Target;

    class Cpu0AsmBackend : public MCAsmBackend
    {
            Triple TheTriple;

            public:
                Cpu0AsmBackend(const Target &T,const MCRegisterInfo &MRI,const Triple &TT,StringRef CPU) : MCAsmBackend(TT.isLittleEndian() ? support::little : support::big),TheTriple(TT)
                {

                }
                
                std::unique_ptr<MCObjectTargetWriter> createObjectTargetWriter() const override;

                void applyFixup(const MCAssembler &Asm,const MCFixup &Fixup,const MCValue &Target,MutableArrayRef<char> Data,uint64_t Value,bool IsResolved,const MCSubtargetInfo *STI) const override;

                const MCFixupKindInfo &getFixupKindInfo(MCFixupKind Kind) const override;                

                unsigned getNumFixupKinds() const override{
                    return Cpu0::NumTargetFixupKins;
                }

                bool mayNeedRelaxation(const MCInst &Inst,const MCSubtargetInfo &STI) const override
                {
                    return false;
                }

                bool fixupNeedsRelaxation(const MCFixup &Fixup, uint64_t Value,
                                        const MCRelaxableFragment *DF,
                                        const MCAsmLayout &Layout) const override {
                llvm_unreachable("RelaxInstruction() unimplemented");
                return false;
                }

                void relaxInstruction(const MCInst &Inst,const MCSubtargetInfo &STI,MCInst &Res) const override
                {

                }

                bool writeNopData(raw_ostream &OS,uint64_t Count) const override;

    };

}
#endif