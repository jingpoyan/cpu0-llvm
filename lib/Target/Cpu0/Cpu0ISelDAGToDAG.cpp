#include "Cpu0ISelDAGToDAG.h"
#include "Cpu0.h"

#include "Cpu0MachineFunctionInfo.h"
#include "Cpu0RegisterInfo.h"
#include "Cpu0SEISelDAGToDAG.h"
#include "Cpu0TargetMachine.h"

#include "llvm/CodeGen/MachineConstantPool.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/CodeGen/SelectionDAGNodes.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"

using namespace llvm;

#define DEBUG_TYPE "cpu0-isel"

bool Cpu0DAGToDAGISel::runOnMachineFunction(MachineFunction &MF)
{
    bool Ret = SelectionDAGISel::runOnMachineFunction(MF);
    return Ret;
}

bool Cpu0DAGToDAGISel::SelectAddr(SDNode *Parent,SDValue Addr,SDValue &Base,SDValue &Offset)
{
    EVT ValTy = Addr.getValueType();
    SDLoc DL(Addr);


    const LSBaseSDNode *LS = 0;
    
    if(Parent && (LS = dyn_cast<LSBaseSDNode>(Parent)))
    {
        EVT VT = LS->getMemoryVT();

        if(VT.getSizeInBits()/8 > LS->getAlignment())
        {
            assert(false && "Unaligned loads/stores not supported for this type.");
            if(VT == MVT::f32)
                return false;
        }
    }

    if(FrameIndexSDNode *FIN = dyn_cast<FrameIndexSDNode>(Addr))
    {
        Base = CurDAG->getTargetFrameIndex(FIN->getIndex(),ValTy);
        Offset = CurDAG->getTargetConstant(0,DL,ValTy);
        return true;
    }

    Base = Addr;
    Offset = CurDAG->getTargetConstant(0,DL,ValTy);
    return true;
}

void Cpu0DAGToDAGISel::Select(SDNode *Node)
{
    unsigned Opcode = Node->getOpcode();

    LLVM_DEBUG(errs()<<"Selecting: ";Node->dump(CurDAG);errs()<<"\b");

    if(Node->isMachineOpcode())
    {
        LLVM_DEBUG(errs()<<"==";Node->dump(CurDAG);errs()<<"\n");
        Node->setNodeId(-1);
        return;
    }

    if(trySelect(Node))
    {
        return;
    }

    switch (Opcode)
    {
    default:
        break;
    }
    SelectCode(Node);
}