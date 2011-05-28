#include "end.hpp"

R_UM_NS_START

	void EndiannessFixer::fixEndianness(Word *block, Word size) {
#ifdef R_UM_OPT_LITTLE_ENDIAN_HARDWARE
		for (Word i = 0; i != size ; ++ i) {
			Word fixed = (block[i] & ((1 << 8) - 1)) << 24;
			fixed |= (block[i] & (((1 << 8) - 1) << 8)) << 8;
			fixed |= (block[i] & (((1 << 8) - 1) << 16)) >> 8;
			fixed |= (block[i] & (((1 << 8) - 1) << 24)) >> 24;
			block[i] = fixed;
		}
#endif
	}

R_UM_NS_END
