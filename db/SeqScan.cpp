#include <db/SeqScan.h>

using namespace db;
SeqScan::SeqScan(TransactionId *tid, int tableid, const std::string &tableAlias) {
    this->tid = tid;
    this->tableid = tableid;
    this->tableAlias = tableAlias;
    this->tupleDesc = Database::getCatalog().getTupleDesc(tableid);
}

std::string SeqScan::getTableName() const {
    return Database::getCatalog().getTableName(tableid);
}

std::string SeqScan::getAlias() const {
    return tableAlias;
}

void SeqScan::reset(int tabid, const std::string &tabAlias) {
    this->tableid = tabid;
    this->tableAlias = tabAlias;
    this->tupleDesc = Database::getCatalog().getTupleDesc(tableid);
}

const TupleDesc &SeqScan::getTupleDesc() const {
    return tupleDesc;
}

int SeqScan::getNumFields() const {
    return tupleDesc.numFields();
}

int SeqScan::getTableId() const {
    return tableid;
}

TransactionId* SeqScan::getTransactionId() const {
    return tid;
}

SeqScan::iterator SeqScan::begin() const {
    return {this, true};
}

SeqScan::iterator SeqScan::end() const {
    return {this, false};
}

//
// SeqScanIterator
//

SeqScanIterator::SeqScanIterator(const SeqScan *scan, bool isBegin) {
    this->scan = scan;
    this->isBegin = isBegin;
    if (isBegin) {
        currentPageIndex = 0; // Start with the first page
        currentTupleIndex = 0;
    } else {
        currentPageIndex = -1; // Indicates the end of the scan
        currentTupleIndex = -1;
    }
}

bool SeqScanIterator::operator!=(const SeqScanIterator &other) const {
    return currentPageIndex != other.currentPageIndex || currentTupleIndex != other.currentTupleIndex;
}

SeqScanIterator &SeqScanIterator::operator++() {
    if (currentPageIndex >= 0) {
        currentTupleIndex++;
        if (currentTupleIndex >= scan->getNumFields()) {
            // Move to the next page if we have scanned all tuples on the current page
            currentTupleIndex = 0;
            currentPageIndex++;
            if (currentPageIndex >= scan->getNumFields()) {
                // Reached the end of the scan
                currentPageIndex = -1;
            }
        }
    }
    return *this;
}

const Tuple &SeqScanIterator::operator*() const {
    if (currentPageIndex >= 0) {
        // Create a HeapPageId using the table ID and current page index
        HeapPageId pageId(scan->getTableId(), currentPageIndex);

        // Fetch and return the current tuple
        Page *page = Database::getBufferPool().getPage(*scan->getTransactionId(), &pageId);
        Tuple *tuple = reinterpret_cast<Tuple *>(page->getPageData());
        return tuple[currentTupleIndex];
    } else {
        // Return an empty tuple to indicate the end of the scan
        return Tuple();
    }
}
