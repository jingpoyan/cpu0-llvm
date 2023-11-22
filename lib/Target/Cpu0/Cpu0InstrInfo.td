def SDT_Cpu0Ret : SDTypeProfile<0,1,[SDTCisInt<0>]>;

def Cpu0Ret : SDNode<"Cpu0ISD::Ret",SDTNone,[SDNPHasChain,SDNPOptInGlue,SDNPVariadic]>;

def EnableOverflow  : Predicate<"Subtarget->enableOverflow()">;
def DisableOverflow : Predicate<"Subtarget->disableOverflow()">;

def HasCmp          : Predicate<"Subtarget->hasCmp()">;
def HasSlt          : Predicate<"Subtarget->hasSlt()">;

include "Cpu0InstrFormats.td"


def simm16 : Operand<i32>
{
    let DecoderMethod = "DecodeSimm16";
}

def mem : Operand<iPTR>
{
    let PrintMethod = "printMemOperand";
    let MIOperandInfo = (ops CPURegs,simm16);
    let EncoderMethod = "getMemEncoding";
}

def immSExt16 : PatLeaf<(imm),[{return isInt<16>(N->getSExtValue());}]>;

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


class ArighLogicI<bits<8> op, string instrAsm, SDNode opNode,
                  Operand od, PatLeaf immType, RegisterClass regClass>
  : FL<op, (outs GPROut:$ra), (ins regClass:$rb, od:$imm16),
       !strconcat(instrAsm, "\t$ra, $rb, $imm16"),
       [(set GPROut:$ra, (opNode regClass:$rb, immType:$imm16))], IIAlu> {
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

def ADDiu    : ArighLogicI<0x09, "addiu", add, simm16, immSExt16, CPURegs>;

def JR      : JumpFR<0x3c,"jr",GPROut>;

def RET     : RetBase<GPROut>;

let addr=0 in
    def NOP : FJ<0,(outs),(ins),"nop",[],IIAlu>;


def : Pat<(i32 immSExt16:$in),(ADDiu ZERO,imm:$in)>;
