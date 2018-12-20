#ifndef DATASTRUCT_MYSQL_H
#define DATASTRUCT_MYSQL_H

#include <mysql.h>
#include <iostream>

class Mysql {
private:
    bool _isConnected = false;
    MYSQL _connection{};
private:
    //获取错误信息
    void errorInfo(const char *error);
public:
    bool connect(const char* host, const char* username, const char* password, const char* databaseName, const char* charset, unsigned int port,  unsigned int connectTimeout);
    void closeConnect();
    MYSQL* getConnection();

    bool isActive();
    ~Mysql();
};

#endif //DATASTRUCT_MYSQL_H
