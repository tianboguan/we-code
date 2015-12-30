#include <iostream>
#include <fstream>
#include "thirdparty/plog/Log.h"
#include "common/cgi_utils/CgiHandle.h"
#include "cgi/lib/Record.h"
#include "cgi/lib/Login.h"
#include "cgi/lib/ParseCgiReq.h"
#include "cgi/lib/BuildCgiRes.h"

using namespace std;

int main(int argc, char *argv[]) {
  plog::init(plog::debug, "/data/log/cgi/record.cgi.log");
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

  string action = base.action();
  if (action != "query_detail") {
    Account account(base.user(), base.token()); 
    ret = account.CheckLogin();
    if (ret != kCgiCodeOk) {
      LOG_ERROR << "check login status for user " << base.user() << " failed!";
      SendPostResWithoutData(ret);
      return 0;
    }
  }

  Record record(base.user());
  if (action == "create") {
    CreateRecordReq req;
    ret = parser.Parse(req);
    if (ret != kCgiCodeOk) {
      LOG_ERROR << parser.Error();
      SendPostResWithoutData(ret);
      return 0;
    }
    CreateRecordRes res;
    ret = record.Create(req, &res);
    if (ret != kCgiCodeOk) {
      SendPostResWithoutData(ret);
    } else {
      SendPostResWithData(ret, res);
    }
    return 0;
  } else if (action == "del") {
    DelRecordReq req;
    ret = parser.Parse(req);
    if (ret != kCgiCodeOk) {
      LOG_ERROR << parser.Error();
      SendPostResWithoutData(ret);
      return 0;
    }
    ret = record.Delete(req);
    SendPostResWithoutData(ret);
    return 0;
  } else if (action == "active" || action == "recent" || action == "home") {
    QueryRecordListReq req;
    ret = parser.Parse(req);
    if (ret != kCgiCodeOk) {
      LOG_ERROR << parser.Error();
      SendPostResWithoutData(ret);
      return 0;
    }

    QueryRecordListRes res;
    if (action == "active") {
      ret = record.GetActive(req, &res); 
    } else if (action == "recent") {
      ret = record.GetRecent(req, &res); 
    } else {
      ret = record.GetHome(req, &res); 
    }
    if (ret == kCgiCodeSystemError) {
      SendPostResWithoutData(ret);
    } else {
      SendPostResWithData(ret, res);
    }
    return 0;
  } else if (action == "alt_privacy") {
    AltRecordQrivateReq req;
    ret = parser.Parse(req);
    if (ret != kCgiCodeOk) {
      LOG_ERROR << parser.Error();
      SendPostResWithoutData(ret);
      return 0;
    }
    ret = record.AltPrivate(req);
    SendPostResWithoutData(ret);
    return 0;
  } else if (action == "query_detail") {
    QueryRecordReq req;
    ret = parser.Parse(req);
    if (ret != kCgiCodeOk) {
      LOG_ERROR << parser.Error();
      SendPostResWithoutData(ret);
      return 0;
    }
    ExtRecord res;
    ret = record.Get(req, &res);
    if (ret != kCgiCodeOk) {
      SendPostResWithoutData(ret);
    } else {
      SendPostResWithData(ret, res);
    }
    return 0;
  } else {
    LOG_ERROR << "invalid action info: " << action;
    SendPostResWithoutData(kCgiCodeParamError);
    return 0;
  }

  return 0;
}
