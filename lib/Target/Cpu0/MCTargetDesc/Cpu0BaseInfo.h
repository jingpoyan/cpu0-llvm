#ifndef LLVM_LIB_TARGET_CPU0_MCTARGETDESC_CPU0BASEINFO_H
#define LLVM_LIB_TARGET_CPU0_MCTARGETDESC_CPU0BASEINFO_H

#include "Cpu0MCTargetDesc.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/Support/DataTypes.h"
#include "llvm/Support/ErrorHandling.h"


namespace llvm{
    namespace Cpu0II
    {
        enum TOF
        {
            MO_NO_FLAG,
            MO_GOT_CALL,
            MO_GPREL,
            NO_ABS_HI,
            NO_ABS_LO,
            NO_GOT_HI16,
            NO_GOT_LO16
        };

        enum {
            Pseudo = 0,
            FrmR = 1,
            FrmI = 2,
            FrmJ = 3,
            FrmOther = 4,
            FrmMask =15
        };
    }
}


#endif