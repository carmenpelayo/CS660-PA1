#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <db/TupleDesc.h>
#include <db/Page.h>
#include <db/PageId.h>
#include <db/TransactionId.h>
#include <db/HeapPage.h>

namespace db {
    class HeapFile;
    class HeapFileIterator {
    private:
        const HeapFile *heapFile;
        int currentPage;
        int currentTupleIndex;
        //HeapPageIterator tupleIter;

    public:
        HeapFileIterator(const HeapFile *heapFile, int currentPage);
        bool operator!=(const HeapFileIterator &other) const;
        Tuple &operator*();
        HeapFileIterator &operator++();
    };

    /**
     * HeapFile is an implementation of a DbFile that stores a collection of tuples
     * in no particular order. Tuples are stored on pages, each of which is a fixed
     * size, and the file is simply a collection of those pages. HeapFile works
     * closely with HeapPage. The format of HeapPages is described in the HeapPage
     * constructor.
     *
     * @see db::HeapPage::HeapPage
     * @author Sam Madden
     */
    class HeapFile : public DbFile {
        const char *fname;
        TupleDesc td;

    public:

        /**
         * Constructs a heap file backed by the specified file.
         *
         * @param f the file that stores the on-disk backing store for this heap file.
         */
        HeapFile(const char *fname, TupleDesc td);

        /**
         * Returns an ID uniquely identifying this HeapFile. Implementation note:
         * you will need to generate this tableid somewhere ensure that each
         * HeapFile has a "unique id," and that you always return the same value for
         * a particular HeapFile. We suggest hashing the absolute file name of the
         * file underlying the heapfile, i.e. f.getAbsoluteFile().hashCode().
         * @return an ID uniquely identifying this HeapFile.
         */
        [[nodiscard]] int getId() const override;

        /**
         * Returns the TupleDesc of the table stored in this DbFile.
         * @return TupleDesc of this DbFile.
         */
        [[nodiscard]] const TupleDesc &getTupleDesc() const override;

        Page *readPage(const PageId &pid) const override;

        /**
         * Returns the number of pages in this HeapFile.
         */
        int getNumPages() const;

        [[nodiscard]] HeapFileIterator begin() const;

        [[nodiscard]] HeapFileIterator end() const;
    };
}