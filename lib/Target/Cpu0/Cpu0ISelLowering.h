#ifndef LLVM_LIB_TARGET_CPU0_CPU0ISELLOWERING_H
#define LLVM_LIB_TARGET_CPU0_CPU0ISELLOWERING_H

#include "MCTargetDesc/Cpu0ABIInfo.h"
#include "MCTargetDesc/Cpu0BaseInfo.h"
#include "Cpu0.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/SelectionDAGNodes.h"
#include "llvm/CodeGen/TargetLowering.h"
#include "llvm/IR/Function.h"
#include <deque>


namespace llvm{
    namespace Cpu0ISD{
        enum NodeType
        {
            FIRST_NUMBER= ISD::BUILTIN_OP_END,
            JmpLink,
            TailCall,
            Hi,
            Lo,
            GPRel,
            ThreadPointer,
            Ret,
            EH_RETURN,
            DivRem,
            DivRemU,
            Wrapper,
            DynAlloc,
            Sync
        };
    }

    class Cpu0MachineFunctionInfo;
    class Cpu0Subtarget;

    class Cpu0TargetLowering : public TargetLowering{
        public:
            explicit Cpu0TargetLowering(const Cpu0TargetMachine &TM,const Cpu0Subtarget &STI);

            static const Cpu0TargetLowering* create(const Cpu0TargetMachine &TM,const Cpu0Subtarget &STI);

            SDValue LowerOperation(SDValue Op,SelectionDAG &DAG) const override;

            const char* getTargetNodeName(unsigned Opcode) const override;

            SDValue PerformDAGCombine(SDNode *N,DAGCombinerInfo &DCI) const override;

            EVT getSetCCResultType(const DataLayout&,LLVMContext &,EVT VT) const override;

            bool isOffsetFoldingLegal(const GlobalAddressSDNode* GA)const override;
        protected:
            SDValue getGlobalReg(SelectionDAG &DAG,EVT Ty) const;
            
            template<class NodeTy>
            SDValue getAddrLocal(NodeTy *N,EVT Ty,SelectionDAG &DAG) const
            {
                SDLoc DL(N);
                unsigned GOTFlag = Cpu0II::MO_GOT;
                SDValue GOT = DAG.getNode(Cpu0ISD::Wrapper, DL, Ty, getGlobalReg(DAG, Ty),getTargetNode(N, Ty, DAG, GOTFlag));
                SDValue Load = DAG.getLoad(Ty, DL, DAG.getEntryNode(), GOT,MachinePointerInfo::getGOT(DAG.getMachineFunction()));
                unsigned LoFlag = Cpu0II::NO_ABS_LO;
                SDValue Lo = DAG.getNode(Cpu0ISD::Lo, DL, Ty,getTargetNode(N, Ty, DAG, LoFlag));
                return DAG.getNode(ISD::ADD, DL, Ty, Load, Lo);            
            }

            template<class NodeTy>
            SDValue getAddrGlobal(NodeTy* N,EVT Ty,SelectionDAG &DAG,unsigned Flag,SDValue Chain,const MachinePointerInfo &PtrInfo) const 
            {
                SDLoc DL(N);
                SDValue Tgt = DAG.getNode(Cpu0ISD::Wrapper,DL,Ty,getGlobalReg(DAG,Ty),getTargetNode(N,Ty,DAG,Flag));
                return DAG.getLoad(Ty,DL,Chain,Tgt,PtrInfo);
            }

            template<class NodeTy>
            SDValue getAddrGlobalLargeGOT(NodeTy *N,EVT Ty,SelectionDAG &DAG,unsigned HiFlag,unsigned LoFlag,SDValue Chain,const MachinePointerInfo &PtrInfo) const
            {
                SDLoc DL(N);
                SDValue Hi = DAG.getNode(Cpu0ISD::Hi,DL,Ty,getTargetNode(N,Ty,DAG,HiFlag));
                Hi = DAG.getNode(ISD::ADD,DL,Ty,Hi,getGlobalReg(DAG,Ty));
                SDValue Wrapper = DAG.getNode(Cpu0ISD::Wrapper,DL,Ty,Hi,getTargetNode(N,Ty,DAG,LoFlag));
                return DAG.getLoad(Ty,DL,Chain,Wrapper,PtrInfo);
            }

