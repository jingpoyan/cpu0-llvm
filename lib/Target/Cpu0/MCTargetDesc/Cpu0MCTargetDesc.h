#ifndef LLVM_LIB_TARGET_CPU0_MCTARGETDESC_CPU0_MCTARGETDESC_H
#define LLVM_LIB_TARGET_CPU0_MCTARGETDESC_CPU0_MCTARGETDESC_H


#include "llvm/Support/DataTypes.h"
#include <memory>

namespace llvm {
    class Target;
    class Triple;
    class MCAsmBackend;
    class MCCodeEmitter;
    class MCContext;
    class MCInstrInfo;
    class MCObjectTargetWriter;
    class MCTargetOptions;
    class MCRegisterInfo;
    class MCSubtargetInfo;
    class StringRef;

    class Target;
    class raw_ostream;
    class Triple;
    class raw_pwrite_stream;

    Target &getTheCpu0Target();
    Target &getTheCpu0elTarget();

    MCCodeEmitter *createCpu0MCCodeEmitterEB(const MCInstrInfo &MCII,
                                            const MCRegisterInfo &MRI,
                                            MCContext &Ctx);
    MCCodeEmitter *createCpu0MCCodeEmitterEL(const MCInstrInfo &MCII,
                                            const MCRegisterInfo &MRI,
                                            MCContext &Ctx);

    MCAsmBackend *createCpu0AsmBackendEB32(const Target &T,
                                            const MCSubtargetInfo &STI,
                                            const MCRegisterInfo &MRI,
                                            const MCTargetOptions &Options);
    MCAsmBackend *createCpu0AsmBackendEL32(const Target &T,
                                            const MCSubtargetInfo &STI,
                                            const MCRegisterInfo &MRI,
                                            const MCTargetOptions &Options);

    std::unique_ptr<MCObjectTargetWriter> createCpu0ELFObjectWriter(const Triple &TT);
}

#define GET_REGINFO_ENUM
#include "Cpu0GenRegisterInfo.inc"

#define GET_INSTRINFO_ENUM
#include "Cpu0GenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "Cpu0GenSubtargetInfo.inc"


#endif