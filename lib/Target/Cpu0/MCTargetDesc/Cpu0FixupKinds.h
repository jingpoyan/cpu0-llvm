#ifndef LLVM_LIB_TARGET_CPU0_MACTARGETDESC_CPU0FIXUPKINDS_H
#define LLVM_LIB_TARGET_CPU0_MACTARGETDESC_CPU0FIXUPKINDS_H

#include "llvm/MC/MCFixup.h"

namespace llvm{
    namespace Cpu0{
        enum Fixups
        {
            fixup_Cpu0_32 = FirstTargetFixupKind,
            fixup_Cpu0_HI16,
            fixup_Cpu0_LO16,
            fixup_Cpu0_GPREL16,
            fixup_Cpu0_GOT,
            fixup_Cpu0_GOT_HI16,
            fixup_Cpu0_GOT_LO16,
            LastTargetFixupKind,
            NumTargetFixupKins = LastTargetFixupKind - FirstTargetFixupKind
        };
    }
}

#endif