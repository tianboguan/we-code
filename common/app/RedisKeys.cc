#include "common/app/RedisKeys.h"
#include <string>

// account info key
std::string GetAccountSequenceNoKey() {
  return "AccountSequenceNo";
}
std::string GetAccountUserKey(const std::string &user) {
  return "au_" + user;
}
std::string GetAccountPhoneKey(const std::string &phone) {
  return "ap_" + phone;
}

// user info key
std::string GetUserProfileKey(std::string user) {
  return "uuf_" + user;
}
std::string GetUserStatKey(std::string user) {
  return "uus_" + user;
}
// std::string GetUserInteractToKey(std::string user) {
std::string GetUserSendedInteracKey(std::string user) {
  return "usi_" + user;
}
std::string GetUserReceivedInteractKey(std::string user) {
  return "uri_" + user;
}
// std::string GetUserInteractNoticeKey(std::string user) {
std::string GetUserNoticeInteractKey(std::string user) {
  return "uni_" + user;
}
std::string GetUserHomeRecordKey(std::string user) {
  return "uhr_" + user;
}
std::string GetUserActiveRecordKey(std::string user) {
  return "uar_" + user;
}
std::string GetUserRecentRecordKey(std::string user) {
  return "urr_" + user;
}

// follow info cache key 
std::string GetFollowActionKey(std::string user) {
  return "fa_" + user;
}
std::string GetFollowToKey(std::string user) {
  return "ft_" + user;
}
std::string GetFollowFromKey(std::string user){
  return "ff_" + user;
}
std::string GetFollowBlockKey(std::string user) {
  return "fb_" + user;
}

// record info key
std::string GetRecordSequenceNoKey() {
  return "RecordSequenceNo";
}
std::string GetRecordRoughDataKey(std::string id) {
  return "rr_" + id;
}
std::string GetRecordInteractDataKey(std::string id) {
  return "ri_" + id;
}
std::string GetRecordExtDataKey(std::string id) {
  return "re_" + id;
}

// interact info key
std::string GetInteractSequenceNoKey() {
  return "InteractSequenceNo";
}
std::string GetRoughInteractKey(std::string id) {
  return "ib_" + id;
}
std::string GetInteractUserDataKey(std::string id) {
  return "iu_" + id;
}
std::string GetInteractRecordDataKey(std::string id) {
  return "ir_" + id;
}

#if 0
std::string GetInteractNoticeDataKey(std::string id) {
  return "in_" + id;
}
std::string GetInteractHistoryDataKey(std::string id) {
  return "ih_" + id;
}
#endif


// business data key 
std::string GetDiseaseKey() {
  return "disease";
}
// tag_name option: weather | mood | status
std::string GetTagKey(const std::string &tag_name) {
  return "tag_" + tag_name;
}
std::string GetAddressKey(const std::string &address_code) {
  return "address_" + address_code;
}

