#include <db/Catalog.h>
#include <stdexcept>

using namespace db;

void Catalog::addTable(DbFile *file, const std::string &name, const std::string &pkeyField) {
    //Each table is constructed using the data in a file (DbFile). Therefore, the table ID becomes that file's ID.
    int tableId = file->getId();
    auto *table = new Table(file, name, pkeyField);
    tablesById[tableId] = table;
    idByName[name] = tableId;
}

int Catalog::getTableId(const std::string &name) const {
    //idByName is an unordered_map that matches the table name with the table ID.
    //Therefore, each pair(element) in idByName(map) is formed by a first(key)=name and second(val)=ID.
    auto it = idByName.find(name);     //.find() returns an iterator (it) to the element in the map whose key is equal to the table's name.
    if(it == idByName.end()) { //If the element is not found, .find() returns an iterator (it) to the end of the map.
        throw std::invalid_argument("Table not found.");
    }
    return it->second; //second = TableId
}

const TupleDesc &Catalog::getTupleDesc(int tableId) const {
    auto it = tablesById.find(tableId);
    if(it == tablesById.end()) {
        throw std::invalid_argument("Table ID not found.");
    }
    return it->second->file->getTupleDesc();
}

DbFile *Catalog::getDatabaseFile(int tableId) const {
    auto it = tablesById.find(tableId);
    if(it == tablesById.end()) {
        throw std::invalid_argument("Table ID not found.");
    }
    return it->second->file;
}

std::string Catalog::getPrimaryKey(int tableId) const {
    auto it = tablesById.find(tableId);
    if(it == tablesById.end()) {
        throw std::invalid_argument("Primary key not found.");
    }
    return it->second->pkeyField;
}

std::string Catalog::getTableName(int id) const {
    auto it = tablesById.find(id);
    if(it == tablesById.end()) {
        throw std::invalid_argument("Table name not found.");
    }
    return it->second->name;
}

void Catalog::clear() {
    tablesById.clear(); //tablesById in an unordered_map. Function unordered_map::clear() removes all elements from the container.
    idByName.clear();
}
