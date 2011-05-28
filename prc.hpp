#ifndef R_UM_HDR_PRC

#define R_UM_HDR_PRC

#include <cstdlib>

#include "um.hpp"

#include "mem.hpp"
#include "reg.hpp"
#include "ldr.hpp"
#include "con.hpp"

#ifdef R_UM_OPT_ENABLE_LOGGING
#ifdef R_UM_OPT_PRC_DEBUG
#include <cstdio>
#include "log.hpp"
#endif
#endif

R_UM_NS_START

	class Processor {
		public:
			Processor(const std::string &filename);
			~Processor();
			void run();
		private:
			Byte execInstruction();
			Byte decodeInstruction(Word instr) const;
			Byte decodeAreg(Word instr) const;
			Byte decodeBreg(Word instr) const;
			Byte decodeCreg(Word instr) const;
			Byte decodeOreg(Word instr) const;
			Word decodeVal(Word instr) const;
			bool isValidMemMan() const;
			bool isValidRegisterBlock() const;

			Byte CNDMOV(Byte A, Byte B, Byte C);
			Byte ARRIDX(Byte A, Byte B, Byte C);
			Byte ARRAMD(Byte A, Byte B, Byte C);
			Byte ADD(Byte A, Byte B, Byte C);
			Byte MUL(Byte A, Byte B, Byte C);
			Byte DIV(Byte A, Byte B, Byte C);
			Byte NOTAND(Byte A, Byte B, Byte C);

			Byte HLT();
			Byte ALLOC(Byte B, Byte C);
			Byte ABAND(Byte C);
			Byte OUTP(Byte C);
			Byte INP(Byte C);
			Byte LDPRG(Byte B, Byte C);

			Byte ORTHO(Byte O, Word V);

			static const Word numOfRegs_ = 8;

			static const Byte IC_BIT_OFFSET = 28;
			static const Byte REG_BIT_WIDTH = 3;
			static const Byte REG_A_BIT_OFFSET = 6;
			static const Byte REG_B_BIT_OFFSET = 3;
			static const Byte REG_C_BIT_OFFSET = 0;
			static const Byte REG_O_BIT_OFFSET = 25;
			static const Byte VAL_BIT_WIDTH = REG_O_BIT_OFFSET;

			static const Byte IC_CNDMOV = 0;
			static const Byte IC_ARRIDX = 1;
			static const Byte IC_ARRAMD = 2;
			static const Byte IC_ADD = 3;
			static const Byte IC_MUL = 4;
			static const Byte IC_DIV = 5;
			static const Byte IC_NOTAND = 6;

			static const Byte IC_HLT = 7;
			static const Byte IC_ALLOC = 8;
			static const Byte IC_ABAND = 9;
			static const Byte IC_OUTP = 10;
			static const Byte IC_INP = 11;
			static const Byte IC_LDPRG = 12;

			static const Byte IC_ORTHO = 13;

			MemMan *mm_;
			RegisterBlock *reg_;
			Word pc_;

#ifdef R_UM_OPT_PRC_DEBUG
			static const Word msglen_ = 250;

			Word ic_;
			Logger *log_;
			char *logmsg_;
#endif
	};

R_UM_NS_END

#endif
