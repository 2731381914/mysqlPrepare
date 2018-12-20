#ifndef DATASTRUCT_PREPARERESULTCREATOR_H
#define DATASTRUCT_PREPARERESULTCREATOR_H

#include <mysql.h>
#include <iostream>

class PrepareResultCreator {
public:
    MYSQL_BIND* bindArray = nullptr;
    int paramCount = 0;
public:
    explicit PrepareResultCreator(int paramCount);
    ~PrepareResultCreator();

    bool validIndex(int index);
    bool bindInt8(int index, int8_t &value);
    bool bindInt32(int index, int &value);
    bool bindUInt32(int index, unsigned int &value);
    bool bindInt64(int index, long long &value);
    bool bindUInt64(int index, unsigned long long &value);
    bool bindFloat(int index, float &value);
    bool bindString(int index, char* value, size_t len);

    void errorInfo(const char *error);
};

#endif //DATASTRUCT_PREPARERESULTCREATOR_H
