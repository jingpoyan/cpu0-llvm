def SDT_Cpu0Ret : SDTypeProfile<0,1,[SDTCisInt<0>]>;

def Cpu0Ret : SDNode<"Cpu0ISD::Ret",SDTNone,[SDNPHasChain,SDNPOptInGlue,SDNPVariadic]>;

def EnableOverflow  : Predicate<"Subtarget->enableOverflow()">;
def DisableOverflow : Predicate<"Subtarget->disableOverflow()">;

def HasCmp          : Predicate<"Subtarget->hasCmp()">;
def HasSlt          : Predicate<"Subtarget->hasSlt()">;

include "Cpu0InstrFormats.td"

class Cpu0InstAlias<string Asm,dag Result,bit Emit = 0b1> : InstAlias<Asm,Result,Emit>;

def simm16 : Operand<i32>
{
    let DecoderMethod = "DecodeSimm16";
}
def shamt : Operand<i32>;

def uimm16 : Operand<i32>
{
    let PrintMethod = "printUnsignedImm";
}


def mem : Operand<iPTR>
{
    let PrintMethod = "printMemOperand";
    let MIOperandInfo = (ops CPURegs,simm16);
    let EncoderMethod = "getMemEncoding";
}

def immSExt16 : PatLeaf<(imm),[{return isInt<16>(N->getSExtValue());}]>;


def LO16 : SDNodeXForm<imm, [{
  return getImm(N, N->getZExtValue() & 0xffff);
}]>;

def HI16 : SDNodeXForm<imm, [{
  return getImm(N, (N->getZExtValue() >> 16) & 0xffff);
}]>;

def immZExt16 : PatLeaf<(imm), [{
  if (N->getValueType(0) == MVT::i32)
    return (uint32_t)N->getZExtValue() == (unsigned short)N->getZExtValue();
  else
    return (uint64_t)N->getZExtValue() == (unsigned short)N->getZExtValue();
}], LO16>;

def immLow16Zero : PatLeaf<(imm), [{
  int64_t Val = N->getSExtValue();
  return isInt<32>(Val) && !(Val & 0xffff);
}]>;

def immZExt5 : ImmLeaf<i32, [{ return Imm == (Imm & 0x1f); }]>;

def addr : ComplexPattern<iPTR,2,"SelectAddr",[frameindex],[SDNPWantParent]>;

class AlignedLoad<PatFrag Node> : PatFrag<(ops node:$ptr),(Node node:$ptr),
[{
    LoadSDNode* LD = cast<LoadSDNode>(N);
    return LD->getMemoryVT().getSizeInBits()/8 <= LD->getAlignment();
}]>;

class AlignedStore<PatFrag Node> : PatFrag<(ops node:$val,node:$ptr),(Node node:$val,node:$ptr),
[{
    StoreSDNode* ST = cast<StoreSDNode>(N);
    return ST->getMemoryVT().getSizeInBits()/8 <= ST->getAlignment();
}]>;

def load_a  : AlignedLoad<load>;
def store_a : AlignedStore<store>;


class ArithLogicI<bits<8> op, string instrAsm, SDNode opNode,
                  Operand od, PatLeaf immType, RegisterClass regClass>
  : FL<op, (outs GPROut:$ra), (ins regClass:$rb, od:$imm16),
       !strconcat(instrAsm, "\t$ra, $rb, $imm16"),
       [(set GPROut:$ra, (opNode regClass:$rb, immType:$imm16))], IIAlu> {
  let isReMaterializable = 1;
}


class ArithLogicR<bits<8> op, string instr_asm, SDNode OpNode,
                  InstrItinClass itin, RegisterClass RC, bit isComm = 0>
  : FA<op, (outs GPROut:$ra), (ins RC:$rb, RC:$rc),
       !strconcat(instr_asm, "\t$ra, $rb, $rc"),
       [(set GPROut:$ra, (OpNode RC:$rb, RC:$rc))], itin> {
  let shamt = 0;
  let isCommutable = isComm;
  let isReMaterializable = 1;
}

class FMem<bits<8> op,dag outs,dag ins,string asmStr,list<dag> pattern,InstrItinClass itin>
: FL<op,outs,ins,asmStr,pattern,itin>
{
    bits<20> addr;
    let Inst{19-16} = addr{19-16};
    let Inst{15-0} = addr{15-0};
    let DecoderMethod = "DecodeMem";
}



