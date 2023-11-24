class CCIfSubtarget<string F,CCAction A> : CCIf<!strconcat("State.getTarget().getSubtarget<Cpu0Subtarget>().",F),A>;

def CSR_O32 : CalleeSavedRegs<(add LR,FP,(sequence "S%u",1,0))>;


def RetCC_Cpu0EABI : CallingConv<[
    CCIfType<[i32],CCAssignToReg<[V0,V1,A0,A1]>>
]>;

def RetCC_Cpu0 : CallingConv<[
    CCDelegateTo<RetCC_Cpu0EABI>
]>;