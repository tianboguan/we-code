#include <iostream>
#include <fstream>
#include "thirdparty/plog/Log.h"
#include "common/cgi_utils/CgiHandle.h"
#include "cgi/lib/Interact.h"
#include "cgi/lib/Login.h"
#include "cgi/lib/ParseCgiReq.h"
#include "cgi/lib/BuildCgiRes.h"

using namespace std;

int main(int argc, char *argv[]) {
  plog::init(plog::debug, "/data/log/cgi/interact.cgi.log");
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

  Interact interact(base.user());
  string action = base.action();
  if (action == "like") {
    LikeReq like_req;
    ret = parser.Parse(like_req);
    if (ret != kCgiCodeOk) {
      LOG_ERROR << parser.Error();
      SendPostResWithoutData(ret);
      return 0;
    }
    ret = interact.Like(like_req);
    SendPostResWithoutData(ret);
    return 0;
  } else if (action == "unlike") {
    UnlikeReq unlike_req;
    ret = parser.Parse(unlike_req);
    if (ret != kCgiCodeOk) {
      LOG_ERROR << parser.Error();
      SendPostResWithoutData(ret);
      return 0;
    }
    ret = interact.Unlike(unlike_req);
    SendPostResWithoutData(ret);
    return 0;
  } else if (action == "comment") {
    CommentReq comment_req;
    ret = parser.Parse(comment_req);
    if (ret != kCgiCodeOk) {
      LOG_ERROR << parser.Error();
      SendPostResWithoutData(ret);
      return 0;
    }
    ret = interact.Comment(comment_req);
    SendPostResWithoutData(ret);
    return 0;
  } else if (action == "uncomment") {
    UncommentReq uncomment_req;
    ret = parser.Parse(uncomment_req);
    if (ret != kCgiCodeOk) {
      LOG_ERROR << parser.Error();
      SendPostResWithoutData(ret);
      return 0;
    }
    ret = interact.Uncomment(uncomment_req);
    SendPostResWithoutData(ret);
    return 0;
  } else if (action == "record") {
    RecordInteractReq req;
    ret = parser.Parse(req);
    if (ret != kCgiCodeOk) {
      LOG_ERROR << parser.Error();
      SendPostResWithoutData(ret);
      return 0;
    }

    RecordInteractRes res;
    ret = interact.GetRecordInteracts(req, &res);
    if (ret != kCgiCodeOk) {
      SendPostResWithoutData(ret);
    } else {
      SendPostResWithData(ret, res);
    }
    return 0;
  } else if (action == "notice" || action == "sended" || action == "received") {
    UserInteractReq req;
    ret = parser.Parse(req);
    if (ret != kCgiCodeOk) {
      LOG_ERROR << parser.Error();
      SendPostResWithoutData(ret);
      return 0;
    }

    UserInteractRes res;
    if (action == "notice") {
      ret = interact.GetNoticeInteracts(req, &res);
    } else if (action == "sended"){
      ret = interact.GetSendedInteracts(req, &res);
    } else if (action == "received"){
      ret = interact.GetSendedInteracts(req, &res);
    }
    if (ret != kCgiCodeSystemError) {
      SendPostResWithData(ret, res);
    } else {
      SendPostResWithoutData(ret);
    }
    return 0;
  } else {
    LOG_ERROR << "invalid action info: " << action;
    SendPostResWithoutData(kCgiCodeParamError);
    return 0;
  }

  return 0;
}
