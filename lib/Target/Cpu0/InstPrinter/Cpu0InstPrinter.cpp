#include "Cpu0InstPrinter.h"
#include "Cpu0InstrInfo.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;


#define PRINT_ALIAS_INSTR
#include "Cpu0GenAsmWriter.inc"


void Cpu0InstPrinter::printRegName(raw_ostream &OS,unsigned RegNo) const {
    OS << '$' << StringRef(getRegisterName(RegNo)).lower();
}

void Cpu0InstPrinter::printInst(const MCInst *MI,raw_ostream &O,StringRef Annot,const MCSubtargetInfo &STI)
{
    if(!printAliasInstr(MI,O))
        printInstruction(MI,O);
    printAnnotation(O,Annot);
}

void Cpu0InstPrinter::printOperand(const MCInst* MI,unsigned OpNo,raw_ostream &O)
{
    const MCOperand &Op = MI->getOperand(OpNo);
    if(Op.isReg())
    {
        printRegName(O,Op.getReg());
        return;
    }

    if(Op.isImm())
    {
        O << Op.getImm();
        return;
    }

    assert(Op.isExpr() && "unknown operand kind in printOperand");
    Op.getExpr()->print(O,&MAI,true);
}


void Cpu0InstPrinter::printUnsignedImm(const MCInst *MI,int OpNum,raw_ostream &O)
{
    const MCOperand &MO = MI->getOperand(OpNum);
    if(MO.isImm())
        O << (unsigned short int)MO.getImm();
    else
        printOperand(MI,OpNum,O);
}

void Cpu0InstPrinter::printMemOperand(const MCInst *MI,int OpNum,raw_ostream &O)
{
    printOperand(MI,OpNum+1,O);
    O<<"(";
    printOperand(MI,OpNum,O);
    O<<")";
}