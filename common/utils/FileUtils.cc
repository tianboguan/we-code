#include "common/utils/FileUtils.h"
#include "thirdparty/glog/logging.h"
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN  // yeah, right
#include <windows.h>         // Find*File().  :(
#include <io.h>
#include <direct.h>
#else
#include <dirent.h>
#include <unistd.h>
#endif
#include <errno.h>


#ifdef _WIN32
#define mkdir(name, mode) mkdir(name)
// Windows doesn't have symbolic links.
#define lstat stat
#ifndef F_OK
#define F_OK 00  // not defined by MSVC for whatever reason
#endif
#endif

bool FileUtils::Exists(const std::string& name) {
  return access(name.c_str(), F_OK) == 0;
}

bool FileUtils::ReadFileToString(const std::string& name, std::string* output) {
  char buffer[1024];
  FILE* file = fopen(name.c_str(), "rb");
  if (file == NULL) return false;

  while (true) {
    size_t n = fread(buffer, 1, sizeof(buffer), file);
    if (n <= 0) break;
    output->append(buffer, n);
  }

  int error = ferror(file);
  if (fclose(file) != 0) return false;
  return error == 0;
}

void FileUtils::ReadFileToStringOrDie(const std::string& name, std::string* output) {
  CHECK(ReadFileToString(name, output)) << "Could not read: " << name;
}

bool FileUtils::WriteStringToFile(const std::string& contents, const std::string& name) {
  FILE* file = fopen(name.c_str(), "wb");
  if (file == NULL) {
    LOG(ERROR) << "fopen(" << name << ", \"wb\"): " << strerror(errno);
    return false;
  }

  if (fwrite(contents.data(), 1, contents.size(), file) != contents.size()) {
    LOG(ERROR) << "fwrite(" << name << "): " << strerror(errno);
    return false;
  }

  if (fclose(file) != 0) {
    return false;
  }
  return true;
}

void FileUtils::WriteStringToFileOrDie(const std::string& contents, const std::string& name) {
  FILE* file = fopen(name.c_str(), "wb");
  CHECK(file != NULL)
      << "fopen(" << name << ", \"wb\"): " << strerror(errno);
  CHECK_EQ(fwrite(contents.data(), 1, contents.size(), file),
                  contents.size())
      << "fwrite(" << name << "): " << strerror(errno);
  CHECK(fclose(file) == 0)
      << "fclose(" << name << "): " << strerror(errno);
}

bool FileUtils::CreateDir(const std::string& name, int mode) {
  return mkdir(name.c_str(), mode) == 0;
}

bool FileUtils::RecursivelyCreateDir(const std::string& path, int mode) {
  if (CreateDir(path, mode)) return true;

  // if (Exists(path)) return false;
  if (Exists(path)) return true;

  // Try creating the parent.
  std::string::size_type slashpos = path.find_last_of('/');
  if (slashpos == std::string::npos) {
    // No parent given.
    return false;
  }

  return RecursivelyCreateDir(path.substr(0, slashpos), mode) &&
         CreateDir(path, mode);
}

void FileUtils::DeleteRecursively(const std::string& name,
                             void* dummy1, void* dummy2) {
  if (name.empty()) return;

  // We don't care too much about error checking here since this is only used
  // in tests to delete temporary directories that are under /tmp anyway.

#ifdef _MSC_VER
  // This interface is so weird.
  WIN32_FIND_DATA find_data;
  HANDLE find_handle = FindFirstFile((name + "/*").c_str(), &find_data);
  if (find_handle == INVALID_HANDLE_VALUE) {
    // Just delete it, whatever it is.
    DeleteFile(name.c_str());
    RemoveDirectory(name.c_str());
    return;
  }

  do {
    std::string entry_name = find_data.cFileName;
    if (entry_name != "." && entry_name != "..") {
      std::string path = name + "/" + entry_name;
      if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        DeleteRecursively(path, NULL, NULL);
        RemoveDirectory(path.c_str());
      } else {
        DeleteFile(path.c_str());
      }
    }
  } while(FindNextFile(find_handle, &find_data));
  FindClose(find_handle);

  RemoveDirectory(name.c_str());
#else
  // Use opendir()!  Yay!
  // lstat = Don't follow symbolic links.
  struct stat stats;
  if (lstat(name.c_str(), &stats) != 0) return;

  if (S_ISDIR(stats.st_mode)) {
    DIR* dir = opendir(name.c_str());
    if (dir != NULL) {
      while (true) {
        struct dirent* entry = readdir(dir);
        if (entry == NULL) break;
        std::string entry_name = entry->d_name;
        if (entry_name != "." && entry_name != "..") {
          DeleteRecursively(name + "/" + entry_name, NULL, NULL);
        }
      }
    }

    closedir(dir);
    rmdir(name.c_str());

  } else if (S_ISREG(stats.st_mode)) {
    remove(name.c_str());
  }
#endif
}

bool FileUtils::ChangeWorkingDirectory(const std::string& new_working_directory) {
  return chdir(new_working_directory.c_str()) == 0;
}

