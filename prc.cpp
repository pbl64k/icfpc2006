#include "prc.hpp"

R_UM_NS_START

	Processor::Processor(const std::string &filename): mm_(new MemMan()),
			reg_(new RegisterBlock(numOfRegs_)), pc_(0)
#ifdef R_UM_OPT_ENABLE_LOGGING
#ifdef R_UM_OPT_PRC_DEBUG
			, ic_(0), log_(new Logger("um.dbg")), logmsg_(new char[msglen_])
#endif
#endif
	{
		assert(isValidMemMan());
		assert(isValidRegisterBlock());

#ifdef R_UM_OPT_ENABLE_LOGGING
#ifdef R_UM_OPT_PRC_DEBUG
		assert(log_ != NULL);
#endif
#endif

		Console::init();

		Word rootPageId = PageLoader::loadPage(mm_, filename);

		mm_->setRootPage(rootPageId);
		mm_->deallocatePage(rootPageId);
	}

	Processor::~Processor() {
		Console::release();

#ifdef R_UM_OPT_ENABLE_LOGGING
#ifdef R_UM_OPT_PRC_DEBUG
		delete[] logmsg_;
		delete log_;
#endif
#endif

		delete reg_;
		delete mm_;
	}

	void Processor::run() {
		while (execInstruction()) {}
	}

	Byte Processor::execInstruction() {
#ifdef R_UM_OPT_ENABLE_LOGGING
#ifdef R_UM_OPT_PRC_DEBUG
		ic_ ++;
#endif
#endif
		Word instr = (*mm_)(0, pc_ ++);
		switch (decodeInstruction(instr)) {
			case IC_CNDMOV:
				return CNDMOV(decodeAreg(instr), decodeBreg(instr),
						decodeCreg(instr));
			case IC_ARRIDX:
				return ARRIDX(decodeAreg(instr), decodeBreg(instr),
						decodeCreg(instr));
			case IC_ARRAMD:
				return ARRAMD(decodeAreg(instr), decodeBreg(instr),
						decodeCreg(instr));
			case IC_ADD:
				return ADD(decodeAreg(instr), decodeBreg(instr),
						decodeCreg(instr));
			case IC_MUL:
				return MUL(decodeAreg(instr), decodeBreg(instr),
						decodeCreg(instr));
			case IC_DIV:
				return DIV(decodeAreg(instr), decodeBreg(instr),
						decodeCreg(instr));
			case IC_NOTAND:
				return NOTAND(decodeAreg(instr), decodeBreg(instr),
						decodeCreg(instr));

			case IC_HLT:
				return HLT();
			case IC_ALLOC:
				return ALLOC(decodeBreg(instr), decodeCreg(instr));
			case IC_ABAND:
				return ABAND(decodeCreg(instr));
			case IC_OUTP:
				return OUTP(decodeCreg(instr));
			case IC_INP:
				return INP(decodeCreg(instr));
			case IC_LDPRG:
				return LDPRG(decodeBreg(instr), decodeCreg(instr));

			case IC_ORTHO:
				return ORTHO(decodeOreg(instr), decodeVal(instr));

			default:
				assert(false);
				return HLT();
		}
	}

	Byte Processor::decodeInstruction(Word instr) const {
		return instr >> IC_BIT_OFFSET;
	}

	Byte Processor::decodeAreg(Word instr) const {
		return (instr >> REG_A_BIT_OFFSET) & ((1 << REG_BIT_WIDTH) - 1);
	}

	Byte Processor::decodeBreg(Word instr) const {
		return (instr >> REG_B_BIT_OFFSET) & ((1 << REG_BIT_WIDTH) - 1);
	}

	Byte Processor::decodeCreg(Word instr) const {
		return (instr >> REG_C_BIT_OFFSET) & ((1 << REG_BIT_WIDTH) - 1);
	}

	Byte Processor::decodeOreg(Word instr) const {
		return (instr >> REG_O_BIT_OFFSET) & ((1 << REG_BIT_WIDTH) - 1);
	}

	Word Processor::decodeVal(Word instr) const {
		return instr & ((1 << VAL_BIT_WIDTH) - 1);
	}

	bool Processor::isValidMemMan() const {
		return mm_ != NULL;
	}

	bool Processor::isValidRegisterBlock() const {
		return reg_ != NULL;
	}

	Byte Processor::CNDMOV(Byte A, Byte B, Byte C) {
#ifdef R_UM_OPT_ENABLE_LOGGING
#ifdef R_UM_OPT_PRC_DEBUG
		std::snprintf(logmsg_, msglen_,
				"[%8x] [%8x] [%11o] CNDMOV %1o[%8x] %1o[%8x] %1o[%8x]\n",
				ic_, pc_ - 1, (*mm_)(0, pc_ -1), A, (*reg_)[A],
				B, (*reg_)[B], C, (*reg_)[C]);
		log_->log(logmsg_);
#endif
#endif
		if ((*reg_)[C]) (*reg_)[A] = (*reg_)[B];

		return true;
	}

	Byte Processor::ARRIDX(Byte A, Byte B, Byte C) {
#ifdef R_UM_OPT_ENABLE_LOGGING
#ifdef R_UM_OPT_PRC_DEBUG
		std::snprintf(logmsg_, msglen_,
				"[%8x] [%8x] [%11o] ARRIDX %1o[%8x] %1o[%8x] %1o[%8x]\n",
				ic_, pc_ - 1, (*mm_)(0, pc_ -1), A, (*reg_)[A],
				B, (*reg_)[B], C, (*reg_)[C]);
		log_->log(logmsg_);
#endif
#endif
		(*reg_)[A] = (*mm_)((*reg_)[B], (*reg_)[C]);

		return true;
	}

	Byte Processor::ARRAMD(Byte A, Byte B, Byte C) {
#ifdef R_UM_OPT_ENABLE_LOGGING
#ifdef R_UM_OPT_PRC_DEBUG
		std::snprintf(logmsg_, msglen_,
				"[%8x] [%8x] [%11o] ARRAMD %1o[%8x] %1o[%8x] %1o[%8x]\n",
				ic_, pc_ - 1, (*mm_)(0, pc_ -1), A, (*reg_)[A],
				B, (*reg_)[B], C, (*reg_)[C]);
		log_->log(logmsg_);
#endif
#endif
		mm_->getWritable((*reg_)[A], (*reg_)[B]) = (*reg_)[C];

		return true;
	}

	Byte Processor::ADD(Byte A, Byte B, Byte C) {
#ifdef R_UM_OPT_ENABLE_LOGGING
#ifdef R_UM_OPT_PRC_DEBUG
		std::snprintf(logmsg_, msglen_,
				"[%8x] [%8x] [%11o] ADD    %1o[%8x] %1o[%8x] %1o[%8x]\n",
				ic_, pc_ - 1, (*mm_)(0, pc_ -1), A, (*reg_)[A],
				B, (*reg_)[B], C, (*reg_)[C]);
		log_->log(logmsg_);
#endif
#endif
		(*reg_)[A] = (*reg_)[B] + (*reg_)[C];

		return true;
	}

	Byte Processor::MUL(Byte A, Byte B, Byte C) {
#ifdef R_UM_OPT_ENABLE_LOGGIN
#ifdef R_UM_OPT_PRC_DEBUG
		std::snprintf(logmsg_, msglen_,
				"[%8x] [%8x] [%11o] MUL    %1o[%8x] %1o[%8x] %1o[%8x]\n",
				ic_, pc_ - 1, (*mm_)(0, pc_ -1), A, (*reg_)[A],
				B, (*reg_)[B], C, (*reg_)[C]);
		log_->log(logmsg_);
#endif
#endif
		(*reg_)[A] = (*reg_)[B] * (*reg_)[C];

		return true;
	}

	Byte Processor::DIV(Byte A, Byte B, Byte C) {
#ifdef R_UM_OPT_ENABLE_LOGGING
#ifdef R_UM_OPT_PRC_DEBUG
		std::snprintf(logmsg_, msglen_,
				"[%8x] [%8x] [%11o] DIV    %1o[%8x] %1o[%8x] %1o[%8x]\n",
				ic_, pc_ - 1, (*mm_)(0, pc_ -1), A, (*reg_)[A],
				B, (*reg_)[B], C, (*reg_)[C]);
		log_->log(logmsg_);
#endif
#endif
		assert((*reg_)[C] != 0);

		(*reg_)[A] = (*reg_)[B] / (*reg_)[C];

		return true;
	}

	Byte Processor::NOTAND(Byte A, Byte B, Byte C) {
#ifdef R_UM_OPT_ENABLE_LOGGING
#ifdef R_UM_OPT_PRC_DEBUG
		std::snprintf(logmsg_, msglen_,
				"[%8x] [%8x] [%11o] NOTAND %1o[%8x] %1o[%8x] %1o[%8x]\n",
				ic_, pc_ - 1, (*mm_)(0, pc_ -1), A, (*reg_)[A],
				B, (*reg_)[B], C, (*reg_)[C]);
		log_->log(logmsg_);
#endif
#endif
		(*reg_)[A] = ~ ((*reg_)[B] & (*reg_)[C]);

		return true;
	}

	Byte Processor::HLT() {
#ifdef R_UM_OPT_ENABLE_LOGGING
#ifdef R_UM_OPT_PRC_DEBUG
		std::snprintf(logmsg_, msglen_,
				"[%8x] [%8x] [%11o] HLT   \n",
				ic_, pc_ - 1, (*mm_)(0, pc_ -1));
		log_->log(logmsg_);
#endif
#endif
		return false;
	}

	Byte Processor::ALLOC(Byte B, Byte C) {
#ifdef R_UM_OPT_ENABLE_LOGGING
#ifdef R_UM_OPT_PRC_DEBUG
		std::snprintf(logmsg_, msglen_,
				"[%8x] [%8x] [%11o] ALLOC  %1o[%8x] %1o[%8x]\n",
				ic_, pc_ - 1, (*mm_)(0, pc_ -1), B, (*reg_)[B],
				C, (*reg_)[C]);
		log_->log(logmsg_);
#endif
#endif
		(*reg_)[B] = mm_->allocatePage((*reg_)[C]);

		return true;
	}

	Byte Processor::ABAND(Byte C) {
#ifdef R_UM_OPT_ENABLE_LOGGING
#ifdef R_UM_OPT_PRC_DEBUG
		std::snprintf(logmsg_, msglen_,
				"[%8x] [%8x] [%11o] ABAND  %1o[%8x]\n",
				ic_, pc_ - 1, (*mm_)(0, pc_ -1), C, (*reg_)[C]);
		log_->log(logmsg_);
#endif
#endif
		mm_->deallocatePage((*reg_)[C]);

		return true;
	}

	Byte Processor::OUTP(Byte C) {
#ifdef R_UM_OPT_ENABLE_LOGGING
#ifdef R_UM_OPT_PRC_DEBUG
		std::snprintf(logmsg_, msglen_,
				"[%8x] [%8x] [%11o] OUTP   %1o[%8x]\n",
				ic_, pc_ - 1, (*mm_)(0, pc_ -1), C, (*reg_)[C]);
		log_->log(logmsg_);
#endif
#endif
		Console::putByte((*reg_)[C]);

		return true;
	}

	Byte Processor::INP(Byte C) {
#ifdef R_UM_OPT_ENABLE_LOGGING
#ifdef R_UM_OPT_PRC_DEBUG
		std::snprintf(logmsg_, msglen_,
				"[%8x] [%8x] [%11o] INP    %1o[%8x]\n",
				ic_, pc_ - 1, (*mm_)(0, pc_ -1), C, (*reg_)[C]);
		log_->log(logmsg_);
#endif
#endif
		(*reg_)[C] = Console::getByte();

		return true;
	}

	Byte Processor::LDPRG(Byte B, Byte C) {
#ifdef R_UM_OPT_ENABLE_LOGGING
#ifdef R_UM_OPT_PRC_DEBUG
		std::snprintf(logmsg_, msglen_,
				"[%8x] [%8x] [%11o] LDPRG  %1o[%8x] %1o[%8x]\n",
				ic_, pc_ - 1, (*mm_)(0, pc_ -1), B, (*reg_)[B],
				C, (*reg_)[C]);
		log_->log(logmsg_);
#endif
#endif
		pc_ = (*reg_)[C];

		if (! (*reg_)[B]) return true;
		
		mm_->setRootPage((*reg_)[B]);

		return true;
	}

	Byte Processor::ORTHO(Byte O, Word V) {
#ifdef R_UM_OPT_ENABLE_LOGGING
#ifdef R_UM_OPT_PRC_DEBUG
		std::snprintf(logmsg_, msglen_,
				"[%8x] [%8x] [%11o] ORTHO  %1o[%8x]  {%8x}\n",
				ic_, pc_ - 1, (*mm_)(0, pc_ -1), O, (*reg_)[O], V);
		log_->log(logmsg_);
#endif
#endif
		(*reg_)[O] = V;

		return true;
	}

R_UM_NS_END
