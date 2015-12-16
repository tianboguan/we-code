#ifndef __STRING_UTILS_H__
#define __STRING_UTILS_H__

#include <algorithm>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

//convert a arbitrary type value to string
template<typename T>
std::string value_to_string(const T &v) {
  std::stringstream ss; 
  ss << v;
  return ss.str();
}

//convert string to a arbitrary type
template<typename T>
void string_to_value(const std::string &strInput, T &v) {
  std::istringstream ss(strInput);
  ss >> v;
  return ;
}

//convert a arbitrary type vector to a string split by strDelim
template<typename T>
std::string vector_to_string(const std::vector<T> &vecInput, std::string strDelim = ",") {
  std::string s;
  typename std::vector<T>::const_iterator iter;
  for (iter = vecInput.begin(); iter != vecInput.end(); iter++) {
    s += value_to_string(*iter);
    s += strDelim;
  }
  if (!s.empty())
    s = s.substr(0, s.length() - strDelim.size());
  return s;
}

//convert a string delimit with delimiter to a vector with type T
template<typename T>
void string_to_vector(std::string strInput, char delimiter, std::vector<T> &vecOutput) {
  std::replace(strInput.begin(), strInput.end(), delimiter, '\t');
  std::istringstream iss(strInput);
  T tv;
  while (iss >> tv) {
    vecOutput.push_back(tv);
  }
  return;
}

#endif  /*__STRING_UTILS_H__*/
