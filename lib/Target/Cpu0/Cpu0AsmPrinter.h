#ifndef LLVM_LIB_TARGET_CPU0_CPU0ASMPRINTER_H
#define LLVM_LIB_TARGET_CPU0_CPU0ASMPRINTER_H

#include "Cpu0MachineFunctionInfo.h"
#include "Cpu0MCInstLower.h"
#include "Cpu0Subtarget.h"
#include "Cpu0TargetMachine.h"

#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/Support/Compiler.h"
#include "llvm/Target/TargetMachine.h"


namespace llvm
{
    class MCStreamer;
    class MachineInstr;
    class MachineBasicBlock;
    class Module;
    class raw_ostream;


    class LLVM_LIBRARY_VISIBILITY Cpu0AsmPrinter : public AsmPrinter
    {
        void EmitInstrWithMacroNoAT(const MachineInstr *MI);
        private:
            bool lowerOperand(const MachineOperand &MO,MCOperand &MCOp);
        public:
            const Cpu0Subtarget *Subtarget;
            const Cpu0MachineFunctionInfo* Cpu0MFI;
            Cpu0MCInstLower MCInstLowering;

            explicit Cpu0AsmPrinter(TargetMachine &TM,
                                    std::unique_ptr<MCStreamer> Streamer)
                : AsmPrinter(TM, std::move(Streamer)),
                MCInstLowering(*this) {
                Subtarget = static_cast<Cpu0TargetMachine &>(TM).getSubtargetImpl();
            }

            virtual StringRef getPassName() const override
            {
                return "Cpu0 Assembly Printer";
            }

            virtual bool runOnMachineFunction(MachineFunction &MF) override;

            void EmitInstruction(const MachineInstr* MI) override;
            void printSavedRegsBitmask(raw_ostream &O);
            void printHex32(unsigned int Value,raw_ostream &O);
            void emitFrameDirective();
            const char *getCurrentABIString() const;
            void EmitFunctionEntryLabel() override;
            void EmitFunctionBodyStart() override;
            void EmitFunctionBodyEnd() override;
            void EmitStartOfAsmFile(Module &M) override;
            void PrinterDebugValueComment(const MachineInstr *MI, raw_ostream &OS);
    };
}

#endif