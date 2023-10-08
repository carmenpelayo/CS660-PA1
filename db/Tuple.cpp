#include <db/Tuple.h>
#include <sstream>

using namespace db;

//
// Tuple
//

Tuple::Tuple(const TupleDesc &td, RecordId *rid): tupleDesc(td), recordId(rid) { //recordId yet to be defined
    fields.resize(td.numFields(), nullptr);
}

const TupleDesc &Tuple::getTupleDesc() const {
    return tupleDesc;
}

const RecordId *Tuple::getRecordId() const {
    return recordId; //recordId yet to be defined
}

void Tuple::setRecordId(const RecordId *id) {
    recordId = id; //recordId yet to be defined
}

const Field &Tuple::getField(int i) const {
    if (i < 0 || i >= fields.size()) {
        throw std::out_of_range("Field index out of range.");
    }
    return *fields[i];
}

void Tuple::setField(int i, const Field *f) {
    if (i < 0 || i >= fields.size()) {
        throw std::out_of_range("Field index out of range.");
    }
    fields[i] = f; //Error!!!!!
}

Tuple::iterator Tuple::begin() const {
    return fields.begin();
}

Tuple::iterator Tuple::end() const {
    return fields.end();
}

std::string Tuple::to_string() const {
    std::ostringstream oss;
    for (size_t i = 0; i < fields.size(); i++) {
        if (i != 0) {
            oss << ", ";
        }
        oss << fields[i]->to_string();
    }
    return oss.str();
}
