#ifndef DATASTRUCT_USERTABLEFIELD_HPP
#define DATASTRUCT_USERTABLEFIELD_HPP

#include <iostream>

template <size_t ByteLen>
struct Str {
    char data[ByteLen] = {};
    size_t len = ByteLen;
};


class UserTableField {
public:
    uint32_t uInt32_id = 0;
    uint32_t uInt32_reg_time = 0;
    uint32_t uInt32_last_login_time = 0;
    int8_t   int8_status = 0;
    Str<45>  str_mobile = {};
    Str<180> str_password = {};
    Str<180> str_access_token = {};

//    Str50 str_mobile = {};
//    Str200 str_password = {};
//    Str200 str_access_token = {};
};

//mysql utf8 占3个字节, char 15 是15个字符 * 3 = 45 个字节
#endif //DATASTRUCT_USERTABLEFIELD_HPP
