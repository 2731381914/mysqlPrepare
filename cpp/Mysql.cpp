#include "Mysql.h"

bool Mysql::connect(const char *host, const char *username, const char *password, const char *databaseName,
                       const char *charset, unsigned int port, unsigned int connectTimeout) {

    if (_isConnected) {
        return true;
    }

    if (mysql_init(&this->_connection) == nullptr) {
        this->errorInfo("初始化数据库失败！");
        return false;
    }

    //设置连接超时
    int ret = mysql_options(&this->_connection, MYSQL_OPT_CONNECT_TIMEOUT, (const char *)&connectTimeout);
    if (ret != 0) {
        this->errorInfo("设置连接超时失败!");
        return false;
    }

    if(!mysql_real_connect(&this->_connection, host, username, password, databaseName, port, nullptr, 0)) {
        this->errorInfo("连接数据库失败!");
        return false;
    }

    if(mysql_set_character_set(&this->_connection, "utf8")) {
        this->errorInfo("设置编码失败!");
        return false;
    }

    this->_isConnected = true;
    return true;
}

MYSQL *Mysql::getConnection() {
    return &this->_connection;
}

void Mysql::errorInfo(const char *error) {
    auto errorCode = mysql_errno(&this->_connection);
    if (errorCode == 0) {
        std::cout << error << std::endl;
    } else {
        std::cout << error << ", errorCode = " << errorCode << ", errorInfo = " << mysql_error(&this->_connection) << std::endl;
    }
}

bool Mysql::isActive() {
    return mysql_ping(&this->_connection) == 0;
}

void Mysql::closeConnect() {
    if (this->_isConnected) {
        mysql_close(&this->_connection);
        _isConnected = false;
    }
}

Mysql::~Mysql() {
    this->closeConnect();
}
