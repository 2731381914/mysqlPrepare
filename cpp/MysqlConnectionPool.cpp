#include "MysqlConnectionPool.h"

MysqlConnectionPool::MysqlConnectionPool(const char *host, const char *username, const char *password,
                                         const char *databaseName, const char *charset, unsigned int port,
                                         unsigned int connectTimeout) {
    this->_host = host;
    this->_username = username;
    this->_password = password;
    this->_databaseName = databaseName;
    this->_charset = charset;
    this->_port = port;
    this->_connectTimeout = connectTimeout;
}

int MysqlConnectionPool::initPool(unsigned short size) {
    for (int i = 0; i < size; ++i) {
        Mysql* mysql = new Mysql();
        if (mysql->connect(this->_host, this->_username, this->_password, this->_databaseName, this->_charset, this->_port, this->_connectTimeout)) {
            this->_queue.push(mysql);
            this->_connectedCount++;
        } else {
            this->errorInfo("MysqlConnectionPool:initPool--连接数据库失败!");
        }
    }
    return this->_connectedCount;
}

Mysql* MysqlConnectionPool::getConnection() {
    Mysql* conn = nullptr;
    std::cout << this->_queue.size() << std::endl;
    std::lock_guard<std::mutex> autoUnlock(this->_mutex);
    if (!this->_queue.empty()) {
        conn = this->_queue.front();
        this->_queue.pop();
    }
    if (conn) {
        if (conn->isActive()) {
            return conn;
        } else {
            conn->closeConnect();//free fd
            delete conn;//free memory
            conn = this->createConnection();
            return conn;
        }
    }
    return conn;
}

void MysqlConnectionPool::returnConnection(Mysql* connection) {
    std::lock_guard<std::mutex> autoUnlock(this->_mutex);
    if (connection) {
        this->_queue.push(connection);
    }
}

Mysql* MysqlConnectionPool::createConnection() {
    Mysql* conn = new Mysql();
    if (conn->connect(this->_host,
                      this->_username,
                      this->_password,
                      this->_databaseName,
                      this->_charset,
                      this->_port,
                      this->_connectTimeout)) {
        return conn;
    }
    return nullptr;
}


void MysqlConnectionPool::destroy() {
    while (this->_connectedCount) {
        Mysql* conn = this->_queue.front();
        this->_queue.pop();
        --this->_connectedCount;
        if (conn) {
            conn->closeConnect();
            delete conn;
            conn = nullptr;
        }
    }
}

MysqlConnectionPool::~MysqlConnectionPool() {
    this->destroy();
}

void MysqlConnectionPool::errorInfo(const char *info) {
    std::cout << info << std::endl;
}