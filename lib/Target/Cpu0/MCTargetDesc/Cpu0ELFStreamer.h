#ifndef LLVM_LIB_TARGET_CPU0_MCTARGETDESC_CPU0ELFSTREAMER_H
#define LLVM_LIB_TARGET_CPU0_MCTARGETDESC_CPU0ELFSTREAMER_H

#include "llvm/ADT/SmallVector.h"
#include "llvm/MC/MCELFStreamer.h"


#include <memory>

namespace llvm{
    class MCAsmBackend;
    class MCCodeEmitter;
    class MCContext;
    class MCSubtargetInfo;

    class Cpu0ELFStreamer : public MCELFStreamer{
        public:
            Cpu0ELFStreamer(MCContext &Context,std::unique_ptr<MCAsmBackend> MAB,std::unique_ptr<MCObjectWriter> OW,std::unique_ptr<MCCodeEmitter> Emitter);
    };

    MCELFStreamer *createCpu0ELFStreamer(MCContext &Context,std::unique_ptr<MCAsmBackend> MAB,std::unique_ptr<MCObjectWriter> OW,std::unique_ptr<MCCodeEmitter> Emitter,bool RelaxAll);
}


#endif