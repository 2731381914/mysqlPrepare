#include "PrepareResultCreator.h"

PrepareResultCreator::PrepareResultCreator(int paramCount) {
    this->bindArray = new MYSQL_BIND[paramCount];
    memset(this->bindArray, 0, sizeof(MYSQL_BIND) * paramCount);
    this->paramCount = paramCount;
}

bool PrepareResultCreator::validIndex(int index) {
    return index < this->paramCount;
}

bool PrepareResultCreator::bindInt32(int index, int &value) {
    if (index >= this->paramCount) {
        this->errorInfo("bindInt32 index out!");
        return false;
    }
    this->bindArray[index].buffer_type = MYSQL_TYPE_LONG;
    this->bindArray[index].buffer = &value;
    this->bindArray[index].is_unsigned = false;
    return true;
}

bool PrepareResultCreator::bindUInt32(int index, unsigned int &value) {
    if (!validIndex(index)) {
        this->errorInfo("bindUInt32 index out!");
        return false;
    }
    this->bindArray[index].buffer_type = MYSQL_TYPE_LONG;
    this->bindArray[index].buffer = &value;
    this->bindArray[index].is_unsigned = true;
    return true;
}

bool PrepareResultCreator::bindInt64(int index, long long &value) {
    if (!validIndex(index)) {
        this->errorInfo("bindInt64 index out!");
        return false;
    }
    this->bindArray[index].buffer_type = MYSQL_TYPE_LONGLONG;
    this->bindArray[index].buffer = &value;
    this->bindArray[index].is_unsigned = false;
    return true;
}

bool PrepareResultCreator::bindUInt64(int index, unsigned long long &value) {
    if (!validIndex(index)) {
        this->errorInfo("bindUInt64 index out!");
        return false;
    }
    this->bindArray[index].buffer_type = MYSQL_TYPE_LONGLONG;
    this->bindArray[index].buffer = &value;
    this->bindArray[index].is_unsigned = true;
    return true;
}

bool PrepareResultCreator::bindFloat(int index, float &value) {
    if (!validIndex(index)) {
        this->errorInfo("bindFloat index out!");
        return false;
    }
    this->bindArray[index].buffer_type = MYSQL_TYPE_FLOAT;
    this->bindArray[index].buffer = &value;
    return true;
}

bool PrepareResultCreator::bindString(int index, char* value, size_t len) {
    if (!validIndex(index)) {
        this->errorInfo("bindString index out!");
        return false;
    }
    this->bindArray[index].buffer_type = MYSQL_TYPE_STRING;
    this->bindArray[index].buffer = value;
    this->bindArray[index].buffer_length = len;
    return true;
}

void PrepareResultCreator::errorInfo(const char *error) {
    std::cout << error << std::endl;
}

PrepareResultCreator::~PrepareResultCreator() {
    if (this->bindArray) {
        delete[] this->bindArray;
        this->bindArray = nullptr;
    }
}

bool PrepareResultCreator::bindInt8(int index, int8_t &value) {
    if (!validIndex(index)) {
        this->errorInfo("bindInt8 index out!");
        return false;
    }
    this->bindArray[index].buffer_type = MYSQL_TYPE_TINY;
    this->bindArray[index].buffer = &value;
    this->bindArray[index].is_unsigned = false;
    return true;
}

