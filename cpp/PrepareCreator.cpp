#include "PrepareCreator.h"

bool PrepareCreator::prepare(const char* sql) {

//     if (this->_stmt != nullptr) {
//         mysql_stmt_close(this->_stmt);
//         this->_stmt = nullptr;
//     }

    this->_stmt = mysql_stmt_init(this->_connection);
    if (!this->_stmt) {
        this->stmtErrorInfo("mysql_stmt_init");
        return false;
    }

    if (mysql_stmt_prepare(this->_stmt, sql, strlen(sql))) {
        this->stmtErrorInfo("mysql_stmt_prepare");
        return false;
    }

    this->_bindParamCount = static_cast<unsigned short>(mysql_stmt_param_count(this->_stmt));
    if (this->_bindParamCount > 0) {
        if (this->_bindParamArray != nullptr) {
            delete []this->_bindParamArray;
            this->_bindParamArray = nullptr;
        }
        this->_bindParamArray = new MYSQL_BIND[this->_bindParamCount];
        if (!_bindParamArray) {
            this->stmtErrorInfo("new MYSQL_BIND");
            return false;
        }
        memset(this->_bindParamArray, 0, sizeof(MYSQL_BIND)*this->_bindParamCount);
    }
    return true;
}

bool PrepareCreator::validIndex(int index) {
    return index < this->_bindParamCount;
}

bool PrepareCreator::bindInt8(int index, int8_t &value) {
    if (index >= this->_bindParamCount) {
        this->errorInfo("bindInt8 index out!");
        return false;
    }
    this->_bindParamArray[index].buffer_type = MYSQL_TYPE_TINY;
    this->_bindParamArray[index].buffer = &value;
    this->_bindParamArray[index].is_unsigned = false;
    return true;
}

bool PrepareCreator::bindInt32(int index, int &value) {
    if (index >= this->_bindParamCount) {
        this->errorInfo("bindInt32 index out!");
        return false;
    }
    this->_bindParamArray[index].buffer_type = MYSQL_TYPE_LONG;
    this->_bindParamArray[index].buffer = &value;
    this->_bindParamArray[index].is_unsigned = false;
    return true;
}

bool PrepareCreator::bindUInt32(int index, unsigned int &value) {
    if (!validIndex(index)) {
        this->errorInfo("bindUInt32 index out!");
        return false;
    }
    this->_bindParamArray[index].buffer_type = MYSQL_TYPE_LONG;
    this->_bindParamArray[index].buffer = &value;
    this->_bindParamArray[index].is_unsigned = true;
    return true;
}

bool PrepareCreator::bindInt64(int index, long long &value) {
    if (!validIndex(index)) {
        this->errorInfo("bindInt64 index out!");
        return false;
    }
    this->_bindParamArray[index].buffer_type = MYSQL_TYPE_LONGLONG;
    this->_bindParamArray[index].buffer = &value;
    this->_bindParamArray[index].is_unsigned = false;
    return true;
}

bool PrepareCreator::bindUInt64(int index, unsigned long long &value) {
    if (!validIndex(index)) {
        this->errorInfo("bindUInt64 index out!");
        return false;
    }
    this->_bindParamArray[index].buffer_type = MYSQL_TYPE_LONGLONG;
    this->_bindParamArray[index].buffer = &value;
    this->_bindParamArray[index].is_unsigned = true;
    return true;
}

bool PrepareCreator::bindFloat(int index, float &value) {
    if (!validIndex(index)) {
        this->errorInfo("bindFloat index out!");
        return false;
    }
    this->_bindParamArray[index].buffer_type = MYSQL_TYPE_FLOAT;
    this->_bindParamArray[index].buffer = &value;
    return true;
}

bool PrepareCreator::bindString(int index, std::string &value) {
    if (!validIndex(index)) {
        this->errorInfo("bindString index out!");
        return false;
    }
    this->_bindParamArray[index].buffer_type = MYSQL_TYPE_STRING;
    this->_bindParamArray[index].buffer = (void*)value.c_str();
    this->_bindParamArray[index].buffer_length = value.size();
    return true;
}

