#include "Cpu0MCCodeEmitter.h"

#include "MCTargetDesc/Cpu0BaseInfo.h"
#include "MCTargetDesc/Cpu0FixupKinds.h"
#include "MCTargetDesc/Cpu0MCExpr.h"
#include "MCTargetDesc/Cpu0MCTargetDesc.h"

#include "llvm/ADT/APFloat.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrDesc.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/raw_ostream.h"

#define DEBUG_TYPE "mccodeemitter"

#define GET_INSTRMAP_INFO
#include "Cpu0GenInstrInfo.inc"
#undef GET_INSTRMAP_INFO

namespace llvm {
MCCodeEmitter *createCpu0MCCodeEmitterEB(const MCInstrInfo &MCII,
                                               const MCRegisterInfo &MRI,
                                               MCContext &Ctx) {
  return new Cpu0MCCodeEmitter(MCII, Ctx, false);
}

MCCodeEmitter *createCpu0MCCodeEmitterEL(const MCInstrInfo &MCII,
                                               const MCRegisterInfo &MRI,
                                               MCContext &Ctx) {
  return new Cpu0MCCodeEmitter(MCII, Ctx, true);
}
} 

void Cpu0MCCodeEmitter::EmitByte(unsigned char C, raw_ostream &OS) const {
  OS << (char)C;
}

void Cpu0MCCodeEmitter::EmitInstruction(uint64_t Val, unsigned Size,
                                        raw_ostream &OS) const {
  for (unsigned i = 0; i < Size; ++i) {
    unsigned Shift = IsLittleEndian ? i*8 : (Size - 1 - i) * 8;
    EmitByte((Val >> Shift) & 0xff, OS);
  }
}

void Cpu0MCCodeEmitter::encodeInstruction(const MCInst &MI, raw_ostream &OS,
                                          SmallVectorImpl<MCFixup> &Fixups,
                                          const MCSubtargetInfo &STI) const {
  uint32_t Binary = getBinaryCodeForInstr(MI, Fixups, STI);

  unsigned Opcode = MI.getOpcode();
  if ((Opcode != Cpu0::NOP) && (Opcode != Cpu0::SHL) && !Binary)
    llvm_unreachable("unimplemented opcode in encodeInstruction()");

  const MCInstrDesc &Desc = MCII.get(MI.getOpcode());
  uint64_t TSFlags = Desc.TSFlags;

  if ((TSFlags & Cpu0II::FrmMask) == Cpu0II::Pseudo)
    llvm_unreachable("Pseudo opcode found in encodeInstruction()");

  int Size = 4; 

  EmitInstruction(Binary, Size, OS);
}

unsigned Cpu0MCCodeEmitter::
getBranch16TargetOpValue(const MCInst &MI, unsigned OpNo,
                         SmallVectorImpl<MCFixup> &Fixups,
                         const MCSubtargetInfo &STI) const {
  return 0;
}

unsigned Cpu0MCCodeEmitter::
getBranch24TargetOpValue(const MCInst &MI, unsigned OpNo,
                         SmallVectorImpl<MCFixup> &Fixups,
                         const MCSubtargetInfo &STI) const {
  return 0;
}

unsigned Cpu0MCCodeEmitter::
getJumpTargetOpValue(const MCInst &MI, unsigned OpNo,
                     SmallVectorImpl<MCFixup> &Fixups,
                     const MCSubtargetInfo &STI) const {
  return 0;
}

unsigned Cpu0MCCodeEmitter::getExprOpValue(const MCExpr *Expr,
                                           SmallVectorImpl<MCFixup> &Fixups,
                                           const MCSubtargetInfo &STI) const {
  MCExpr::ExprKind Kind = Expr->getKind();
  if (Kind == MCExpr::Constant) {
    return cast<MCConstantExpr>(Expr)->getValue();
  }

  if (Kind == MCExpr::Binary) {
    unsigned Res = getExprOpValue(cast<MCBinaryExpr>(Expr)->getLHS(), Fixups, STI);
    Res += getExprOpValue(cast<MCBinaryExpr>(Expr)->getRHS(), Fixups, STI);
    return Res;
  }

  if (Kind == MCExpr::Target) {
    const Cpu0MCExpr *Cpu0Expr = cast<Cpu0MCExpr>(Expr);

    Cpu0::Fixups FixupKind = Cpu0::Fixups(0);
    switch(Cpu0Expr->getKind()) {
    default: llvm_unreachable("Unsupported fixup kind for target expression!");
    }
    Fixups.push_back(MCFixup::create(0, Cpu0Expr, MCFixupKind(FixupKind)));
    return 0;
  }
  return 0;
}

unsigned Cpu0MCCodeEmitter::
getMachineOpValue(const MCInst &MI, const MCOperand &MO,
                  SmallVectorImpl<MCFixup> &Fixups,
                  const MCSubtargetInfo &STI) const {
  if (MO.isReg()) {
    unsigned Reg = MO.getReg();
    unsigned RegNo = Ctx.getRegisterInfo()->getEncodingValue(Reg);
    return RegNo;
  } else if (MO.isImm()) {
    return static_cast<unsigned>(MO.getImm());
  } else if (MO.isFPImm()) {
    return static_cast<unsigned>(APFloat(MO.getFPImm())
          .bitcastToAPInt().getHiBits(32).getLimitedValue());
  }

  assert(MO.isExpr());
  return getExprOpValue(MO.getExpr(), Fixups, STI);
}

unsigned Cpu0MCCodeEmitter::getMemEncoding(const MCInst &MI, unsigned OpNo,
                                           SmallVectorImpl<MCFixup> &Fixups,
                                           const MCSubtargetInfo &STI) const {
  assert(MI.getOperand(OpNo).isReg());
  unsigned RegBits = getMachineOpValue(MI, MI.getOperand(OpNo), Fixups, STI) << 16;
  unsigned OffBits = getMachineOpValue(MI, MI.getOperand(OpNo+1), Fixups, STI);

  return (OffBits & 0xFFFF) | RegBits;
}

#include "Cpu0GenMCCodeEmitter.inc"