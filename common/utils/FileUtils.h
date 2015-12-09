#ifndef COMMON_UTILS_FILEUTILS_H_
#define COMMON_UTILS_FILEUTILS_H_

#include <string>

const int DEFAULT_FILE_MODE = 0777;

// Protocol buffer code only uses a couple static methods of File, and only
// in tests.
class FileUtils {
 public:
  // Check if the file exists.
  static bool Exists(const std::string& name);

  // Read an entire file to a std::string.  Return true if successful, false
  // otherwise.
  static bool ReadFileToString(const std::string& name, std::string* output);

  // Same as above, but crash on failure.
  static void ReadFileToStringOrDie(const std::string& name, std::string* output);

  // Create a file and write a std::string to it.
  static bool WriteStringToFile(const std::string& contents,
                                const std::string& name);

  // Same as above, but crash on failure.
  static void WriteStringToFileOrDie(const std::string& contents,
                                     const std::string& name);

  // Create a directory.
  static bool CreateDir(const std::string& name, int mode);

  // Create a directory and all parent directories if necessary.
  static bool RecursivelyCreateDir(const std::string& path, int mode);

  // If "name" is a file, we delete it.  If it is a directory, we
  // call DeleteRecursively() for each file or directory (other than
  // dot and double-dot) within it, and then delete the directory itself.
  // The "dummy" parameters have a meaning in the original version of this
  // method but they are not used anywhere in protocol buffers.
  static void DeleteRecursively(const std::string& name,
                                void* dummy1, void* dummy2);

  // Change working directory to given directory.
  static bool ChangeWorkingDirectory(const std::string& new_working_directory);

  static bool GetContents(
      const std::string& name, std::string* output, bool /*is_default*/) {
    return ReadFileToString(name, output);
  }

  static bool SetContents(
      const std::string& name, const std::string& contents, bool /*is_default*/) {
    return WriteStringToFile(contents, name);
  }

 private:
  // GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(FileUtils);
};

#endif  // COMMON_UTILS_FILEUTILS_H_
