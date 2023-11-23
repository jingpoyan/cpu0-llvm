#ifndef LLVM_LIB_TARGET_CPU0_CPU0ISELDAGTODAG_H
#define LLVM_LIB_TARGET_CPU0_CPU0ISELDAGTODAG_H

#include "Cpu0.h"
#include "Cpu0Subtarget.h"
#include "Cpu0TargetMachine.h"

#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/Debug.h"


namespace llvm{
    class Cpu0DAGToDAGISel : public SelectionDAGISel
    {
        public:
            explicit Cpu0DAGToDAGISel(Cpu0TargetMachine &TM,CodeGenOpt::Level OL)
            : SelectionDAGISel(TM,OL),Subtarget(nullptr)
            {

            }
            bool runOnMachineFunction(MachineFunction &MF) override;
        
        protected:
            const Cpu0Subtarget *Subtarget;
        private:
            #include "Cpu0GenDAGISel.inc"

            const Cpu0TargetMachine &getTargetMachine()
            {
                return static_cast<const Cpu0TargetMachine &>(TM);
            }

            void Select(SDNode *N) override;

            virtual bool trySelect(SDNode *Node) = 0;

            bool SelectAddr(SDNode *Parent,SDValue N,SDValue &Base,SDValue &Offset);

            inline SDValue getImm(const SDNode *Node,unsigned Imm)
            {
                return CurDAG->getTargetConstant(Imm,SDLoc(Node),Node->getValueType(0));
            }
            
            virtual void processFunctionAfterISel(MachineFunction &MF) = 0;
    };
}


#endif