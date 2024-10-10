#ifndef REDATAMLIB_FUZZYVARIABLEPARSER_HPP
#define REDATAMLIB_FUZZYVARIABLEPARSER_HPP

#include <vector>
#include <string>
#include <utility>          //  std::pair
#include <mutex>

#include "Variable.hpp"
#include "Entity.hpp"
#include "ByteArrayReader.hpp"

namespace RedatamLib {
using std::pair;
using std::string;
using std::vector;

class FuzzyVariableParser
{
public:
    //  throws std::ios_base::failure if fails to open file
    FuzzyVariableParser(const string& filePath);

    FuzzyVariableParser(ByteArrayReader reader, const string& rootPath);
    ~FuzzyVariableParser() = default;

    FuzzyVariableParser(const FuzzyVariableParser&) = delete;
    FuzzyVariableParser& operator=(const FuzzyVariableParser&) = delete;

    void ParseAllVariables(vector<Entity>& entities);

private:
    ByteArrayReader m_reader;
    string m_rootPath;
    std::mutex m_mtx;

    vector<pair<size_t, size_t>> GetSearchBounds(vector<Entity> entities);
    
    static VarType ParseType(ByteArrayReader* reader);
    static string ParseIdxFileName(const string& rootPath, ByteArrayReader* reader);
    static size_t ParseDataSize(VarType type, ByteArrayReader* reader);
    static vector<Tag> ParseTags(ByteArrayReader* reader);
    static void ParseMissingAndNA(vector<Tag>* tags, ByteArrayReader* reader);
    static size_t ParseDecimals(ByteArrayReader* reader);
    static size_t GetSubstringLength(string delimiter, ByteArrayReader* reader);
    static void ParseVariables(shared_ptr<vector<Variable>> output,
                                pair<size_t, size_t> bounds,
                                const string& rootPath, 
                                ByteArrayReader reader);
    static void ThreadParseVars(std::mutex& mutex,
                                size_t start, size_t end,
                                vector<Entity>& entities,
                                vector<pair<size_t, size_t>> searchBounds,
                                const string& rootPath, 
                                ByteArrayReader reader);
};

} // namespace RedatamLib

#endif  //  REDATAMLIB_FUZZYVARIABLEPARSER_HPP