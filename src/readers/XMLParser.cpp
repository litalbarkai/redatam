#include <algorithm>    //  std::replace

#include "XMLParser.hpp"
#include "utils.hpp"

namespace RedatamLib
{
using std::cerr;
using std::endl;

// Constructor
XMLParser::XMLParser() 
{
  // No need for initialization, PugiXML handles everything internally
}

// Destructor
XMLParser::~XMLParser(){
  // No need for explicit cleanup, PugiXML handles resource management
}

vector<Entity> XMLParser::ParseFile(const string& fileName)
{
    m_rootPath = FindRootPath(fileName);

    vector<Entity> ret;

    try
    {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(fileName.c_str());
        if (!result) {
            std::string errorMsg =
                "Error parsing file: " + std::string(result.description());
            throw std::runtime_error(errorMsg);
        }

        // Look for the correct root element
        pugi::xml_node redDictionaryNode = doc.child("redDictionary_XML");
        if (!redDictionaryNode) {
            throw std::runtime_error("redDictionary_XML element not found");
        }

        pugi::xml_node rootElement = redDictionaryNode.child("root");
        if (!rootElement) {
            throw std::runtime_error("Root element not found");
        }

        pugi::xml_node child = ParseEntity(&ret, rootElement);
        while (child)
        {
            child = ParseEntity(&ret, child, ret.back().GetName());
        }

        for (size_t idx = 0; idx < ret.size() - 1; ++idx)
        {
            ret[idx].AttachChild(&ret[idx + 1]);
        }
    }
    catch(const std::exception& e)
    {
        std::string errorMsg = "Error: " + std::string(e.what());
        cerr << errorMsg << endl;
        throw;
    }

    return ret;
}

string XMLParser::GetTagValue(pugi::xml_node node, const string& tag, size_t idx)
{
    pugi::xml_node child = node.child(tag.c_str());
    return child ? child.child_value() : "";
}

pugi::xml_node XMLParser::ParseEntity(vector<Entity>* results, pugi::xml_node node, const string& parentName)
{
    string name = GetTagValue(node, "name");

    string description = GetTagValue(node, "label");

    string idxFileName = GetTagValue(node, "filename");
    idxFileName = ReplaceRootPath(m_rootPath, idxFileName);

    Entity curr(name, parentName, description, idxFileName, pair<size_t, size_t>(0, 0));

    shared_ptr<vector<Variable>> variables = ParseVariables(node);
    curr.AttachVariables(variables);

    results->push_back(curr);

    pugi::xml_node child = node.child("entity");
    return child;
}

shared_ptr<vector<Variable>> XMLParser::ParseVariables(pugi::xml_node node)
{
    shared_ptr<vector<Variable>> ret(new vector<Variable>);

    for (pugi::xml_node var : node.children("variable"))
    {
        string name = GetTagValue(var, "name");

        pair<VarType, size_t> typeDetails = ParseVarTypeAndSize(var);

        string idxFileName = GetTagValue(var, "filename");
        idxFileName = ReplaceRootPath(m_rootPath, idxFileName);

        string filter = GetTagValue(var, "filter");

        string range = ParseVarRange(var);

        vector<Tag> tags = ParseVarTags(var);

        string description = GetTagValue(var, "label");

        string decimalsStr = GetTagValue(var, "decimals");
        size_t decimals = decimalsStr.empty() ? 0 : std::stoi(decimalsStr);

        ret->push_back(Variable(name, typeDetails.first, idxFileName,
                            typeDetails.second, filter, range, tags,
                            description, decimals));
    }

    return ret;
}

vector<pugi::xml_node> XMLParser::GetChildren(pugi::xml_node parent, const string& tag)
{
    vector<pugi::xml_node> children;

    for (pugi::xml_node currElement = parent.first_child(); currElement; currElement = currElement.next_sibling())
    {
        if (tag == currElement.name())
        {
        children.push_back(currElement);
        }
    }

    return children;
}

pair<VarType, size_t> XMLParser::ParseVarTypeAndSize(pugi::xml_node var)
{
    pugi::xml_node details = var.child("varDicChoice");

    VarType varType = NA;
    string type = GetTagValue(details, "datasetType");
    if ("BIN" == type)
    {
        varType = BIN;
    }
    else if ("CHR" == type)
    {
        varType = CHR;
    }
    else if ("DBL" == type)
    {
        varType = DBL;
    }
    else if ("INT" == type)
    {
        varType = INT;
    }
    else if ("LNG" == type)
    {
        varType = LNG;
    }
    else if ("PCK" == type)
    {
        varType = PCK;
    }

    size_t size = std::stoi(GetTagValue(details, "datasetSize"));

    return {varType, size};
}

string XMLParser::ParseVarRange(pugi::xml_node var)
{
    pugi::xml_node rangeTag = var.child("range");
    if (!rangeTag)
    {
        return "";
    }

    string ret = GetTagValue(rangeTag, "rangeMin");
    if (!ret.empty())
    {
        ret.append(" TO ");
        ret.append(GetTagValue(rangeTag, "rangeMax"));
    }

    return ret;
}

vector<Tag> XMLParser::ParseVarTags(pugi::xml_node var)
{
    vector<Tag> ret;

    pugi::xml_node valueLabelTag = var.child("valueLabels");
    if (!valueLabelTag)
    {
        return ret;
    }

    for (pugi::xml_node valueLabel : valueLabelTag.children("valueLabelElement"))
    {
        string key = GetTagValue(valueLabel, "value");
        string value = GetTagValue(valueLabel, "label");

        ret.push_back(Tag(key, value));
    }

    ret.push_back(Tag(GetTagValue(var, "missing"), "MISSING"));
    ret.push_back(Tag(GetTagValue(var, "notappl"), "NOTAPPLICABLE"));

    return ret;
}
}   // namespace RedatamLib