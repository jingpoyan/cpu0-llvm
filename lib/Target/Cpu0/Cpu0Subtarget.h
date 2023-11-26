#ifndef LLVM_LIB_TARGET_CPU0_CPU0SUBTARGET_H
#define LLVM_LIB_TARGET_CPU0_CPU0SUBTARGET_H


#include"Cpu0FrameLowering.h"
#include"Cpu0ISelLowering.h"
#include"Cpu0InstrInfo.h"
#include"llvm/CodeGen/SelectionDAGTargetInfo.h"
#include"llvm/IR/DataLayout.h"
#include"llvm/MC/MCInstrItineraries.h"
#include"llvm/CodeGen/TargetSubtargetInfo.h"
#include <string>


#define GET_SUBTARGETINFO_HEADER
#include "Cpu0GenSubtargetInfo.inc"

extern bool Cpu0ReserveGP;
extern bool Cpu0NoCpload;

namespace llvm
{
    class StringRef;
    class Cpu0TargetMachine;

    class Cpu0Subtarget : public Cpu0GenSubtargetInfo{
            virtual void anchor();
        protected:
            enum Cpu0ArchEnum
            {
                Cpu032I,
                Cpu032II
            };

            Cpu0ArchEnum Cpu0ArchVersion;

            bool IsLittle;

            bool EnableOverflow;
            bool HasCmp;
            bool HasSlt;
            
            InstrItineraryData InstrItins;
            
            bool UseSmallSection;
            
            const Cpu0TargetMachine &TM;

            Triple TargetTriple;
            
            const SelectionDAGTargetInfo TSInfo;

            std::unique_ptr<const Cpu0InstrInfo> InstrInfo;
            std::unique_ptr<const Cpu0FrameLowering> FrameLowering;
            std::unique_ptr<const Cpu0TargetLowering> TLInfo;

        public:
            bool isPositionIndependent() const;
            const Cpu0ABIInfo &getABI() const;

            Cpu0Subtarget(const Triple &TT,const std::string &CPU,const std::string &FS,bool little,const Cpu0TargetMachine &_TM);

            void ParseSubtargetFeatures(StringRef CPU,StringRef FS);

            bool isLittle() const { return IsLittle; }

            bool hasCpu032I() const {return Cpu0ArchVersion >= Cpu032I;}
            bool isCpu032I() const {return Cpu0ArchVersion == Cpu032I;}
            bool hasCpu032II() const {return Cpu0ArchVersion >= Cpu032II;}
            bool isCpu032II() const {return Cpu0ArchVersion == Cpu032II;}

            bool enableOverflow() const {return EnableOverflow;}
            bool disableOverflow() const {return !EnableOverflow;}
            bool hasCmp() const {return HasCmp;}
            bool hasSlt() const {return HasSlt;}

            bool useSmallSection() const { return UseSmallSection; }

            bool abiUsesSoftFloat() const;
            bool enableLongBranchPass() const{
                return hasCpu032II();
            }

            unsigned stackAlignment() const {return 8;}
            
            Cpu0Subtarget &initializezSubtargetDependencies(StringRef CPU,StringRef FS,const TargetMachine &TM);

            const SelectionDAGTargetInfo *getSelectionDAGInfo() const override
            {
                return &TSInfo;
            }

            const Cpu0InstrInfo *getInstrInfo() const override
            {
                return InstrInfo.get();
            }

            const TargetFrameLowering* getFrameLowering() const override
            {
                return FrameLowering.get();
            }

            const Cpu0RegisterInfo* getRegisterInfo() const override
            {
                return &InstrInfo->getRegisterInfo();
            }

            const Cpu0TargetLowering* getTargetLowering() const override
            {
                return TLInfo.get();
            }

            const InstrItineraryData* getInstrItineraryData() const override
            {
                return &InstrItins;
            }
    };
}

#endif