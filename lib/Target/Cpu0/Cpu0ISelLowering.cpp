#include"Cpu0ISelLowering.h"
#include"Cpu0MachineFunctionInfo.h"
#include"Cpu0TargetMachine.h"
#include"Cpu0TargetObjectFile.h"
#include"Cpu0Subtarget.h"

#include"llvm/ADT/Statistic.h"
#include"llvm/CodeGen/CallingConvLower.h"
#include"llvm/CodeGen/MachineFrameInfo.h"
#include"llvm/CodeGen/MachineFunction.h"
#include"llvm/CodeGen/MachineInstrBuilder.h"
#include"llvm/CodeGen/MachineRegionInfo.h"
#include"llvm/CodeGen/SelectionDAG.h"
#include"llvm/CodeGen/ValueTypes.h"
#include"llvm/IR/CallingConv.h"
#include"llvm/IR/DerivedTypes.h"
#include"llvm/IR/GlobalVariable.h"
#include"llvm/Support/CommandLine.h"
#include"llvm/Support/Debug.h"
#include"llvm/Support/ErrorHandling.h"
#include"llvm/Support/raw_ostream.h"


using namespace llvm;

#define DEBUG_TYPE "cpu0-lower"

const char *Cpu0TargetLowering::getTargetNodeName(unsigned Opcode) const{
    switch (Opcode)
    {
    case Cpu0ISD::JmpLink        :  return "Cpu0ISD::JmpLink";
    case Cpu0ISD::TailCall       :  return "Cpu0ISD::TailCall";
    case Cpu0ISD::Hi             :  return "Cpu0ISD::Hi";
    case Cpu0ISD::Lo             :  return "Cpu0ISD::Lo";
    case Cpu0ISD::GPRel          :  return "Cpu0ISD::GPRel";
    case Cpu0ISD::Ret            :  return "Cpu0ISD::Ret";
    case Cpu0ISD::EH_RETURN      :  return "Cpu0ISD::EH_RETURN";
    case Cpu0ISD::DivRem         :  return "Cpu0ISD::DivRem";
    case Cpu0ISD::DivRemU        :  return "Cpu0ISD::DivRemU";
    case Cpu0ISD::Wrapper        :  return "Cpu0ISD::Wrapper";    
    default                      :  return NULL;
    }
}

Cpu0TargetLowering::Cpu0TargetLowering(const Cpu0TargetMachine &TM,
                                       const Cpu0Subtarget &STI) : TargetLowering(TM),Subtarget(STI),ABI(TM.getABI())
{}

const Cpu0TargetLowering* Cpu0TargetLowering::create(const Cpu0TargetMachine &TM,
                                                     const Cpu0Subtarget &STI)
{
    return createCpu0SETargetLowering(TM,STI);                                                        
}

#include "Cpu0GenCallingConv.inc"

SDValue Cpu0TargetLowering::LowerFormalArguments(SDValue Chain,CallingConv::ID,
                                                 bool IsVarArg,
                                                 const SmallVectorImpl<ISD::InputArg> &Ins,
                                                 const SDLoc &DL,
                                                 SelectionDAG &DAG,
                                                 SmallVectorImpl<SDValue> &InVals) const
{
    return Chain;
}

SDValue
Cpu0TargetLowering::LowerReturn(SDValue Chain,
                                CallingConv::ID CallConv, bool IsVarArg,
                                const SmallVectorImpl<ISD::OutputArg> &Outs,
                                const SmallVectorImpl<SDValue> &OutsVals,
                                const SDLoc &DL, SelectionDAG &DAG) const {
  return DAG.getNode(Cpu0ISD::Ret, DL, MVT::Other, Chain,
                     DAG.getRegister(Cpu0::LR, MVT::i32));
}