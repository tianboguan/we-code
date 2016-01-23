#ifndef COMMON_APP_REDISKEYS_H_
#define COMMON_APP_REDISKEYS_H_

#include <string>

// ALL KEY STYLE IS [PREFIX][USER]

// account info cache key, 
std::string GetAccountSequenceNoKey();
std::string GetAccountUserKey(const std::string &user);
std::string GetAccountPhoneKey(const std::string &phone);

// ------- user info cache key --------------
std::string GetUserProfileKey(std::string user);
std::string GetUserFromProfileKey(std::string key);
std::string GetUserStatKey(std::string user);
std::string GetUserSendedInteracKey(std::string user);
std::string GetUserReceivedInteractKey(std::string user);
std::string GetUserNoticeInteractKey(std::string user);

std::string GetUserHomeRecordKey(std::string user);
std::string GetUserActiveRecordKey(std::string user);
// std::string GetUserRecentRecordKey(std::string user);

// ---------- follow info cache key -----------
std::string GetFollowActionKey(std::string user);
std::string GetFollowToKey(std::string user);
std::string GetFollowFromKey(std::string user);
std::string GetFollowBlockKey(std::string user);

// ------ record info cache key -----------------
std::string GetRecordSequenceNoKey();
std::string GetRecordRoughDataKey(std::string id);
std::string GetRecordInteractDataKey(std::string id);
std::string GetRecordRecentKey();

// -------- interact info cache key -------------
std::string GetInteractSequenceNoKey();
std::string GetRoughInteractKey(std::string id);
std::string GetInteractUserDataKey(std::string id);
std::string GetInteractRecordDataKey(std::string id);

// ------ business data cache key ------------
std::string GetDiseaseKey();
std::string GetTagKey(const std::string &tag_name);
std::string GetAddressKey(const std::string &address_code);

// ------ recommand data cache key ------------
std::string GetRecommandKey(const std::string &user);

// -------------- statistic data cache key --------
std::string GetRecordStatisticKey(const std::string &id);
std::string GetUserStatisticKey(const std::string &user);

#endif // COMMON_APP_REDISKEYS_H_ 
