#ifndef DATASTRUCT_MYSQLPREPARE_HPP
#define DATASTRUCT_MYSQLPREPARE_HPP

#include "PrepareCreator.h"
#include "Mysql.h"

class MysqlPrepare : public PrepareCreator {
public:

    explicit MysqlPrepare(Mysql* conn) {
        this->_connection = conn->getConnection();
    }

    /**
     * select limit 1
     * @param sql
     * @param bind
     * @return int status
     */
    int findOne(std::string &sql, MYSQL_BIND *bind) {
        if (this->query()) {
            if (this->getResult(bind)) {
                int rowCount = this->resultRowCount();
                if (rowCount > 0) {
                    this->fetch_result();
                    return rowCount;
                }
            }
        }
        return -1;
    }

    template <typename DataStruct>
    int findAll(std::string &sql, MYSQL_BIND *bind, DataStruct &dataStruct, std::vector<DataStruct> &list) {
        if (this->query()) {
            if (this->getResult(bind)) {
                int rowCount = this->resultRowCount();
                if (rowCount > 0) {
                    for (int i = 0; i < rowCount; ++i) {
                        this->fetch_result();
                        list.push_back(dataStruct);
                    }
                    return rowCount;
                }
            }
        }
        return -1;
    }

};

#endif //DATASTRUCT_MYSQLPREPARE_HPP