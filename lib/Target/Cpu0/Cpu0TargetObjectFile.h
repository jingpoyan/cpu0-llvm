#ifndef LLVM_LIB_TARGET_CPU0_CPU0TARGETOBJECTFILE_H
#define LLVM_LIB_TARGET_CPU0_CPU0TARGETOBJECTFILE_H

#include "Cpu0TargetMachine.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"

namespace llvm{
    class Cpu0TargetMachine;
    class Cpu0TargetObjectFile : public TargetLoweringObjectFileELF {
        MCSection *SmallDataSection;
        MCSection *SmallBSSSection;
        const Cpu0TargetMachine* TM;

        bool IsGlobalInSmallSection(const GlobalObject *GO,
                              const TargetMachine &TM, SectionKind Kind) const;
        bool IsGlobalInSmallSectionImpl(const GlobalObject *GO,
                                        const TargetMachine &TM) const;
        public:
            void Initialize(MCContext &Ctx,const TargetMachine &TM) override;
            bool IsGlobalInSmallSection(const GlobalObject *GO,
                              const TargetMachine &TM) const;

            MCSection *SelectSectionForGlobal(const GlobalObject *GO, SectionKind Kind,
                                                const TargetMachine &TM) const override;
    };
}
#endif