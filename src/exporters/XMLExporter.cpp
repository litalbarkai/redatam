#include <sstream>    //  std::ostringstream
#include <fstream>    //  std::ofstream
#include <iostream>   //  std::endl
#include <stdexcept>  //  std::runtime_error

#include "XMLExporter.hpp"
#include "utils.hpp"

namespace RedatamLib {
using std::endl;
using std::ofstream;
using std::ostringstream;
using std::runtime_error;

XMLExporter::XMLExporter(const std::string& outputDirectory) : m_path(outputDirectory + "/redatam_converter_description.xml") {
    pugi::xml_node decl = m_doc.append_child(pugi::node_declaration);
    decl.append_attribute("version") = "1.0";
    decl.append_attribute("encoding") = "UTF-8";

    m_doc.append_child("database");
}

void XMLExporter::ExportSummary(std::vector<Entity>& entities)
{
    pugi::xml_node root = m_doc.child("database");

    CreateEntityElement(entities[0], root);

    if (!m_doc.save_file(m_path.c_str(), "  "))
    {
        throw runtime_error("Error: Failed to save XML file.");
    }
}

void XMLExporter::SetAttribute(pugi::xml_node& node, const std::string& name, const std::string& value)
{
    node.append_attribute(name.c_str()) = value.c_str();
}

void XMLExporter::CreateEntityElement(Entity& e, pugi::xml_node& parentTag)
{
    pugi::xml_node elem = parentTag.append_child("entity");

    SetAttribute(elem, "name", e.GetName());
    SetAttribute(elem, "pointer", e.GetPTRPath());

    for (Variable& v : *(e.GetVariables().get()))
    {
        CreateVariableElement(v, elem);
    }

    Entity* child = e.GetChild();
    if (nullptr != child)
    {
        CreateEntityElement(*child, elem);
    }
}

void XMLExporter::CreateVariableElement(Variable& v, pugi::xml_node& parentTag)
{
    pugi::xml_node elem = parentTag.append_child("variable");

    SetAttribute(elem, "name", v.GetName());
    SetAttribute(elem, "label", v.GetDescription());
    SetAttribute(elem, "type", GetVarType(v.GetType()));
    SetAttribute(elem, "size", std::to_string(v.GetDataSize()));
    SetAttribute(elem, "decimals", std::to_string(v.GetDecimals()));
    SetAttribute(elem, "range", v.GetRange());
    SetAttribute(elem, "filter", v.GetFilter());
    SetAttribute(elem, "data", v.GetFilePath());

    vector<Tag> tags = v.GetTags();
    if (!tags.empty())
    {
        pugi::xml_node tagsHeader = elem.append_child("valueLabels");

        for (Tag& t : tags)
        {
            pugi::xml_node label = tagsHeader.append_child("valueLabel");
            SetAttribute(label, "name", t.first);
            SetAttribute(label, "value", t.second);
        }
    }
}

//  static
std::string XMLExporter::GetVarType(VarType type)
{
    switch (type)
    {
    case CHR:
        return "STRING";

    case DBL:
        return "REAL";

    case INT:
        return "INT16";

    case LNG:
    case BIN:
    case PCK:
        return "INTEGER";

    case NA:
    default:
        return "";
    }
}
}  // namespace RedatamLib
