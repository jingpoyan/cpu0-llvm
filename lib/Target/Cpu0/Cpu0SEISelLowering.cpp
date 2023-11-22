#include"Cpu0MachineFunctionInfo.h"
#include"Cpu0ISelLowering.h"
#include"Cpu0SEISelLowering.h"

#include"Cpu0RegisterInfo.h"
#include"Cpu0TargetMachine.h"

#include"llvm/CodeGen/MachineInstrBuilder.h"
#include"llvm/CodeGen/MachineRegionInfo.h"
#include"llvm/IR/Intrinsics.h"
#include"llvm/Support/CommandLine.h"
#include"llvm/Support/Debug.h"
#include"llvm/Support/raw_ostream.h"
#include"llvm/CodeGen/TargetInstrInfo.h"


using namespace llvm;


#define DEBUG_TYPE "cpu0-isel"


static cl::opt<bool> EnableCpu0TailCalls("enable-cpu-tail-calls",cl::Hidden,cl::desc("CPU0: Enable tail calls."),cl::init(false));

Cpu0SETargetLowering::Cpu0SETargetLowering(const Cpu0TargetMachine &TM,
                                            const Cpu0Subtarget &STI) : Cpu0TargetLowering(TM,STI)
{
  addRegisterClass(MVT::i32, &Cpu0::CPURegsRegClass);
  computeRegisterProperties(Subtarget.getRegisterInfo());
}

SDValue Cpu0SETargetLowering::LowerOperation(SDValue Op,SelectionDAG &DAG) const{
    return Cpu0TargetLowering::LowerOperation(Op,DAG);
}

const Cpu0TargetLowering* llvm::createCpu0SETargetLowering(const Cpu0TargetMachine &TM,const Cpu0Subtarget &STI)
{
    return new Cpu0SETargetLowering(TM,STI);
}