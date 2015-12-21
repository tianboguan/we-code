#ifndef COMMON_UTILS_PBUTILS_H_
#define COMMON_UTILS_PBUTILS_H_

#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <string>
#include <string.h>
#include <errno.h>
//#include "thirdparty/glog/logging.h"
#include "thirdparty/protobuf/text_format.h"
#include "thirdparty/protobuf/io/zero_copy_stream_impl.h"

template<typename T>
int32_t ParsePbConfigFromFile(const std::string &file_name, T *config) {
  int fd = open(file_name.c_str(), O_RDONLY);
  if (fd < 0) {
    // LOG(ERROR) << "open pb config file:" << file_name << " failed!";
    return -1;
  }

  google::protobuf::io::ZeroCopyInputStream* input =
    new google::protobuf::io::FileInputStream(fd);

  if (google::protobuf::TextFormat::Parse(input, config)) {
    delete input;
    close(fd);
    return 0;
  } else {
    // LOG(ERROR) << "Parse config file content failed! file: " << file_name;
    delete input;
    close(fd);
    return -1;
  }
}

template<typename T>
int32_t DumpPbConfigToFile(const std::string &file_name, const T &config) {
  int fd = open(file_name.c_str(), O_RDWR | O_TRUNC | O_CREAT, 0666);
  if (fd < 0) {
    // LOG(ERROR) << "open pb config file:" << file_name
    //  << " failed! err: " << strerror(errno);
    return -1;
  }

#if 1
  std::string result;
  // google::protobuf::TextFormat::PrintToString(config, &result);
  result = config.Utf8DebugString();
  if (write(fd, result.data(), result.size()) < 0) {
    // LOG(ERROR) << "write data to file failed!, file:" << file_name
    //  << " err: " << strerror(errno);
    return -1;
  } else {
    // LOG(INFO) << "write data to file success!, file:" << file_name;
    return 0;
  }
#else
  google::protobuf::io::ZeroCopyOutputStream* output =
    new google::protobuf::io::FileOutputStream(fd);
  if (google::protobuf::TextFormat::Print(config, output)) {
    delete output;
    close(fd);
    return 0;
  } else {
    // LOG(ERROR) << "dump config  to file failed! file: " << file_name;
    delete output;
    close(fd);
    return -1;
  }
#endif
}

template<typename T>
int32_t ReadPBFromFile(const std::string &file_name, T *pb_object) {
  int fd = open(file_name.c_str(), O_RDONLY);
  if (fd < 0) {
    // LOG(ERROR) << "open pb data file:" << file_name <<
    //  " failed! err:" << strerror(errno);
    return -1;
  }

  if (pb_object->ParseFromFileDescriptor(fd)) {
    close(fd);
    return 0;
  } else {
    // LOG(ERROR) << "read pb data to file failed! file: " << file_name;
    close(fd);
    return -1;
  }
}

template<typename T>
int32_t WritePBToFile(const std::string &file_name, const T &pb_object) {
  int fd = open(file_name.c_str(), O_CREAT | O_TRUNC | O_RDWR, S_IRWXU);
  if (fd < 0) {
    // LOG(ERROR) << "open pb data file:" << file_name << " failed!";
    return -1;
  }

  if (pb_object.SerializeToFileDescriptor(fd)) {
    close(fd);
    return 0;
  } else {
    // LOG(ERROR) << "write pb data to file failed! file: " << file_name;
    close(fd);
    return -1;
  }
}

#endif  // COMMON_UTILS_PBUTILS_H_