let canFoldAsLoad = 1 in
class LoadM<bits<8> op,string instrAsm,PatFrag opNode,RegisterClass regClass,
Operand od,bit pseudo> : FMem<op,(outs regClass:$ra),(ins od:$addr),
    !strconcat(instrAsm,"\t$ra,$addr"),
    [(set regClass:$ra,(opNode addr:$addr))],IIAlu>
{
    let isPseudo = pseudo;
}

class StoreM<bits<8> op,string instrAsm,PatFrag opNode,RegisterClass regClass,
Operand od,bit pseudo> : FMem<op,(outs),(ins regClass:$ra,od:$addr),
    !strconcat(instrAsm,"\t$ra,$addr"),
    [(opNode regClass:$ra,addr:$addr)],IIStore>
{
    let isPseudo = pseudo;
}


multiclass LoadM32<bits<8> op,string instrAsm,PatFrag opNode,bit pseudo=0>
{
    def #NAME# : LoadM<op,instrAsm,opNode,GPROut,mem,pseudo>;
}

class LoadUpper<bits<8> op, string instrAsm, RegisterClass RC, Operand Imm>
  : FL<op, (outs RC:$ra), (ins Imm:$imm16),
       !strconcat(instrAsm, "\t$ra, $imm16"), [], IIAlu> {
  let rb = 0;
  let isReMaterializable = 1;
}

class ShiftRotateI<bits<8> op, bits<4> isRotate, string instrAsm,
                   SDNode OpNode, PatFrag PF, Operand ImmOperand,
                   RegisterClass RC>
  : FA<op, (outs GPROut:$ra), (ins RC:$rb, ImmOperand:$shamt),
       !strconcat(instrAsm, "\t$ra, $rb, $shamt"),
       [(set GPROut:$ra, (OpNode RC:$rb, PF:$shamt))], IIAlu> {
  let rc = 0;
}



multiclass StoreM32<bits<8> op,string instrAsm,PatFrag opNode,bit pseudo=0>
{
    def #NAME# : StoreM<op,instrAsm,opNode,CPURegs,mem,pseudo>;
}


//JumpFR
let isBranch=1,isTerminator=1,isBarrier=1,hasDelaySlot=1,isIndirectBranch=1 in
class JumpFR<bits<8> op,string instrAsm,RegisterClass regClass> : FL<op,(outs),(ins regClass:$ra),
    !strconcat(instrAsm,"\t$ra"),[(brind regClass:$ra)],IIBranch>
{
    let rb = 0;
    let imm16 = 0;
}

//return
class RetBase<RegisterClass regClass> : JumpFR<0x3c,"ret",regClass>
{
    let isReturn = 1;
    let isCodeGenOnly = 1;
    let hasCtrlDep = 1;
    let hasExtraSrcRegAllocReq = 1;
}

defm LD     : LoadM32<0x01,"ld",load_a>;
defm ST     : StoreM32<0x02,"st",store_a>;

def ADDiu    : ArithLogicI<0x09, "addiu", add, simm16, immSExt16, CPURegs>;
def ORi      : ArithLogicI<0x0d, "ori", or, uimm16, immZExt16, CPURegs>;
def LUi      : LoadUpper<0x0f, "lui", GPROut, uimm16>;
def SHL      : ShiftRotateI<0x1e, 0x00, "shl", shl, immZExt5, shamt, CPURegs>;

let Predicates = [DisableOverflow] in {
  def ADDu   : ArithLogicR<0x11, "addu", add, IIAlu, CPURegs, 1>;
}

def JR      : JumpFR<0x3c,"jr",GPROut>;

def RET     : RetBase<GPROut>;

let addr=0 in
    def NOP : FJ<0,(outs),(ins),"nop",[],IIAlu>;


let isReturn=1,isTerminator=1,hasDelaySlot=1,isBarrier=1,hasCtrlDep=1 in
def RetLR : Cpu0Pseudo<(outs),(ins),"",[(Cpu0Ret)]>;

def : Pat<(i32 immSExt16:$in), (ADDiu ZERO, imm:$in)>;
def : Pat<(i32 immZExt16:$in), (ORi ZERO, imm:$in)>;
def : Pat<(i32 immLow16Zero:$in), (LUi (HI16 imm:$in))>;
