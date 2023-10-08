#include <db/BufferPool.h>
#include <db/Database.h>

using namespace db;

BufferPool::BufferPool(int numPages): capacity(numPages) {
    pageCache.reserve(numPages); // reserve(n) creates enough buckets in the unordered_map (pageCache) to hold at least n items.
}

Page *BufferPool::getPage(const TransactionId &tid, PageId *pid) {
    // Check if the page is in cache
    auto it = pageCache.find(pid);
    if(it != pageCache.end()) {
        // Move the page to the back of the LRU queue (recently accessed)
        auto lruIt = std::find(lruQueue.begin(), lruQueue.end(), pid);
        lruQueue.erase(lruIt);
        lruQueue.push_back(pid);
        return it->second;
    }

    // If not in cache, fetch from the disk
    DbFile* file = Database::getCatalog().getDatabaseFile(pid->getTableId());
    Page* page = file->readPage(*pid);

    // If buffer is full, evict a page
    if(pageCache.size() == capacity) {
        evictPage();
    }

    // Insert into cache and LRU queue
    pageCache[pid] = page;
    lruQueue.push_back(pid);

    return page;
}

void BufferPool::evictPage() {
    // do nothing for now
}
