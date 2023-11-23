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
            bool trySelect(SDNode *Node) override;
            void processFunctionAfterISel(MachineFunction &MF) override;

    };

    FunctionPass *createCpu0SEISelDAG(Cpu0TargetMachine &TM,CodeGenOpt::Level OptLevel);
}



#endif