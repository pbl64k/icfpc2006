#ifndef R_UM_HDR_END

#define R_UM_HDR_END

#include "um.hpp"

R_UM_NS_START

	class EndiannessFixer {
		friend class TestSuite;
		friend class PageLoader;
		protected:
			static void fixEndianness(Word *block, Word size);
	};

R_UM_NS_END

#endif
