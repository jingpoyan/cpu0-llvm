#ifndef LLVM_LIB_TARGET_CPU0_INSTRPRINTER_CPU0INSTRPRINTER_H
#define LLVM_LIB_TARGET_CPU0_INSTRPRINTER_CPU0INSTRPRINTER_H


#include "llvm/MC/MCInstPrinter.h"

namespace llvm
{
    class TargetMachine;

    class Cpu0InstPrinter : public MCInstPrinter
    {
        public:
            Cpu0InstPrinter(const MCAsmInfo &MAI,const MCInstrInfo &MII,const MCRegisterInfo &MRI) : MCInstPrinter(MAI,MII,MRI) {}
            
            void printInstruction(const MCInst *MI,raw_ostream &O);

            static const char *getRegisterName(unsigned RegNo);

            void printRegName(raw_ostream &OS,unsigned RegNo) const override;
            void printInst(const MCInst *MI,raw_ostream &O,StringRef Annot,const MCSubtargetInfo &STI) override;

            bool printAliasInstr(const MCInst *MI,raw_ostream &OS);
            void printCustomAliasOperand(const MCInst *MI,unsigned OpIdx,unsigned PrintMethodIdx,raw_ostream &O);

        private:
            void printOperand(const MCInst *MI,unsigned OpNo,raw_ostream &O);
            void printUnsignedImm(const MCInst *MI,int opNum,raw_ostream &O);
            void printMemOperand(const MCInst *MI,int opNum,raw_ostream &O);

    };
}



#endif