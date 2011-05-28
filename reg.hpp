#ifndef R_UM_HDR_REG

#define R_UM_HDR_REG

#include <vector>

#include "um.hpp"

#include "raw.hpp"

R_UM_NS_START

	class RegisterBlock : public RawBlock {
		friend class Processor;
		friend class TestSuite;
		protected:
			RegisterBlock(Byte blockSize): RawBlock(blockSize) {
			}
	};

R_UM_NS_END

#endif
