class CCIfSubtarget<string F,CCAction A> : CCIf<!strconcat("State.getTarget().getSubtarget<Cpu0Subtarget>().",F),A>;

def CSR_032 : CalleeSavedRegs<(add LR,FP,(sequence "S%u",1,0))>;
