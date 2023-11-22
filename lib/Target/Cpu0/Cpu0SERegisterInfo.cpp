#include "Cpu0SERegisterInfo.h"


using namespace llvm;

#define DEBUG_TYPES "cpu0-reg-info"

Cpu0SERegisterInfo::Cpu0SERegisterInfo(const Cpu0Subtarget &ST) : Cpu0RegisterInfo(ST){}


const TargetRegisterClass *Cpu0SERegisterInfo::intRegClass(unsigned Size) const
{
    return &Cpu0::CPURegsRegClass;
}