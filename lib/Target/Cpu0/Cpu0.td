include "llvm/Target/Target.td"


include "Cpu0RegisterInfo.td"

include "Cpu0Schedule.td"

include "Cpu0InstrInfo.td"


def Cpu0InstrInfo : InstrInfo;

def Cpu0 : Target
{
    let InstructionSet = Cpu0InstrInfo;
}