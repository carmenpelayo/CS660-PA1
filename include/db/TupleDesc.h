#ifndef DB_TUPLEDESC_H
#define DB_TUPLEDESC_H

#include <db/Type.h>
#include <string>
#include <stdexcept>
#include <utility>
#include <vector>

namespace db {
    /**
     * A helper class to facilitate organizing the information of each field
     */
    class TDItem {
    public:
        /**
         * The type of the field
         */
        Types::Type fieldType; //Types::Type can be either INT_TYPE or STRING_TYPE

        /**
         * The name of the field
         */
        std::string fieldName; //name of the field (in string)

        TDItem(Types::Type t, std::string n) : fieldType(t), fieldName(std::move(n)) {};

        bool operator==(const TDItem &other) const;

        bool operator!=(const TDItem &other) const {return !(*this == other);}

        [[nodiscard]] std::string to_string() const {
            return fieldName + "(" + Types::to_string(fieldType) + ")"; }; //E.G. "name(INT_TYPE)"
    };

    /**
     * TupleDesc describes the schema of a tuple.
     */
    class TupleDesc {
        using iterator = std::vector<TDItem>::const_iterator;

    private:
        std::vector<TDItem> fieldDescriptions; // Container to store field descriptions

    public:
        TupleDesc() = default;

        /**
         * Create a new TupleDesc with types.length fields with fields of the
         * specified types, with associated named fields.
         *
         * @param types
         *            array specifying the number of and types of fields in this
         *            TupleDesc. It must contain at least one entry.
         * @param names
         *            array specifying the names of the fields.
         */
        TupleDesc(const std::vector<Types::Type> &types, const std::vector<std::string> &names);

        /**
         * Constructor. Create a new tuple desc with typeAr.length fields with
         * fields of the specified types, with anonymous (unnamed) fields.
         *
         * @param typeAr
         *            array specifying the number of and types of fields in this
         *            TupleDesc. It must contain at least one entry.
         */
        explicit TupleDesc(const std::vector<Types::Type> &types); //no seria &typeAr?

        /**
         * @return the number of fields in this TupleDesc
         */
        [[nodiscard]] size_t numFields() const;

        /**
         * Gets the field name of the ith field of this TupleDesc.
         *
         * @param i
         *            index of the field name to return. It must be a valid index.
         * @return the name of the ith field
         */
        [[nodiscard]] std::string getFieldName(size_t i) const;

        /**
         * Gets the type of the ith field of this TupleDesc.
         *
         * @param i
         *            The index of the field to get the type of. It must be a valid
         *            index.
         * @return the type of the ith field
         */
        [[nodiscard]] Types::Type getFieldType(size_t i) const;

        /**
         * Find the index of the field with a given name.
         *
         * @param name
         *            name of the field.
         * @return the index of the field that is first to have the given name.
         */
        [[nodiscard]] int fieldNameToIndex(const std::string &fieldName) const;

        /**
         * @return The size (in bytes) of tuples corresponding to this TupleDesc.
         *         Note that tuples from a given TupleDesc are of a fixed size.
         */
        [[nodiscard]] size_t getSize() const;

        /**
         * Merge two TupleDescs into one, with td1.numFields + td2.numFields fields,
         * with the first td1.numFields coming from td1 and the remaining from td2.
         *
         * @param td1
         *            The TupleDesc with the first fields of the new TupleDesc
         * @param td2
         *            The TupleDesc with the last fields of the TupleDesc
         * @return the new TupleDesc
         */
        static TupleDesc merge(const TupleDesc &td1, const TupleDesc &td2);

        /**
         * Compares the specified object with this TupleDesc for equality. Two
         * TupleDescs are considered equal if they are the same size and if the n-th
         * type in this TupleDesc is equal to the n-th type in td.
         *
         * @param o
         *            the Object to be compared for equality with this TupleDesc.
         * @return true if the object is equal to this TupleDesc.
         */
        bool operator==(const TupleDesc &other) const;

        bool operator!=(const TupleDesc &other) const { return !(*this == other); }

        /**
         * Returns a string describing this descriptor. It should be of the form
         * "fieldType[0](fieldName[0]), ..., fieldType[M](fieldName[M])", although
         * the exact format does not matter.
         *
         * @return String describing this descriptor.
         */
        [[nodiscard]] std::string to_string() const;

        [[nodiscard]] iterator begin() const;

        [[nodiscard]] iterator end() const;
    };
}

template<>
struct std::hash<db::TDItem> {
    std::size_t operator()(const db::TDItem &o) const;
};

template<>
struct std::hash<db::TupleDesc> {
    std::size_t operator()(const db::TupleDesc &o) const;
};

#endif
