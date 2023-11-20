#include "Cpu0TargetMachine.h"
#include "Cpu0.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/Support/TargetRegistry.h"


using namespace llvm;

#define DBEUG_TYPE "cpu0"

extern "C" void LLVMInitializeCpu0Target()
{
    
}