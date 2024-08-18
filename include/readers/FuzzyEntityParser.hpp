//  replaces DataBlock.cs
#ifndef REDATAMLIB_FUZZYENTITYPARSER_HPP
#define REDATAMLIB_FUZZYENTITYPARSER_HPP

#include <vector>
#include <string>
#include <utility>          //  std::pair
#include <unordered_map>

#include "Entity.hpp"
#include "Variable.hpp"
#include "ByteArrayReader.hpp"

namespace RedatamLib {
using std::vector, std::string, std::pair, std::unordered_map;

class FuzzyEntityParser
{
public:
    FuzzyEntityParser(string filePath);
    FuzzyEntityParser(ByteArrayReader reader);
    ~FuzzyEntityParser() = default;

    FuzzyEntityParser(const FuzzyEntityParser&) = delete;
    FuzzyEntityParser& operator=(const FuzzyEntityParser&) = delete;

    vector<Entity> ParseEntities();

private:
    ByteArrayReader m_reader;

    //  throws std::out_of_range
    pair<bool, Entity> TryGetEntity();

    //  Note: changes entities inner values
    static void AssignChildren(vector<Entity>& entitites,
                                unordered_map<string, Entity*> mapping);
};

} // namespace RedatamLib

#endif  //  REDATAMLIB_FUZZYENTITYPARSER_HPP