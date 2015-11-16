//
//  sms.cpp
//  利用第三方短信平台www.yunpian.com发送码证码
//
//  Created by joshua on 15-11-7.

#include "common/sms/sms.h"
#include <sstream>
#include <iostream>
#include <string.h>
#include "thirdparty/simplesocket/TCPSocket.h"
#include "thirdparty/glog/logging.h"
#include "common/encode/urlencode.h"

using namespace std;

// http://www.yunpian.com/api/sms.html#c5
static const string host_name = "yunpian.com";
static const string request_url = "/v1/sms/tpl_send.json";
static const string api_key = "db671fd9df50fcf40ed4d55c196d3797";

static const int regist_tpl_id = 1070703;   // 注册短信模板id
static const int setpass_tpl_id = 1070721;  // 重置密码短信模板id

static std::string build_http_request(const std::string &content) {
  ostringstream ss;
  ss << "POST " << request_url << " HTTP/1.1\r\n"
    << "Host: " << host_name << "\r\n"
    << "Content-type: application/x-www-form-urlencoded\r\n"
    << "Content-length: " << content.length() << "\r\n\r\n"
    << content;

  return ss.str();
}

static string build_regist_content(string mobile, string app_name, string code) {
  ostringstream tpl_param;
  tpl_param << "#app#=" << app_name
    << "&#code#=" << code;

  ostringstream ss;
  ss << "apikey=" << api_key
    << "&mobile=" << mobile
    << "&tpl_id=" << regist_tpl_id
    << "&tpl_value=" << URLEncode(tpl_param.str().c_str());
  return ss.str();
}

static string build_setpass_content(string mobile, string code) {
  ostringstream tpl_param;
  tpl_param << "#code#=" << code;

  ostringstream ss;
  ss << "apikey=" << api_key
    << "&mobile=" << mobile
    << "&tpl_id=" << setpass_tpl_id 
    << "&tpl_value=" << URLEncode(tpl_param.str().c_str());
  return ss.str();
}

static int send_req(const string &req, string &res) {
  try {
    NET::TCPSocket socket;
    socket.connect(host_name, 80);
    socket.send(req.c_str(), req.length());

    char recv_msg[2048];
    memset(recv_msg, 0, sizeof(recv_msg));
    // receive response in 1s
    int ret = socket.timedReceive(recv_msg, sizeof(recv_msg), 1000);
    if (ret > 0) {
      res = recv_msg;
    } else {
      LOG(ERROR) << "receive sms server response failed in 2s!";
      return -1;
    }

    socket.disconnect();
    return 0;
  } catch( const NET::SocketException& e) {
    LOG(ERROR) << "send request to sms server encount exception: " << e.what();
    return -1;
  }
}

int RegistSMS(string mobile, string app_name, string code) {
  string reg_content = build_regist_content(mobile, app_name, code);
  string sms_req = build_http_request(reg_content);
  string sms_res;
  return send_req(sms_req, sms_res);
}

int32_t SetPassSMS(std::string mobile, string code) {
  string reg_content = build_setpass_content(mobile, code);
  string sms_req = build_http_request(reg_content);
  string sms_res;
  return send_req(sms_req, sms_res);
}

