#ifndef LLVM_LIB_TARGET_CPU0_CPU0SEISELDAGTODAG_H
#define LLVM_LIB_TARGET_CPU0_CPU0SEISELDAGTODAG_H

#include "Cpu0ISelDAGToDAG.h"

namespace llvm{
    class Cpu0SEDAGToDAGISel : public Cpu0DAGToDAGISel
    {
        public:
            explicit Cpu0SEDAGToDAGISel(Cpu0TargetMachine &TM,CodeGenOpt::Level OL)
            : Cpu0DAGToDAGISel(TM,OL)
            {

            }

        private:
            bool runOnMachineFunction(MachineFunction &MF) override;

            void selectAddESubE(unsigned MOp,SDValue InFlag,SDValue CmpLHS,const SDLoc &DL,SDNode *Node) const;

            bool trySelect(SDNode *Node) override;
            void processFunctionAfterISel(MachineFunction &MF) override;
            std::pair<SDNode *, SDNode *> selectMULT(SDNode *N, unsigned Opc,const SDLoc &DL, EVT Ty,bool HasLo, bool HasHi);

    };

    FunctionPass *createCpu0SEISelDAG(Cpu0TargetMachine &TM,CodeGenOpt::Level OptLevel);
}



#endif