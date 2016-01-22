#include "common/app/CgiCode.h"
#include <map>
#include <string>

std::string GetErrMsg(int code) {
  std::map<int, std::string> code2msg;
  code2msg[kCgiCodeFollowOperatorSelf] = "不能操作自己";
  code2msg[kCgiCodeParamError] = "参数错误";
  code2msg[kCgiCodeSystemError] = "系统异常";
  code2msg[kCgiCodeOk] = "处理成功";
  code2msg[kCgiCodePhoneEnrolled] = "该用户已经注册, 请直接登录";
  code2msg[kCgiCodeUserInvalid] = "非法用户";
  code2msg[kCgiCodeUserAbnormal] = "用户状态异常";
  code2msg[kCgiCodePasswdError] = "用户名或密码错误";
  code2msg[kCgiCodeBlocked] = "被拉黑名单";
  code2msg[kCgiCodeNotLogin] = "用户未登录";
  code2msg[kCgiCodeOldPasswdError] = "旧密码错误";
  code2msg[kCgiCodeSendSmsError] = "验证码发送失败";
  code2msg[kCgiCodeSmsCodeInvalid] = "验证码错误";
  code2msg[kCgiCodeMoreData] = "还有更多数据";
  code2msg[kCgiCodeNoMoreData] = "没有更多数据";
  code2msg[kCgiCodeSetUserProfile] = "请先设置个人资料";
  code2msg[kCgiCodeUserProfileInexist] = "用户信息不存在";
  code2msg[kCgiCodeInvalidAddressCode] = "未知地址码";

  std::map<int, std::string>::iterator iter;
  iter = code2msg.find(code);
  if (iter != code2msg.end()) {
    return iter->second;
  } else {
    return "未知错误码";
  }
}

