#ifndef REDATAMLIB_UTILS_HPP
#define REDATAMLIB_UTILS_HPP

#include <string>
#include <iostream>     //  std::cerr, std::endl
#include <stdexcept>    //  std::invalid_argument
#include <system_error> //  std::error_code, std::generic_category

#include <cstring>      //  std::strerror

namespace RedatamLib
{
using std::string;
using std::cerr;
using std::endl;

string FindRootPath(const string& fileName);
string ReplaceRootPath(const string& rootPath, const string& fileName);

//  Throws std::invalid_argument
string GetFileExtension(const string& fileName);

bool TryGetFileExtension(const string& fileName, string* output);
bool exists(const std::string& path);
bool create_directories(const std::string& path);

template <typename E>
void ThrowIfBad(bool is_good_, std::error_code err_, const string& e_msg_)
{
    if (!is_good_)
    {
        // cerr << e_msg_ << endl;
        throw E(err_, e_msg_);
    }
}

template <typename E>
void ThrowIfBad(bool is_good_, int errno_)
{
    if (!is_good_)
    {
        // cerr << std::strerror(errno_) << endl;
        throw E(std::error_code(errno_, std::generic_category()),
                                            string(std::strerror(errno_)));
    }
}

template <typename E>
void ThrowIfBad(bool is_good_, const E& e_)
{
    if (!is_good_)
    {
        // cerr << e_.what() << endl;
        throw e_;
    }
}

} // namespace RedatamLib

#endif  //  REDATAMLIB_UTILS_HPP