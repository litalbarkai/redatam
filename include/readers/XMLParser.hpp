#ifndef REDATAMLIB_XMLPARSER_HPP
#define REDATAMLIB_XMLPARSER_HPP

#include <iostream>
#include <memory>  //  std::shared_ptr
#include <string>
#include <utility>  //  std::pair
#include <vector>

#include "pugixml.hpp"
#include "Entity.hpp"
#include "Variable.hpp"

namespace RedatamLib
{
using std::pair;
using std::shared_ptr;
using std::string;
using std::vector;

class XMLParser
{
public:
    explicit XMLParser();
    ~XMLParser();

    XMLParser(const XMLParser&) = delete;
    XMLParser& operator=(const XMLParser&) = delete;

    vector<Entity> ParseFile(const string& fileName);

private:
    pugi::xml_document m_doc;
    string m_rootPath;

    string GetTagValue(pugi::xml_node element, const string& tag, size_t idx = 0);
    pugi::xml_node ParseEntity(vector<Entity>* results, pugi::xml_node element,
                                const string& parentName = "");
    shared_ptr<vector<Variable>> ParseVariables(pugi::xml_node e);
    vector<pugi::xml_node> GetChildren(pugi::xml_node parent, const string& tag);
    pair<VarType, size_t> ParseVarTypeAndSize(pugi::xml_node var);
    string ParseVarRange(pugi::xml_node var);
    vector<Tag> ParseVarTags(pugi::xml_node var);
};
} // namespace RedatamLib

#endif  //  REDATAMLIB_XMLPARSER_HPP