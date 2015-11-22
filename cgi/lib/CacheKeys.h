#ifndef CGI_LIB_CACHEKEYS_H_
#define CGI_LIB_CACHEKEYS_H_

#include <string>

// account info cache key, account_[user]
const std::string kAcountPrefix = "account_";

// follow info cache key
// ft_[user], user's follow list
const std::string kFollowPrefix = "ft_";
// ff_[user], user's followed list 
const std::string kFollowedPrefix = "ff_";
// bu_[user1], blocked user list
const std::string kBlockUserPrefix = "bu_";

#endif // CGI_LIB_CACHEKEYS_H_