bool PrepareCreator::bindString(int index, char *value, size_t len) {
    if (!validIndex(index)) {
        this->errorInfo("bindString index out!");
        return false;
    }
    this->_bindParamArray[index].buffer_type = MYSQL_TYPE_STRING;
    this->_bindParamArray[index].buffer = value;
    this->_bindParamArray[index].buffer_length = len;
    return true;
}

bool PrepareCreator::execute() {

    if (mysql_stmt_bind_param(this->_stmt, this->_bindParamArray)) {
        this->stmtErrorInfo("mysql_stmt_bind_param");
        return false;
    }

    if (mysql_stmt_execute(this->_stmt)) {
        this->stmtErrorInfo("mysql_stmt_execute");
        return false;
    }
    return true;
}

bool PrepareCreator::query() {

    if (mysql_stmt_bind_param(this->_stmt, this->_bindParamArray)) {
        this->stmtErrorInfo("mysql_stmt_bind_param");
        return false;
    }

    if (nullptr != this->_prepare_meta_result) {
        mysql_free_result(this->_prepare_meta_result);
        _prepare_meta_result = nullptr;
    }

    this->_prepare_meta_result = mysql_stmt_result_metadata(this->_stmt);
    if (nullptr == this->_prepare_meta_result) {
        this->stmtErrorInfo("mysql_stmt_result_metadata");
        return false;
    }

    if (mysql_stmt_execute(this->_stmt)) {
        this->stmtErrorInfo("mysql_stmt_execute");
        return false;
    }
    return true;

}

bool PrepareCreator::getResult(MYSQL_BIND *bind) {

    if (mysql_stmt_bind_result(this->_stmt, bind)) {
        this->stmtErrorInfo("mysql_stmt_bind_result");
        return false;
    }

    if (mysql_stmt_store_result(this->_stmt)) {
        this->stmtErrorInfo("mysql_stmt_store_result");
        return false;
    }

    return true;
}

int PrepareCreator::fetch_result() {
    int res = mysql_stmt_fetch(this->_stmt);
    if (res == 0) {
        return 1;
    } else {
        this->stmtErrorInfo("PrepareCreator::fetch_result");
        return 0;
    }
}

//获取查询结果集数据行数
int PrepareCreator::resultRowCount() {
    if (this->_stmt) {
        return static_cast<int>(mysql_stmt_num_rows(this->_stmt));
    }
    return 0;
}

unsigned int PrepareCreator::affected_rows() {
    return static_cast<unsigned int>(mysql_stmt_affected_rows(this->_stmt));
}

unsigned int PrepareCreator::insert_id() {
    return static_cast<int>(mysql_stmt_insert_id(this->_stmt));
}

int PrepareCreator::fieldCount() {
    return static_cast<int>(mysql_stmt_param_count(this->_stmt));
}

PrepareCreator::~PrepareCreator() {
    this->free();
}

void PrepareCreator::free() {

    if (this->_stmt) {
        mysql_stmt_close(this->_stmt);
        this->_stmt = nullptr;
    }

    if (this->_bindParamArray) {
        delete []this->_bindParamArray;
        this->_bindParamArray = nullptr;
    }

    if (nullptr != this->_prepare_meta_result) {
        mysql_free_result(this->_prepare_meta_result);
        _prepare_meta_result = nullptr;
    }

}

void PrepareCreator::errorInfo(const char *error) {
    //mysql_errno 它通常都是非0值。没有错误，它将返回0
    auto errorCode = mysql_errno(this->_connection);
    if (errorCode == 0) {
        std::cout << error << std::endl;
    } else {
        std::cout << error << ", errorCode = " << errorCode << ", errorInfo = " << mysql_error(this->_connection) << std::endl;
    }
}

void PrepareCreator::stmtErrorInfo(const char *error) {
    //mysql_errno 它通常都是非0值。没有错误，它将返回0
    auto errorCode = mysql_stmt_errno(this->_stmt);
    if (errorCode == 0) {
        std::cout << error << std::endl;
    } else {
        std::cout << error << ", stmtErrorCode = " << errorCode << ", stmtErrorInfo = " << mysql_stmt_error(this->_stmt) << std::endl;
    }
}
