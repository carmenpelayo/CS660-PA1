#include <db/TupleDesc.h>

using namespace db;

//
// TDItem
//

bool TDItem::operator==(const TDItem &other) const {
    return (fieldType == other.fieldType) && (fieldName == other.fieldName);
}

std::size_t std::hash<TDItem>::operator()(const TDItem &r) const {
        // Hash the TDItem based on its fieldType and fieldName
    std::size_t hash1 = std::hash<Types::Type>()(r.fieldType);
    std::size_t hash2 = std::hash<std::string>()(r.fieldName);
    return hash1 ^ (hash2 << 1); // Combine the hashes
}

//
// TupleDesc
//

TupleDesc::TupleDesc(const std::vector<Types::Type> &types) {
    // Create a TupleDesc with anonymous (unnamed) fields
    for (auto type : types) {
        TDItem fieldDesc(type, "");
        fieldDescriptions.push_back(fieldDesc);
    }
}

TupleDesc::TupleDesc(const std::vector<Types::Type> &types, const std::vector<std::string> &names) {
    // Check that the sizes of 'types' and 'names' vectors match
    if (types.size() != names.size()) {
        throw std::invalid_argument("Types and names vectors must have the same size");
    }
    // Initialize the field descriptions using 'types' and 'names'
    for (size_t i = 0; i < types.size(); ++i) {
        TDItem fieldDesc(types[i], names[i]);
        fieldDescriptions.push_back(fieldDesc);
    }
}

size_t TupleDesc::numFields() const {
    return fieldDescriptions.size();
}

std::string TupleDesc::getFieldName(size_t i) const {
    if (i >= fieldDescriptions.size()) {
        throw std::out_of_range("Index out of range");
    }
    return fieldDescriptions[i].fieldName;
}

Types::Type TupleDesc::getFieldType(size_t i) const {
    if (i >= fieldDescriptions.size()) {
        throw std::out_of_range("Index out of range");
    }
    return fieldDescriptions[i].fieldType;
}

int TupleDesc::fieldNameToIndex(const std::string &fieldName) const {
    for (size_t i = 0; i < fieldDescriptions.size(); i++) {
        if (fieldDescriptions[i].fieldName == fieldName) {
            return static_cast<int>(i);
        }
    }
    return -1; // Field not found
}

size_t TupleDesc::getSize() const {
    size_t totalSize = 0;

    // Calculate the total size by summing the size of each field
    for (const TDItem &field : fieldDescriptions) {
        totalSize += Types::getLen(field.fieldType);
    }
    return totalSize;
}

TupleDesc TupleDesc::merge(const TupleDesc &td1, const TupleDesc &td2) {
    TupleDesc merged_td = td1; // Copy the x2 vector of td1
    for (const auto & fieldDescription : td2.fieldDescriptions) {
        merged_td.fieldDescriptions.push_back(fieldDescription);
    }
    return merged_td;
}

std::string TupleDesc::to_string() const {
    std::string desc;
    for (size_t i = 0; i < fieldDescriptions.size(); i++) {
        if (i > 0) {
            desc += ", ";
        }
        desc += Types::to_string(fieldDescriptions[i].fieldType) + "(" + fieldDescriptions[i].fieldName + ")";
    }
    return desc;
}

bool TupleDesc::operator==(const TupleDesc &other) const {
    return fieldDescriptions == other.fieldDescriptions;
}

TupleDesc::iterator TupleDesc::begin() const {
    return fieldDescriptions.begin();
}

TupleDesc::iterator TupleDesc::end() const {
    return fieldDescriptions.end();
}

std::size_t std::hash<db::TupleDesc>::operator()(const db::TupleDesc &td) const {
    /// Hash the TupleDesc based on its field types and field names
    std::size_t hash = 0;
    for (size_t i = 0; i < td.numFields(); i++) {
        std::size_t fieldTypeHash = std::hash<Types::Type>()(td.getFieldType(i));
        std::size_t fieldNameHash = std::hash<std::string>()(td.getFieldName(i));
        hash ^= (fieldTypeHash ^ (fieldNameHash << 1));
    }
    return hash;
}
