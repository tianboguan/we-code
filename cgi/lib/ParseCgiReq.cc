#include "cgi/lib/ParseCgiReq.h"
#include "common/app/CgiCode.h"
#include "common/utils/StringUtils.h"

#define CHECK(key, need)      \
  ret_ = Check(key, need);      \
if (!ret_) {                    \
  return kCgiCodeParamError;   \
}

int ParseCgiReq::Parse(CgiBaseInfo &base) {
  CHECK("user", true);
  CHECK("token", true);
  CHECK("action", true);
  base.set_user(params_["user"]);
  base.set_token(params_["token"]);
  base.set_action(params_["action"]);
  return kCgiCodeOk;
}

// account related
int ParseCgiReq::Parse(EnrollReq &enroll) {
  CHECK("phone", true);
  CHECK("password", true);
  enroll.set_phone(params_["phone"]);
  enroll.set_password(params_["password"]);
  return kCgiCodeOk;
}

int ParseCgiReq::Parse(LoginReq &login) {
  CHECK("phone", true);
  CHECK("password", true);
  login.set_phone(params_["phone"]);
  login.set_password(params_["password"]);
  return kCgiCodeOk;
}

int ParseCgiReq::Parse(ModifyPassReq &modify_pass) {
  CHECK("o_pass", true);
  CHECK("n_pass", true);
  modify_pass.set_o_pass(params_["o_pass"]);
  modify_pass.set_n_pass(params_["n_pass"]);
  return kCgiCodeOk;
}

int ParseCgiReq::Parse(ResetPassReq &reset_pass) {
  CHECK("n_pass", true);
  CHECK("phone", true);
  reset_pass.set_n_pass(params_["n_pass"]);
  reset_pass.set_phone(params_["phone"]);
  return kCgiCodeOk;
}

int ParseCgiReq::Parse(VerifyCodeReq &verify_code) {
  CHECK("code", true);
  verify_code.set_code(params_["code"]);
  return kCgiCodeOk;
}

int ParseCgiReq::Parse(FollowReq &follow) {
  CHECK("target_user", true);
  follow.set_target_user(params_["target_user"]);
  return kCgiCodeOk;
}

int ParseCgiReq::Parse(QueryProfileReq &query_profile) {
  CHECK("target_user", true);
  query_profile.set_target_user(params_["target_user"]);
  return kCgiCodeOk;
}

int ParseCgiReq::Parse(AltProfileReq &alt_profile) {
  CHECK("profile", true);
  alt_profile.set_profile(params_["profile"]);
  return kCgiCodeOk;
}

int ParseCgiReq::Parse(AddressReq &address) {
  CHECK("address_code", true);
  address.set_address_code(params_["address_code"]);
  return kCgiCodeOk;
}

int ParseCgiReq::Parse(TagReq &tag) {
  CHECK("tag_class", true);
  tag.set_tag_class(params_["tag_class"]);
  return kCgiCodeOk;
}

int ParseCgiReq::Parse(LikeReq &like) {
  CHECK("record_id", true);
  like.set_record_id(params_["record_id"]);
  return kCgiCodeOk;
}
int ParseCgiReq::Parse(UnlikeReq &unlike) {
  CHECK("record_id", true);
  unlike.set_record_id(params_["record_id"]);
  return kCgiCodeOk;
}
int ParseCgiReq::Parse(CommentReq &comment) {
  CHECK("record_id", true);
  CHECK("target_interact_id", false);
  CHECK("text", true);
  comment.set_record_id(params_["record_id"]);
  comment.set_target_interact_id(params_["target_interact_id"]);
  comment.set_text(params_["text"]);
  return kCgiCodeOk;
}
int ParseCgiReq::Parse(UncommentReq &uncomment) {
  CHECK("record_id", true);
  CHECK("interact_id", true);
  uncomment.set_record_id(params_["record_id"]);
  uncomment.set_interact_id(params_["interact_id"]);
  return kCgiCodeOk;
}
int ParseCgiReq::Parse(RecordInteractReq &record_interact) {
  CHECK("record_id", true);
  record_interact.set_record_id(params_["record_id"]);
  return kCgiCodeOk;
}
int ParseCgiReq::Parse(UserInteractReq &user_interact) {
  CHECK("page", true);
  int32_t page = 1;
  string_to_value(params_["page"], page);
  user_interact.set_page(page);
  return kCgiCodeOk;
}

std::string ParseCgiReq::Error() {
  return err_oss.str();
}

bool ParseCgiReq::Check(std::string key, bool need) {
  if (params_.find(key) == params_.end() && need) {
    if (need) {
      err_oss << "not find: " << key << std::endl;
      return false;
    } else {
      return true;
    }
  }

  return true;
}

