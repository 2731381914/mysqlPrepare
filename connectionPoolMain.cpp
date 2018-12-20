#include <iostream>
#include <thread>
#include <vector>

#include "cpp/PrepareResultCreator.h"
#include "cpp/UserTableField.hpp"
#include "cpp/MysqlPrepare.hpp"
#include "cpp/MysqlConnectionPool.h"

std::string getSplitChar(size_t fieldCount) {
    std::string tempSplitString;
    for (int i = 0; i < fieldCount; ++i) {
        tempSplitString += (i == 0) ? "?" : ",?";
    }
    return tempSplitString;
}

void cppQuery(MysqlPrepare* mysqlPrepare) {

    std::string mobile   = "13212505550";
    std::string password = "$2y$13$WpL0gixfR7NLrUteq4uV6u0XmbL9v1IgPn8kTsMjOQ0Ko1VeKsyHO";

    std::string sql = "select mobile, password from qh_user where mobile=? and password=? limit 1";
    if (mysqlPrepare->prepare(sql.c_str())) {

        mysqlPrepare->bindString(0, mobile);
        mysqlPrepare->bindString(1, password);

        UserTableField tempTable;

        PrepareResultCreator prepareResultCreator(2);
        prepareResultCreator.bindString(0, tempTable.str_mobile.data, tempTable.str_mobile.len);
        prepareResultCreator.bindString(1, tempTable.str_password.data, tempTable.str_password.len);

        if (mysqlPrepare->findOne(sql, prepareResultCreator.bindArray)) {
            std::cout << "mobile=" << tempTable.str_mobile.data << std::endl;
            std::cout << "password=" << tempTable.str_password.data << std::endl;
        }

    }
}

void cppQueryAll(MysqlPrepare* mysqlPrepare, std::vector<unsigned int> &friendIdList) {
    std::string sql = "select mobile, password from qh_user where id in("+getSplitChar(friendIdList.size())+")";
    if (mysqlPrepare->prepare(sql.c_str())) {

        for (int i = 0; i < friendIdList.size(); i++) {
            mysqlPrepare->bindUInt32(i, friendIdList[i]);
        }

        UserTableField tempTable;

        PrepareResultCreator prepareResultCreator(2);
        prepareResultCreator.bindString(0, tempTable.str_mobile.data, tempTable.str_mobile.len);
        prepareResultCreator.bindString(1, tempTable.str_password.data, tempTable.str_password.len);

        std::vector<UserTableField> FriendInfoList;
        if (mysqlPrepare->findAll(sql, prepareResultCreator.bindArray, tempTable, FriendInfoList)) {
            for (const auto &userInfo : FriendInfoList) {
                std::cout << userInfo.str_mobile.data << std::endl;
                std::cout << userInfo.str_password.data << std::endl;
            }
        }

    }
}

void cppInsert(MysqlPrepare* mysqlPrepare) {
    std::string field = "id, mobile, password, reg_time, last_login_time, status, access_token";
    std::string sql   = "insert into qh_user("+field+") value("+getSplitChar(7)+")";

    if (mysqlPrepare->prepare(sql.c_str())) {

        uint32_t id = MYSQL_TYPE_NULL;
        std::string mobile   = "13212505551";
        std::string password = "asdfadfadfadfasdf";
        uint32_t reg_time = 1234567891;
        uint32_t last_login_time = 1234567891;
        int8_t status = 1;
        std::string access_token = "adfasdfasdfasdf";


        mysqlPrepare->bindUInt32(0, id);
        mysqlPrepare->bindString(1, mobile);
        mysqlPrepare->bindString(2, password);
        mysqlPrepare->bindUInt32(3, reg_time);
        mysqlPrepare->bindUInt32(4, last_login_time);
        mysqlPrepare->bindInt8(5, status);
        mysqlPrepare->bindString(6, access_token);

        if (mysqlPrepare->execute()) {
            std::cout << mysqlPrepare->affected_rows() << std::endl;
        } else {
            std::cout << "insert failed!" << std::endl;
        }

    }
}

int main(int argc, const char* args[]) {

    std::string mobile;

    //show variables like '%max_connections%'; 最大连接数
    //[mysqld]
    //max_connections = 1000

    //show status like 'Threads%'; theads_connected 表示客户端建立的连接数
    MysqlConnectionPool mysqlConnectionPool("127.0.0.1", "root", "", "app", "utf8", 3306, 5);
    int connectedCount = mysqlConnectionPool.initPool(20);


//    while(true) {
        Mysql* conn = mysqlConnectionPool.getConnection();
        MysqlPrepare mysqlPrepare(conn);

        std::vector<unsigned int> friendIdList = {21,38,42};
        cppQueryAll(&mysqlPrepare, friendIdList);

        cppQuery(&mysqlPrepare);
        cppInsert(&mysqlPrepare);

        mysqlConnectionPool.returnConnection(conn);
//
//        std::chrono::milliseconds t(10);
//        std::this_thread::sleep_for(t);
//    }

    mysqlConnectionPool.destroy();
    return 0;
}