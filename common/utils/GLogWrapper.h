
#ifndef COMMON_UTILS_GLOGWRAPPER_H_
#define COMMON_UTILS_GLOGWRAPPER_H_

#include <string>
#include <sys/types.h> 
#include <sys/stat.h> 
#include "thirdparty/glog/logging.h"
#include "thirdparty/gflags/gflags.h"
#include "common/utils/FileUtils.h"
#include <iostream>

inline bool InitGlog(const char *log_file,
    const char *log_dir,
    int32_t max_log_size = 20,    // 20MB
    int32_t min_log_level = google::ERROR,
    int32_t stderr_threshold = google::ERROR) {
  bool ret;
  ret = FileUtils::RecursivelyCreateDir(log_dir, S_IRWXO |  S_IRWXU | S_IRWXG);
  if (!ret) {
    return false;
  }

  google::InitGoogleLogging(log_file); 
  FLAGS_log_dir = log_dir;  
  FLAGS_minloglevel = min_log_level;  
  FLAGS_stderrthreshold = stderr_threshold;
  FLAGS_max_log_size = max_log_size;
  FLAGS_stop_logging_if_full_disk = 1;

  return true;
}

#endif // COMMON_UTILS_GLOGWRAPPER_H_
