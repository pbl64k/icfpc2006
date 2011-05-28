#ifndef R_UM_HDR_BLK

#define R_UM_HDR_BLK

#include "um.hpp"

#include "raw.hpp"
#include "log.hpp"

R_UM_NS_START

	class LazyBlock {
		friend class TestSuite;
		friend class MemMan;
		protected:
			LazyBlock(Word blockSize): block_(new RawBlock(blockSize)) {
#ifdef R_UM_OPT_ENFORCE_LOWLEV_VALIDITY
				assert(isValidBlock());
#endif
			}
		
			LazyBlock(const LazyBlock &sourceBlock):
					block_(sourceBlock.block_) {
#ifdef R_UM_OPT_ENFORCE_LOWLEV_VALIDITY
				assert(isValidBlock());
#endif
		
				block_->grab();
			}
		
			~LazyBlock() {
#ifdef R_UM_OPT_ENFORCE_LOWLEV_VALIDITY
				assert(isValidBlock());
#endif
		
				block_->release();
			}
		
			const Word &operator[](Word offset) const {
#ifdef R_UM_OPT_ENFORCE_LOWLEV_VALIDITY
				assert(isValidBlock());
#endif
		
				return (*static_cast<RawBlock const *>(block_))[offset];
			}

			Word &operator[](Word offset) {
#ifdef R_UM_OPT_ENFORCE_LOWLEV_VALIDITY
				assert(isValidBlock());
#endif
				
				makeWritable();
				return (*block_)[offset];
			}

		private:
			void makeWritable() {
#ifdef R_UM_OPT_ENFORCE_LOWLEV_VALIDITY
				assert(isValidBlock());
#endif
		
				if (block_->isWritable()) return;
		
				RawBlock *oldBlock = block_;
				block_ = new RawBlock(*oldBlock);
				oldBlock->release();
		
#ifdef R_UM_OPT_ENFORCE_LOWLEV_VALIDITY
				assert(isValidBlock());
				assert(block_->isWritable());
#endif
			}
		
			bool isValidBlock() const {
#ifdef R_UM_OPT_ENFORCE_LOWLEV_VALIDITY
				return block_ != NULL;
#else
				return false;
#endif
			}

			RawBlock *block_;
	};

R_UM_NS_END

#endif
