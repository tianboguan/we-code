#ifndef COMMON_UTILS_PBUTILS_H_
#define COMMON_UTILS_PBUTILS_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <string>
#include "thirdparty/glog/logging.h"
#include "thirdparty/protobuf/text_format.h"
#include "thirdparty/protobuf/io/zero_copy_stream_impl.h"

template<typename T>
int32_t ParsePbConfigFromFile(std::string file_name, T *config) {
  int fd = open(file_name.c_str(), O_RDONLY);
  if (fd < 0) {
    LOG(ERROR) << "open pb config file:" << file_name << " failed!";
    return -1;
  }

  google::protobuf::io::ZeroCopyInputStream* input =
    new google::protobuf::io::FileInputStream(fd);

  if (google::protobuf::TextFormat::Parse(input, config)) {
    close(fd);
    delete input;
    return 0;
  } else {
    LOG(ERROR) << "Parse config file content failed! file: " << file_name;
    close(fd);
    delete input;
    return -1;
  }
}

template<typename T>
int32_t ReadPBFromFile(const std::string &file_name, T *pb_object) {
  int fd = open(file_name.c_str(), O_RDONLY);
  if (fd < 0) {
    LOG(ERROR) << "open pb data file:" << file_name <<
      " failed! err:" << strerror(errno);
    return -1;
  }

  if (pb_object->ParseFromFileDescriptor(fd)) {
    close(fd);
    return 0;
  } else {
    LOG(ERROR) << "read pb data to file failed! file: " << file_name;
    close(fd);
    return -1;
  }
}

template<typename T>
int32_t WritePBToFile(const std::string &file_name, const T &pb_object) {
  int fd = open(file_name.c_str(), O_CREAT | O_TRUNC | O_RDWR, S_IRWXU);
  if (fd < 0) {
    LOG(ERROR) << "open pb data file:" << file_name << " failed!";
    return -1;
  }

  if (pb_object.SerializeToFileDescriptor(fd)) {
    close(fd);
    return 0;
  } else {
    LOG(ERROR) << "write pb data to file failed! file: " << file_name;
    close(fd);
    return -1;
  }
}

#endif  // COMMON_UTILS_PBUTILS_H_

