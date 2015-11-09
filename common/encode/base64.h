#ifndef __BASE64_H__
#define __BASE64_H__

// #include <unistd.h>
// #include <sys/select.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <time.h>
// #include <iostream>
// #include <fstream>
// #include <vector>
#include <string>

std::string base64_encode(unsigned char const* ,unsigned int len);
std::string base64_decode(std::string const& s);

#endif /* __BASE64_H__ */
