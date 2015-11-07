#ifndef COMMON_SMS_SMS_H_
#define COMMON_SMS_SMS_H_

#include <string>

using namespace std;

// 【百感科技】欢迎使用#app#，你的注册验证码是#code#，验证码5分钟内有效。
int32_t RegistSMS(string mobile, string app_name, string code);

// 【百感科技】本次重设密码的验证码是#code#，验证码5分钟内有效
int32_t SetPassSMS(string mobile, string code);

#endif // COMMON_SMS_SMS_H_

