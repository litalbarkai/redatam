// #include <numeric>    //  std::accumulate
#include <string>       //  find_last_of, substr, npos
#include <stdexcept>    //  std::invalid_argument

#include <cctype>       //  std::tolower

#include "RedatamDatabase.hpp"
#include "FuzzyEntityParser.hpp"
#include "XMLParser.hpp"
#include "CSVExporter.hpp"
#include "utils.hpp"    //  ThrowIfBad, GetFileExtension

namespace RedatamLib
{
using std::string, std::vector, std::invalid_argument;

RedatamDatabase::RedatamDatabase(const string& fileName)
{
    OpenDictionary(fileName);

    //TODO validate files (frmMain)
}

void RedatamDatabase::ExportCSVFiles(const string& outputDir)
{
    CSVExporter exporter(outputDir);

    for (Entity& e : m_entities)
    {
        exporter.CreateVariablesLegend(e);
        exporter.CreateVariablesLabels(e);
        exporter.CreateVariablesData(e);
    }
}

void RedatamDatabase::ExportSummary(const string& outputDir)
{
    //TODO
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
