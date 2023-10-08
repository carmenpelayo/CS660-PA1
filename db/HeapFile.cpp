#include <db/HeapFile.h>
#include <db/TupleDesc.h>
#include <db/Page.h>
#include <db/PageId.h>
#include <db/HeapPage.h>
#include <stdexcept>
#include <utility>
#include <sys/stat.h>
#include <fcntl.h>

using namespace db;

//
// HeapFile
//

HeapFile::HeapFile(const char *fname, TupleDesc td): fname(fname), td(std::move(td)) {
}

int HeapFile::getId() const {
    return std::hash<std::string>{}(fname);
}

const TupleDesc &HeapFile::getTupleDesc() const {
    return td;
}

Page *HeapFile::readPage(const PageId &pid) const {
    int pageSize = Database::getBufferPool().getPageSize();
    int offset = pid.pageNumber() * pageSize;

    std::ifstream file(fname, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file for reading.");
    }

    file.seekg(offset);
    auto* data = new uint8_t[pageSize];
    file.read(reinterpret_cast<char*>(data), pageSize);

    HeapPageId hpid(getId(), pid.pageNumber());
    return new HeapPage(hpid, data);
}

int HeapFile::getNumPages() const {
    std::ifstream file(fname, std::ios::binary | std::ios::ate);
    int fileSize = file.tellg();
    return fileSize / Database::getBufferPool().getPageSize();
}

HeapFileIterator HeapFile::begin() const {
    return {this, 0};
}

HeapFileIterator HeapFile::end() const {
    return {this, getNumPages()};
}

//
// HeapFileIterator
//

HeapFileIterator::HeapFileIterator(const HeapFile *heapFile, int currentPage)
    : heapFile(heapFile), currentPage(currentPage) {
}

bool HeapFileIterator::operator!=(const HeapFileIterator &other) const {
    return currentPage != other.currentPage || heapFile != other.heapFile;
}

Tuple &HeapFileIterator::operator*() {
    // Use HeapPageId to construct the specific type of PageId
    //HeapPageId hpid(heapFile->getId(), currentPage);

    // Fetch the current page
    //HeapPage *page = dynamic_cast<HeapPage*>(heapFile->readPage(hpid));

    // This assumes HeapPage has an appropriate method to fetch the tuple by index
    //return page->getTuple(currentTupleIndex);

    // First, read the page using the current page number
    Page *pagePtr = heapFile->readPage(HeapPageId(heapFile->getId(), currentPage));
    HeapPage *page = dynamic_cast<HeapPage*>(pagePtr);

    // Next, retrieve the tuple from the page using the iterator
    HeapPageIterator it = page->begin();
    return *it;
}

HeapFileIterator &HeapFileIterator::operator++() {
    // First, read the page using the current page number
    Page *pagePtr = heapFile->readPage(HeapPageId(heapFile->getId(), currentPage));
    HeapPage *page = dynamic_cast<HeapPage*>(pagePtr);

    // Increment the internal page iterator
    HeapPageIterator it = page->begin();
    ++it;

    // If we've reached the end of the current page, go to the next page
    if (it != page->end()) {
        currentPage++;
    }

    return *this;
}
