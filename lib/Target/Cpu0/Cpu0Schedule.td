def ALU : FuncUnit;
def IMULDIV : FuncUnit;


def IIAlu               : InstrItinClass;
def IICLO               : InstrItinClass;
def IICLZ               : InstrItinClass;
def IILoad              : InstrItinClass;
def IIStore             : InstrItinClass;
def IIBranch            : InstrItinClass;
def IIPseudo            : InstrItinClass;

def IIHiLo              : InstrItinClass;
def IIImul              : InstrItinClass;
def IIIdiv              : InstrItinClass;


def Cpu0GenericItineraries : ProcessorItineraries<[ALU,IMULDIV],[],
[
    InstrItinData<IIAlu             , [InstrStage<1,[ALU]>]>,
    InstrItinData<IICLO             , [InstrStage<1,[ALU]>]>,
    InstrItinData<IICLZ             , [InstrStage<1,[ALU]>]>,
    InstrItinData<IILoad            , [InstrStage<3,[ALU]>]>,
    InstrItinData<IIStore           , [InstrStage<1,[ALU]>]>,
    InstrItinData<IIPseudo          , [InstrStage<1,[ALU]>]>,
    InstrItinData<IIBranch          , [InstrStage<1,[ALU]>]>,
    InstrItinData<IIHiLo            , [InstrStage<1,[IMULDIV]>]>,
    InstrItinData<IIImul            , [InstrStage<17,[IMULDIV]>]>,
    InstrItinData<IIIdiv            , [InstrStage<37,[IMULDIV]>]>
]>;