#ifndef COMMON_APP_REDISKEYS_H_
#define COMMON_APP_REDISKEYS_H_

#include <string>

// ALL KEY STYLE IS [PREFIX][USER]

// account info cache key, 
std::string GetAccountSequenceNoKey();
std::string GetAccountUserKey(const std::string &user);
std::string GetAccountPhoneKey(const std::string &phone);

// ------- profile info cache key --------------
std::string GetUserProfileKey(std::string user);
std::string GetUserStatKey(std::string user);
std::string GetUserSendedInteracKey(std::string user);
std::string GetUserReceivedInteractKey(std::string user);
std::string GetUserNoticeInteractKey(std::string user);

// ---------- follow info cache key -----------
std::string GetFollowActionKey(std::string user);
std::string GetFollowToKey(std::string user);
std::string GetFollowFromKey(std::string user);
std::string GetFollowBlockKey(std::string user);

// ------ record info cache key -----------------
std::string GetRecordRoughDataKey(std::string id);
std::string GetRecordInteractDataKey(std::string id);


// -------- interact info cache key -------------
std::string GetInteractSequenceNoKey();
std::string GetRoughInteractKey(std::string id);
std::string GetInteractUserDataKey(std::string id);
std::string GetInteractRecordDataKey(std::string id);

#if 0
std::string GetRoughInteractKey(std::string id);
std::string GetInteractUserDataKey(std::string id);
std::string GetInteractRecordDataKey(std::string id);
std::string GetInteractRecordDataKey(std::string id);
std::string GetInteractNoticeDataKey(std::string id);
std::string GetInteractHistoryDataKey(std::string id);
#endif

// ------ business data cache key ------------
std::string GetDiseaseKey();
std::string GetTagKey(const std::string &tag_name);
std::string GetAddressKey(const std::string &address_code);

#endif // COMMON_APP_REDISKEYS_H_ 
