include "llvm/Target/Target.td"


include "Cpu0RegisterInfo.td"

include "Cpu0Schedule.td"

include "Cpu0InstrInfo.td"


//Calling Convention
include "Cpu0CallingConv.td"

//Subtarget
def FeatureCmp              : SubtargetFeature<"cmp", "HasCmp", "true","Enable 'cmp' instructions.">;
def FeatureSlt              : SubtargetFeature<"slt", "HasSlt", "true","Enable 'slt' instructions.">;

def FeatureCpu032I          : SubtargetFeature<"cpu032I","Cpu0ArchVersion","Cpu032I","Cpu032I ISA Support",[FeatureCmp]>;

def FeatureCpu032II         : SubtargetFeature<"cpu032II","Cpu0ArchVersion","Cpu032II","Cpu032II ISA Support",[FeatureCmp,FeatureSlt]>;


class Proc<string Name,list<SubtargetFeature> Features> : Processor<Name,Cpu0GenericItineraries,Features>;

def : Proc<"cpu032I",[FeatureCpu032I]>;
def : Proc<"cpu032II",[FeatureCpu032II]>;


def Cpu0InstrInfo : InstrInfo;

def Cpu0 : Target
{
    let InstructionSet = Cpu0InstrInfo;
}