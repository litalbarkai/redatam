#include "utils.hpp"

#include <algorithm>  //  std::transform, std::min

namespace RedatamLib {
using std::invalid_argument;

string FindRootPath(const string& fileName) {
#ifdef _WIN32
  // Windows
  size_t pos = fileName.find_last_of('\\');
#else
  // Unix
  size_t pos = fileName.find_last_of('/');
#endif

  // Return the root path up to and including the last path separator
  return (pos == string::npos) ? "" : fileName.substr(0, pos + 1);
}

string GetFileName(const string& fullPath) {
  // Find the position of the last path separator (either '/' or '\')
  size_t posSlash = fullPath.find_last_of('/');
  size_t posBackslash = fullPath.find_last_of('\\');

  // Use parentheses around std::min to deambiguate min on Windows
  size_t pos = (posSlash == string::npos) ? posBackslash
               : (posBackslash == string::npos)
                   ? posSlash
                   : (std::min)(posSlash, posBackslash);

  return (pos == string::npos) ? fullPath : fullPath.substr(pos + 1);
}

string ReplaceRootPath(const string& rootPath, const string& fileName) {
  // Ensure rootPath ends with a path separator
  string normalizedRootPath = rootPath;
  if (normalizedRootPath.back() != '/' && normalizedRootPath.back() != '\\') {
    normalizedRootPath.append("/");
  }

  // Remove any leading path separators or relative path components from
  // fileName

  string cleanedFileName = GetFileName(fileName);

  while (!cleanedFileName.empty() &&
         (cleanedFileName.front() == '/' || cleanedFileName.front() == '\\' ||
          cleanedFileName.substr(0, 2) == ".\\" ||
          cleanedFileName.substr(0, 2) == "./")) {
    if (cleanedFileName.front() == '/' || cleanedFileName.front() == '\\') {
      cleanedFileName = cleanedFileName.substr(1);
    } else if (cleanedFileName.substr(0, 2) == ".\\" ||
               cleanedFileName.substr(0, 2) == "./") {
      cleanedFileName = cleanedFileName.substr(2);
    }
  }

  // Remove leading '.' if it is followed by a character
  if (!cleanedFileName.empty() && cleanedFileName.front() == '.' &&
      cleanedFileName.size() > 1 && cleanedFileName[1] != '/') {
    cleanedFileName = cleanedFileName.substr(1);
  }

  // Construct the new path
  string ret = normalizedRootPath;
  ret.append(cleanedFileName);

  // Normalize the path separators to the platform-specific separator
#ifdef _WIN32
  std::replace(ret.begin(), ret.end(), '/', '\\');  // Windows
#else
  std::replace(ret.begin(), ret.end(), '\\', '/');  // Unix
#endif

  return ret;
}

bool TryGetFileExtension(const string& fileName, string* output) {
  try {
    string ext = GetFileExtension(fileName);
    *output = ext;
  } catch (const invalid_argument&) {
    return false;
  }

  return true;
}

string GetFileExtension(const string& fileName) {
  auto extPos = fileName.find_last_of('.');
  ThrowIfBad<invalid_argument>(
      extPos != string::npos,
      invalid_argument("Error: No file extension found."));

  string ext = fileName.substr(extPos);
  std::transform(ext.begin(), ext.end(), ext.begin(),
                 [](unsigned char c) { return std::tolower(c); });

  return ext;
}

}  // namespace RedatamLib
