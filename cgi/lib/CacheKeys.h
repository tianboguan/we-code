#ifndef CGI_LIB_CACHEKEYS_H_
#define CGI_LIB_CACHEKEYS_H_

#include <string>

// ALL KEY STYLE IS [PREFIX][USER]

// account info cache key, 
const std::string kAcountPrefix = "ac_";


// ------- follow info cache key --------------
// user's follow list
const std::string kFollowPrefix = "ft_";
// user's followed list 
const std::string kFollowedPrefix = "ff_";
// blocked user list
const std::string kBlockUserPrefix = "bu_";



// ------- profile info cache key --------------
// user profile
const std::string kProfilePrefix = "uf_";
// user stat
const std::string kStatPrefix= "us_";


// ------ business data cache key ------------
// disease
const std::string kDisease = "disease";
// address
const std::string kAddressPrefix = "ad_";
// tags
const std::string kTagWeather = "weather";
const std::string kTagMood = "mood";
const std::string kTagStatus = "status";

#endif // CGI_LIB_CACHEKEYS_H_