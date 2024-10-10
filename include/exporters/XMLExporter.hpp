#ifndef REDATAMLIB_XMLEXPORTER_HPP
#define REDATAMLIB_XMLEXPORTER_HPP

#include <string>
#include <vector>

#include "Entity.hpp"
#include "pugixml.hpp"

namespace RedatamLib
{
using std::string;
using std::vector;

class XMLExporter {
 public:
    explicit XMLExporter(const string& outputDirectory);
    ~XMLExporter() = default;

    void ExportSummary(vector<Entity>& entities);

 private:
    string m_path;
    pugi::xml_document m_doc;

    void SetAttribute(pugi::xml_node& node, const string& name, const string& value);
    void CreateEntityElement(Entity& e, pugi::xml_node& parentTag);
    void CreateVariableElement(Variable& v, pugi::xml_node& parentTag);

    static string GetVarType(VarType type);
};
} // namespace RedatamLib

#endif // REDATAMLIB_XMLEXPORTER_HPP
