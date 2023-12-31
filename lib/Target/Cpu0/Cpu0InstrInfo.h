#ifndef LLVM_LIB_TARGET_CPU0_CPU0INSTRINFO_H
#define LLVM_LIB_TARGET_CPU0_CPU0INSTRINFO_H

#include "Cpu0.h"
#include "Cpu0RegisterInfo.h"
#include "Cpu0AnalyzeImmediate.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/TargetInstrInfo.h"


#define GET_INSTRINFO_HEADER
#include "Cpu0GenInstrInfo.inc"


namespace llvm{
    class Cpu0InstrInfo : public Cpu0GenInstrInfo
    {
        virtual void anchor();
        protected:
            const Cpu0Subtarget &Subtarget;
        public:
            explicit Cpu0InstrInfo(const Cpu0Subtarget &STI);
            static const Cpu0InstrInfo* create(Cpu0Subtarget &STI);
            virtual const Cpu0RegisterInfo &getRegisterInfo() const = 0;
            unsigned GetInstSizeInBytes(const MachineInstr &MI) const;

            void storeRegToStackSlot(MachineBasicBlock &MBB,
                                    MachineBasicBlock::iterator MBBI,
                                    unsigned SrcReg, bool isKill, int FrameIndex,
                                    const TargetRegisterClass *RC,
                                    const TargetRegisterInfo *TRI) const override {
                storeRegToStack(MBB, MBBI, SrcReg, isKill, FrameIndex, RC, TRI, 0);
            }

            void loadRegFromStackSlot(MachineBasicBlock &MBB,
                                        MachineBasicBlock::iterator MBBI,
                                        unsigned DestReg, int FrameIndex,
                                        const TargetRegisterClass *RC,
                                        const TargetRegisterInfo *TRI) const override {
                loadRegFromStack(MBB, MBBI, DestReg, FrameIndex, RC, TRI, 0);
            }

            virtual void storeRegToStack(MachineBasicBlock &MBB,
                                        MachineBasicBlock::iterator MI,
                                        unsigned SrcReg, bool isKill, int FrameIndex,
                                        const TargetRegisterClass *RC,
                                        const TargetRegisterInfo *TRI,
                                        int64_t Offset) const = 0;

            virtual void loadRegFromStack(MachineBasicBlock &MBB,
                                            MachineBasicBlock::iterator MI,
                                            unsigned DestReg, int FrameIndex,
                                            const TargetRegisterClass *RC,
                                            const TargetRegisterInfo *TRI,
                                            int64_t Offset) const = 0;

            virtual void adjustStackPtr(unsigned SP, int64_t Amount,
                                        MachineBasicBlock &MBB,
                                        MachineBasicBlock::iterator I) const = 0;

            protected:
            MachineMemOperand *GetMemOperand(MachineBasicBlock &MBB, int FI,
                                            MachineMemOperand::Flags Flags) const;
    };

    const Cpu0InstrInfo *createCpu0SEInstrInfo(const Cpu0Subtarget &STI);
}

#endif