            template<class NodeTy>
            SDValue getAddrNonPIC(NodeTy *N,EVT Ty,SelectionDAG &DAG) const
            {
                SDLoc DL(N);
                SDValue Hi = getTargetNode(N,Ty,DAG,Cpu0II::NO_ABS_HI);
                SDValue Lo = getTargetNode(N,Ty,DAG,Cpu0II::NO_ABS_LO);
                return DAG.getNode(ISD::ADD,DL,Ty,DAG.getNode(Cpu0ISD::Hi,DL,Ty,Hi),DAG.getNode(Cpu0ISD::Lo,DL,Ty,Lo));
            }

            struct ByValArgInfo
            {
                unsigned FirstIdx;
                unsigned NumRegs;
                unsigned Address;
                ByValArgInfo() : FirstIdx(0),NumRegs(0),Address(0){} 
            };
            
            const Cpu0Subtarget &Subtarget;
            const Cpu0ABIInfo &ABI;

            class Cpu0CC
            {
                public:
                    enum SpecialCallingConvType
                    {
                        NoSpecialCallingConv
                    };

                    Cpu0CC(CallingConv::ID CallConv,bool IsO32,CCState &Info,SpecialCallingConvType SpecialCallingConv = NoSpecialCallingConv);

                    void analyzeCallResult(const SmallVectorImpl<ISD::InputArg> &Ins,bool IsSoftFloat,const SDNode *CallNode,const Type *RetTy) const;

                    void analyzeReturn(const SmallVectorImpl<ISD::OutputArg> &Outs,bool IsSoftFloat,const Type *RetTy) const;

                    const CCState &getCCInfo() const {return CCInfo;}
                    
                    bool hashByValArg() const {return !ByValArgs.empty();}

                    unsigned reservedArgArea() const;

                    typedef SmallVectorImpl<ByValArgInfo>::const_iterator byval_iterator;
                    byval_iterator byval_begin() const {return ByValArgs.begin();}
                    byval_iterator byval_end() const {return ByValArgs.end();}

                private:
                    MVT getRegVT(MVT VT,const Type *OrigTy,const SDNode *CallNode,bool IsSoftFloat) const;

                    template<typename Ty> void analyzeReturn(const SmallVectorImpl<Ty> &RetVals,bool IsSoftFloat,const SDNode *CallNode,const Type* RetTy) const;

                    CCState &CCInfo;
                    CallingConv::ID CallConv;
                    bool IsO32;
                    SmallVector<ByValArgInfo,2> ByValArgs;
            };

        private:
            SDValue getTargetNode(GlobalAddressSDNode *N, EVT Ty, SelectionDAG &DAG,unsigned Flag) const;
            SDValue getTargetNode(ExternalSymbolSDNode *N, EVT Ty, SelectionDAG &DAG,unsigned Flag) const;
            SDValue LowerGlobalAddress(SDValue Op,SelectionDAG &DAG) const;
            SDValue LowerFormalArguments(SDValue Chain,
                               CallingConv::ID CallConv, bool IsVarArg,
                               const SmallVectorImpl<ISD::InputArg> &Ins,
                               const SDLoc &dl, SelectionDAG &DAG,
                               SmallVectorImpl<SDValue> &InVals) const override;    
            SDValue LowerReturn(SDValue Chain,
                                CallingConv::ID CallConv, bool IsVarArg,
                                const SmallVectorImpl<ISD::OutputArg> &Outs,
                                const SmallVectorImpl<SDValue> &OutVals,
                                const SDLoc &dl, SelectionDAG &DAG) const override;                
};
    const Cpu0TargetLowering* createCpu0SETargetLowering(const Cpu0TargetMachine &TM,const Cpu0Subtarget &STI);
}

#endif