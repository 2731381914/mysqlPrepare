#ifndef DATASTRUCT_PREPARECREATOR_H
#define DATASTRUCT_PREPARECREATOR_H

#include <mysql.h>
#include <iostream>
#include "Mysql.h"

class PrepareCreator {
protected:
    MYSQL* _connection{};
private:
    MYSQL_STMT* _stmt{};
    MYSQL_BIND*    _bindParamArray = nullptr;//参数绑定
    unsigned short _bindParamCount = 0;//参数总数

    MYSQL_RES *_prepare_meta_result = nullptr;
private:
    //获取错误信息
    void errorInfo(const char *error);
    void stmtErrorInfo(const char *error);
public:

    bool prepare(const char* sql);
    bool execute();
    bool query();
    bool getResult(MYSQL_BIND* bind);
    int  fetch_result();

    bool validIndex(int index);
    bool bindInt8(int index, int8_t &value);
    bool bindInt32(int index, int &value);
    bool bindUInt32(int index, unsigned int &value);
    bool bindInt64(int index, long long &value);
    bool bindUInt64(int index, unsigned long long &value);
    bool bindFloat(int index, float &value);
    bool bindString(int index, std::string& value);
    bool bindString(int index, char* value, size_t len);

    int resultRowCount();
    unsigned int affected_rows();
    unsigned int insert_id();

    void free();
    virtual ~PrepareCreator();

protected:
    int fieldCount();
};


#endif //DATASTRUCT_PREPARECREATOR_H
