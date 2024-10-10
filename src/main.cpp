#include <string>
#include <iostream>    // std::cerr, std::cout, std::endl

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/stat.h>  // mkdir
#include <unistd.h>    // access
#endif

#include "RedatamDatabase.hpp"

using std::cerr;
using std::cout;
using std::endl;
using std::string;

#ifdef _WIN32
bool exists(const string& path)
{
    DWORD attr = GetFileAttributes(path.c_str());
    return (attr != INVALID_FILE_ATTRIBUTES);
}

bool create_directories(const string& path)
{
    return CreateDirectory(path.c_str(), NULL) ||
        GetLastError() == ERROR_ALREADY_EXISTS;
}
#else
bool exists(const string& path) { return access(path.c_str(), F_OK) != -1; }

bool create_directories(const string& path)
{
    mode_t mode = 0755;
    return mkdir(path.c_str(), mode) == 0 || errno == EEXIST;
}
#endif

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        cerr << "Usage: redatam input.dic(x) output-csv/" << endl;
        return 1;
    }

    string dicFilePath = argv[1];
    string outputDirPath = argv[2];

    if (!exists(dicFilePath))
    {
        cerr << "Error: Input dictionary file does not exist." << endl;
        return 1;
    }

    if (!create_directories(outputDirPath))
    {
        cerr << "Error: Failed to create output directory." << endl;
        return 1;
    }

    try
    {
        RedatamLib::RedatamDatabase db(dicFilePath);
        db.ExportCSVFiles(outputDirPath);
        db.ExportSummary(outputDirPath);
        cout << "Conversion successful!" << endl;
    }
    catch (const std::exception& e)
    {
        cout << "\033[1;31mConversion failed.\033[0m" << endl;
        return 1;
    }

    return 0;
}