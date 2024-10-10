// #include <numeric>    //  std::accumulate
#include <string>       //  find_last_of, substr, npos
#include <stdexcept>    //  std::invalid_argument
#include <cctype>       //  std::tolower
#include <iostream>     //  std::cout

#include "RedatamDatabase.hpp"
#include "FuzzyEntityParser.hpp"
#include "XMLParser.hpp"
#include "CSVExporter.hpp"
#include "XMLExporter.hpp"
#include "utils.hpp"    //  ThrowIfBad, GetFileExtension

namespace RedatamLib
{
using std::string;
using std::vector;
using std::invalid_argument;

RedatamDatabase::RedatamDatabase(const string& fileName)
{
    std::cout << "Opening dictionary file..." << std::endl;
    OpenDictionary(fileName);
}

void RedatamDatabase::ExportCSVFiles(const string& outputDir)
{
    CSVExporter exporter(outputDir);
    exporter.ExportAll(m_entities);
}

void RedatamDatabase::ExportSummary(const string& outputDir)
{
    XMLExporter exporter(outputDir);
    exporter.ExportSummary(m_entities);
}

void RedatamDatabase::OpenDictionary(const string& fileName)
{
    string ext = GetFileExtension(fileName);

    if (".dic" == ext)
    {
        FuzzyEntityParser parser(fileName);
        m_entities = parser.ParseEntities();
    }
    else if (".dicx" == ext)
    {
        XMLParser parser;
        m_entities = parser.ParseFile(fileName);
    }
    else
    {
        ThrowIfBad<invalid_argument>(false,
            invalid_argument("Error: Dictionary file's extension must be .dic or .dicx ."));
    }
}
} // namespace RedatamLib
