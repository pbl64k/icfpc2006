#ifndef R_UM_HDR_MEM

#define R_UM_HDR_MEM

#include <vector>
#include <stack>

#include "um.hpp"

#include "blk.hpp"
#include "log.hpp"

R_UM_NS_START

	class MemMan {
		friend class Processor;
		friend class TestSuite;
		friend class PageLoader;
		protected:
			MemMan(): pages_(new PageStorage), ids_(new RecycledIdList),
					rootPage_(NULL), nextPage_(1) {
#ifdef R_UM_OPT_ENFORCE_LOWLEV_VALIDITY
				assert(isValidBlock());
#endif
			}
		
			~MemMan() {
#ifdef R_UM_OPT_ENFORCE_LOWLEV_VALIDITY
				assert(isValidBlock());
#endif
		
				delete ids_;
		
				for (PageStorage::iterator i = pages_->begin();
						i != pages_->end(); ++ i)
					if (*i != NULL) delete *i;
		
				delete pages_;
			}
		
			Word allocatePage(Word blockSize) {
				Word nextPage = getNextPage();
		
				getNewPage(nextPage) = new LazyBlock(blockSize);
		
				return nextPage;
			}
		
			void deallocatePage(Word pageId) {
#ifdef R_UM_OPT_ENFORCE_LOWLEV_VALIDITY
				assert(isValidPage(pageId));
#endif
		
				delete getPage(pageId);
				getPage(pageId) = NULL;
				ids_->push(pageId);
			}
		
			void setRootPage(Word pageId) {
				if (pageId) {
					if (isValidRootPage()) deallocateRootPage();
					rootPage_ = copyPage(pageId);
				}
		
#ifdef R_UM_OPT_ENFORCE_LOWLEV_VALIDITY
				assert(isValidRootPage());
#endif
			}
		
			const Word &operator()(Word pageId, Word offset) const {
				return (*getPage(pageId))[offset];
			}
		
			Word &getWritable(Word pageId, Word offset) {
				return (*getPage(pageId))[offset];
			}

		private:
			void deallocateRootPage() {
#ifdef R_UM_OPT_ENFORCE_LOWLEV_VALIDITY
				assert(isValidRootPage());
#endif
		
				delete getRawPage(rootPage_);
				getRawPage(rootPage_) = NULL;
				ids_->push(rootPage_);
		
#ifdef R_UM_OPT_ENFORCE_LOWLEV_VALIDITY
				assert(! isValidRootPage());
#endif
			}
		
			Word copyPage(Word pageId) {
				Word nextPage = getNextPage();
		
				getNewPage(nextPage) = new LazyBlock(*getPage(pageId));
		
				return nextPage;
			}
		
			LazyBlock const *getPage(Word pageId) const {
				pageId = checkPage(pageId);
		
#ifdef R_UM_OPT_ENFORCE_LOWLEV_VALIDITY
				assert(pages_->size() > pageId);
				assert(isValidPage(pageId));
#endif
		
				return getRawPage(pageId);
			}
		
			LazyBlock *&getPage(Word pageId) {
				pageId = checkPage(pageId);
		
#ifdef R_UM_OPT_ENFORCE_LOWLEV_VALIDITY
				assert(pages_->size() > pageId);
				assert(isValidPage(pageId));
#endif
		
				return getRawPage(pageId);
			}
		
			LazyBlock *&getNewPage(Word pageId) {
				pageId = checkPage(pageId);
		
				while (pageId >= pages_->size())
					pages_->resize(pages_->size() + vecGrowth_, NULL);
		
				return getRawPage(pageId);
			}
		
			LazyBlock const *getRawPage(Word pageId) const {
				return pages_->at(pageId);
			}
		
			LazyBlock *&getRawPage(Word pageId) {
				return pages_->at(pageId);
			}
		
			Word checkPage(Word pageId) const {
				if (! pageId) pageId = rootPage_;
		
#ifdef R_UM_OPT_ENFORCE_LOWLEV_VALIDITY
				assert(pageId);
#endif
		
				return pageId;
			}
		
			Word getNextPage() {
				if (ids_->size()) {
					Word candidateId = ids_->top();
					ids_->pop();
					return candidateId;
				}
		
#ifdef R_UM_OPT_ENFORCE_LOWLEV_VALIDITY
				Word curPage = nextPage_;
#endif
		
				while (getNewPage(nextPage_) != NULL) {
					++ nextPage_;
		
					if ((! nextPage_ ) || (nextPage_ == rootPage_)) {
						++ nextPage_;
						continue;
					}
		
#ifdef R_UM_OPT_ENFORCE_LOWLEV_VALIDITY
					assert(curPage != nextPage_);
#endif
				}
		
#ifdef R_UM_OPT_ENFORCE_LOWLEV_VALIDITY
				assert(nextPage_);
#endif
		
				return nextPage_ ++;
			}
		
			bool isValidBlock() const {
#ifdef R_UM_OPT_ENFORCE_LOWLEV_VALIDITY
				return (pages_ != NULL) && (ids_ != NULL);
#else
				return false;
#endif
			}
		
			bool isValidRootPage() const {
				return isValidPage(rootPage_);
			}
		
			bool isValidPage(Word pageId) const {
				return pageId && getRawPage(pageId) != NULL;
			}

			static const Word vecGrowth_ = 0x1000;

			typedef std::vector<LazyBlock *> PageStorage;
			typedef std::stack<Word> RecycledIdList;

			PageStorage *pages_;
			RecycledIdList *ids_;
			Word rootPage_;
			Word nextPage_;
	};

R_UM_NS_END

#endif
