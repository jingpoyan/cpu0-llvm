#ifndef CPU0_ANALYZE_IMMEDIATE_H
#define CPU0_ANALYZE_IMMEDIATE_H

#include "llvm/ADT/SmallVector.h"
#include "llvm/Support/DataTypes.h"

namespace llvm {

  class Cpu0AnalyzeImmediate {
  public:
    struct Inst {
      unsigned Opc, ImmOperand;
      Inst(unsigned Opc, unsigned ImmOperand);
    };
    typedef SmallVector<Inst, 7 > InstSeq;

    const InstSeq &Analyze(uint64_t Imm, unsigned Size, bool LastInstrIsADDiu);
  private:
    typedef SmallVector<InstSeq, 5> InstSeqLs;

    void AddInstr(InstSeqLs &SeqLs, const Inst &I);


    void GetInstSeqLsADDiu(uint64_t Imm, unsigned RemSize, InstSeqLs &SeqLs);

    void GetInstSeqLsORi(uint64_t Imm, unsigned RemSize, InstSeqLs &SeqLs);


    void GetInstSeqLsSHL(uint64_t Imm, unsigned RemSize, InstSeqLs &SeqLs);

    void GetInstSeqLs(uint64_t Imm, unsigned RemSize, InstSeqLs &SeqLs);

    void ReplaceADDiuSHLWithLUi(InstSeq &Seq);

    void GetShortestSeq(InstSeqLs &SeqLs, InstSeq &Insts);

    unsigned Size;
    unsigned ADDiu, ORi, SHL, LUi;
    InstSeq Insts;
  };
}

#endif