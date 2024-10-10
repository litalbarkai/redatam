#include <algorithm>    //  std::replace

#include "FuzzyEntityParser.hpp"
#include "FuzzyVariableParser.hpp"
#include "utils.hpp"                //  GetFileExtension, ThrowIfBad

namespace RedatamLib
{
FuzzyEntityParser::FuzzyEntityParser(const string& filePath) :
    m_reader(filePath),
    m_rootPath(FindRootPath(filePath))
{}

vector<Entity> FuzzyEntityParser::ParseEntities()
{
    pair<bool, Entity> curr;
    vector<Entity> ret;
    unordered_map<string, Entity*> entities;

    try
    {
        while (true)
        {
            curr = TryGetEntity();
            if (curr.first)
            {
                ret.push_back(curr.second);
            }
            else
            {
                m_reader.MovePos(1);
            }
        }
    }
    catch (const std::out_of_range&)
    {}

    for (Entity& e : ret)
    {
        entities[e.GetName()] = &e;
    }

    AssignChildren(ret, entities);

    FuzzyVariableParser varParser(m_reader, m_rootPath);
    varParser.ParseAllVariables(ret);

    return ret;
}

pair<bool, Entity> FuzzyEntityParser::TryGetEntity()
{
    size_t ogPos = m_reader.GetPos();

    string entityName("");
    if (!m_reader.TryReadStr(&entityName) || entityName.empty())
    {
        m_reader.SetPos(ogPos);
        return std::pair<bool, Entity>(false, Entity());
    }

    string entityNameRepeated("");
    if (!m_reader.TryReadStr(&entityNameRepeated))
    {
        m_reader.MovePos(2);
    }
    else if (entityName != entityNameRepeated)
    {
        m_reader.SetPos(ogPos);
        return std::pair<bool, Entity>(false, Entity());
    }

    string parentEntityName("");
    if (!entityNameRepeated.empty() && !m_reader.TryReadStr(&parentEntityName))
    {}

    string description("");
    if (!m_reader.TryReadStr(&description, false))
    {
        m_reader.MovePos(2);
    }

    string idxFileName(""), ext("");
    if ((!m_reader.TryReadStr(&idxFileName, false)) ||
        !(!idxFileName.empty() && TryGetFileExtension(idxFileName, &ext) &&
        ".ptr" == ext))
    {
        m_reader.SetPos(ogPos);
        return std::pair<bool, Entity>(false, Entity());
    }
    idxFileName = ReplaceRootPath(m_rootPath, idxFileName);

    pair<size_t, size_t> bounds(ogPos, m_reader.GetPos());

    return std::pair<bool,Entity>(true, Entity(entityName, parentEntityName, description,
            idxFileName, bounds));
}

//  static
void FuzzyEntityParser::AssignChildren(vector<Entity>& entitites,
                                        unordered_map<string, Entity*> mapping)
{
    for (Entity& e : entitites)
    {
        string parent = e.GetParentName();
        if ("" != parent)
        {
            mapping[parent]->AttachChild(&e);
        }
    }
}

} // namespace RedatamLib
