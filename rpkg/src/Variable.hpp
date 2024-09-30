#ifndef REDATAMLIB_VARIABLE_HPP
#define REDATAMLIB_VARIABLE_HPP

#include <string>
#include <vector>
#include <utility>  //  std::pair
#include <memory>   //  std::shared_ptr

#include "ByteArrayReader.hpp"

namespace RedatamLib {
using std::vector, std::string, std::pair, std::shared_ptr;

enum VarType {BIN, CHR, DBL, INT, LNG, PCK, NA};
using Tag = pair<string, string>;   //  Tag = <Key, Value>

class Variable {
public:
    explicit Variable();
    explicit Variable(const string& name,
                        VarType type,
                        const string& idxFileName,
                        size_t dataSize,
                        const string& filter,
                        const string& range,
                        vector<Tag> tags,
                        const string& description,
                        size_t decimals = 0);
    ~Variable() = default;

    string GetName() const;
    VarType GetType() const;
    string GetFilePath() const;
    size_t GetDataSize() const;
    string GetFilter() const;
    string GetRange() const;
    vector<Tag> GetTags() const;
    string GetDescription() const;
    size_t GetDecimals() const;
    std::shared_ptr<void> GetValues() const;

    // for R's cpp11
    void Parse() { ParseValues(); }

private:
    string m_name;
    VarType m_type;
    string m_idxFileName;
    size_t m_dataSize;
    string m_filter;
    string m_range;
    vector<Tag> m_tags;
    string m_description;
    size_t m_decimals;
    std::shared_ptr<void> m_values;

    void ParseValues();

    void ParseStrings(size_t length, ByteArrayReader reader);
    void ParseIntegers(size_t size, ByteArrayReader reader);
    void ParsePCK(size_t size, ByteArrayReader reader);
    void ParseBIN(size_t size, ByteArrayReader reader);
    void ParseFloats(ByteArrayReader reader);
};

} // namespace RedatamLib

#endif // REDATAMLIB_VARIABLE_HPP
