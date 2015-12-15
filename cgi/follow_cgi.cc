#include <iostream>
#include <fstream>
#include "thirdparty/plog/Log.h"
#include "common/cgi_utils/CgiHandle.h"
#include "cgi/lib/Follow.h"
#include "cgi/lib/Login.h"
#include "cgi/lib/ParseCgiReq.h"
#include "cgi/lib/BuildCgiRes.h"

using namespace std;

int main(int argc, char *argv[]) {
  plog::init(plog::debug, "/data/log/cgi/follow.cgi.log");
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

  FollowReq follow_req;
  ret = parser.Parse(follow_req);
  if (ret != kCgiCodeOk) {
    LOG_ERROR << parser.Error();
    SendPostResWithoutData(ret);
    return 0;
  }

  Follow follow(base.user()); 
  string action = base.action();
  if (action == "add" || action == "del" ) {
    if (action== "add") {
      ret = follow.Add(follow_req);
    } else {
      ret = follow.Del(follow_req);
    }
    SendPostResWithoutData(ret);
    return 0;
  } else if (action == "follow_list" || action == "followed_list") {
    FollowListRes res;
    if (action == "follow_list") {
      ret = follow.FollowList(follow_req, &res);
    } else {
      ret = follow.FollowedList(follow_req, &res);
    }

    if (ret == kCgiCodeOk) {
      SendPostResWithData(ret, res);
    } else {
      SendPostResWithoutData(ret);
    }
    return 0;
  } else if (action == "follow_status") {
    FollowStatusRes res;
    ret = follow.FollowStatus(follow_req, &res);
    if (ret == kCgiCodeOk) {
      SendPostResWithData(ret, res);
    } else {
      SendPostResWithoutData(ret);
    }
    return 0;
  } else if (action == "block" || action == "unblock") {
    if (action == "block") {
      ret = follow.Block(follow_req);
    } else {
      ret = follow.DeBlock(follow_req);
    }
    SendPostResWithoutData(ret);
    return 0;
  } else {
    LOG_ERROR << "invalid action info: " << action;
    SendPostResWithoutData(kCgiCodeParamError);
    return 0;
  }

  return 0;
}
