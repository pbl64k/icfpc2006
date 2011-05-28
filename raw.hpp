#ifndef R_UM_HDR_RAW

#define R_UM_HDR_RAW

#include <cstring>

#include "um.hpp"

#include "log.hpp"

R_UM_NS_START

	class RawBlock {
		friend class TestSuite;
		friend class LazyBlock;
		protected:
			RawBlock(Word blockSize): refcnt_(1), size_(blockSize),
					block_(new Word[size_]) {
#ifdef R_UM_OPT_ENFORCE_LOWLEV_VALIDITY
				assert(isValidSize());
				assert(isValidBlock());
#endif
		
				std::memset(block_, 0, size_ * sizeof(Word));
			}
		
			RawBlock(const RawBlock &sourceBlock): refcnt_(1),
					size_(sourceBlock.size_), block_(new Word[size_]) {
#ifdef R_UM_OPT_ENFORCE_LOWLEV_VALIDITY
				assert(isValidSize());
				assert(isValidBlock());
				assert(sourceBlock.isValidBlock());
				assert(size_ == sourceBlock.size_);
#endif
		
				std::memcpy(block_, sourceBlock.block_,
						size_ * sizeof(Word));
			}
		
			~RawBlock() {
#ifdef R_UM_OPT_ENFORCE_LOWLEV_VALIDITY
				assert(isValidBlock());
				assert(! isValidRefcnt());
#endif
		
				delete[] block_;
			}
		
			const Word &operator[](Word offset) const {
#ifdef R_UM_OPT_ENFORCE_LOWLEV_VALIDITY
				assert(isWithinBounds(offset));
				assert(isReadable());
#endif
		
				return block_[offset];
			}
		
			Word &operator[](Word offset) {
#ifdef R_UM_OPT_ENFORCE_LOWLEV_VALIDITY
				assert(isWithinBounds(offset));
				assert(isWritable());
#endif
		
				return block_[offset];
			}
		
			void grab() {
#ifdef R_UM_OPT_ENFORCE_LOWLEV_VALIDITY
				assert(isValidRefcnt());
#endif
		
				++ refcnt_;
		
#ifdef R_UM_OPT_ENFORCE_LOWLEV_VALIDITY
				assert(isValidRefcnt());
#endif
			}
		
			void release() {
#ifdef R_UM_OPT_ENFORCE_LOWLEV_VALIDITY
				assert(isValidRefcnt());
#endif
		
				if (! (-- refcnt_)) delete this;
			}

			bool isReadable() const {
#ifdef R_UM_OPT_ENFORCE_LOWLEV_VALIDITY
				assert(isValidRefcnt());
				assert(isValidBlock());
#endif
		
				return true;
			}
		
			bool isWritable() const {
#ifdef R_UM_OPT_ENFORCE_LOWLEV_VALIDITY
				assert(isReadable());
#endif
		
				return refcnt_ == 1;
			}

		private:
			bool isValidRefcnt() const {
#ifdef R_UM_OPT_ENFORCE_LOWLEV_VALIDITY
				return refcnt_ > 0;
#else
				return false;
#endif
			}
		
			bool isValidSize() const {
#ifdef R_UM_OPT_ENFORCE_LOWLEV_VALIDITY
				return true;
#else
				return false;
#endif
			}
		
			bool isValidBlock() const {
#ifdef R_UM_OPT_ENFORCE_LOWLEV_VALIDITY
				return block_ != NULL;
#else
				return false;
#endif
			}
		
			bool isWithinBounds(Word offset) const {
#ifdef R_UM_OPT_ENFORCE_LOWLEV_VALIDITY
				return offset < size_;
#else
				return offset && false;
#endif
			}

			Word refcnt_;
			Word size_;
			Word *block_;
	};

R_UM_NS_END

#endif
