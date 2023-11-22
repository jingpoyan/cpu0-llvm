#include "Cpu0MachineFunctionInfo.h"
#include "Cpu0InstrInfo.h"
#include "llvm/IR/Function.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"

using namespace llvm;


bool FixGlobalBaseReg;

Cpu0MachineFunctionInfo::~Cpu0MachineFunctionInfo(){}

void Cpu0MachineFunctionInfo::anchor(){}