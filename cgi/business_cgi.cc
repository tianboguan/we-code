#include <iostream>
#include <fstream>
#include "thirdparty/plog/Log.h"
#include "common/cgi_utils/CgiHandle.h"
#include "cgi/lib/Business.h"
#include "cgi/lib/Login.h"
#include "cgi/lib/ParseCgiReq.h"
#include "cgi/lib/BuildCgiRes.h"

using namespace std;

int main(int argc, char *argv[]) {
  plog::init(plog::debug, "/data/log/cgi/business.cgi.log");
  CgiHandle cgi_handle;
  map<string, string> params = cgi_handle.GetParams();

  ParseCgiReq parser(params);
  CgiBaseInfo base;
  int ret = parser.Parse(base);
  if (ret != kCgiCodeOk) {
    LOG_ERROR << parser.Error();
    SendPostResWithoutData(ret);
    return 0;
  }

  Account account(base.user(), base.token()); 
  ret = account.CheckLogin();
  if (ret != kCgiCodeOk) {
    LOG_ERROR << "check login status for user " << base.user() << " failed!";
    SendPostResWithoutData(ret);
    return 0;
  }

  Business business;
  string action = base.action();
  if (action == "disease" ) {
    // ElementList res;
    DiseaseRes res;
    ret = business.Disease(&res);
    if (ret != kCgiCodeOk) {
      SendPostResWithoutData(ret);
    } else {
      SendPostResWithData(ret, res);
    }
    return 0;
  } else if (action == "address") {
    AddressReq req;
    ret = parser.Parse(req);
    if (ret != kCgiCodeOk) {
      LOG_ERROR << parser.Error();
      SendPostResWithoutData(ret);
      return 0;
    }

    AddressRes res;
    ret = business.Address(req, &res);
    if (ret != kCgiCodeOk) {
      SendPostResWithoutData(ret);
    } else {
      SendAddressRes(ret, res);
    }
    return 0;
  } else if (action == "tag") {
    TagReq req;
    ret = parser.Parse(req);
    if (ret != kCgiCodeOk) {
      LOG_ERROR << parser.Error();
      SendPostResWithoutData(ret);
      return 0;
    }

    TagRes res;
    ret = business.Tag(req, &res);
    if (ret != kCgiCodeOk) {
      SendPostResWithoutData(ret);
    } else {
      SendPostResWithData(ret, res);
    }
  } else {
    LOG_ERROR << "invalid action info: " << action;
    SendPostResWithoutData(kCgiCodeParamError);
    return 0;
  }

  return 0;
}
