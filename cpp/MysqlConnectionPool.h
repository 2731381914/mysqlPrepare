#ifndef DATASTRUCT_MYSQLCONNECTIONPOOL_H
#define DATASTRUCT_MYSQLCONNECTIONPOOL_H

#include <queue>
#include "Mysql.h"

class MysqlConnectionPool {
private:
    const char* _host;
    unsigned int _port;
    const char* _username;
    const char* _password;
    const char* _databaseName;
    const char* _charset;
    unsigned int _connectTimeout;
    std::queue<Mysql*> _queue;
    std::atomic_int _connectedCount{};
    std::mutex _mutex;
public:
    MysqlConnectionPool(const char* host, const char* username, const char* password, const char* databaseName, const char* charset, unsigned int port,  unsigned int connectTimeout);
    int initPool(unsigned short size);
    Mysql* createConnection();
    Mysql* getConnection();
    void returnConnection(Mysql* connection);
    void destroy();
    void errorInfo(const char* info);
    ~MysqlConnectionPool();
};

#endif //DATASTRUCT_MYSQLCONNECTIONPOOL_H