#ifndef CGI_LIB_CGICODE_H_
#define CGI_LIB_CGICODE_H_

#include <string>

#define kCgiCodeFollowOperatorSelf   -4
#define kCgiCodeInvalidAddressCode   -3
#define kCgiCodeParamError           -2
#define kCgiCodeSystemError          -1

#define kCgiCodeOk                   0

#define kCgiCodePhoneEnrolled        1
#define kCgiCodeUserInvalid          2
#define kCgiCodeUserAbnormal         3
#define kCgiCodePasswdError          4

#define kCgiCodeBlocked              5
#define kCgiCodeNotLogin             6
#define kCgiCodeOldPasswdError       7
#define kCgiCodeSendSmsError         8
#define kCgiCodeSmsCodeInvalid       9

#define kCgiCodeRecreateUserProfile 10

#define kCgiCodeMoreData            11
#define kCgiCodeNoMoreData          12

#define kCgiCodeSetUserProfile      13

extern std::string GetErrMsg(int code);

#endif // CGI_LIB_CGICODE_H_
