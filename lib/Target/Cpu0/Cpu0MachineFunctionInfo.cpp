#include "Cpu0MachineFunctionInfo.h"
#include "Cpu0InstrInfo.h"
#include "llvm/IR/Function.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"

using namespace llvm;


bool FixGlobalBaseReg;

Cpu0MachineFunctionInfo::~Cpu0MachineFunctionInfo(){}

bool Cpu0MachineFunctionInfo::globalBaseRegFixed() const
{
    return FixGlobalBaseReg;
}

bool Cpu0MachineFunctionInfo::globalBaseRegSet() const
{
    return GlobalBaseReg;
}

unsigned Cpu0MachineFunctionInfo::getGlobalBaseReg()
{
    return GlobalBaseReg = Cpu0::GP;
}

void Cpu0MachineFunctionInfo::anchor(){}

