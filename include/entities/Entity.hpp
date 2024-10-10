#ifndef REDATAMLIB_ENTITY_HPP
#define REDATAMLIB_ENTITY_HPP

#include <string>
#include <vector>
#include <memory>
#include <utility>  //  std::pair

#include "Variable.hpp"
#include "ByteArrayReader.hpp"

namespace RedatamLib
{
using std::pair;
using std::shared_ptr;
using std::string;
using std::vector;

class Entity {
public:
    explicit Entity();
    explicit Entity(const string& name,
            const string& parentName,
            const string& description,
            const string& idxFileName,
            pair<size_t, size_t> bounds);
    ~Entity() = default;

    string GetName() const;
    string GetParentName() const;
    string GetPTRPath() const;

    shared_ptr<vector<Variable>> GetVariables() const;
    size_t GetRowsCount() const;

    pair<size_t, size_t> GetBounds() const;

    size_t GetPTRData();

    Entity* GetChild() const;

    void AttachChild(Entity* child);
    void AttachVariables(shared_ptr<vector<Variable>> variables);

private:
    string m_name;
    string m_parentName;
    string m_description;
    string m_indexFilename;
    Entity* m_child;
    shared_ptr<vector<Variable>> m_variables;
    pair<size_t, size_t> m_bounds;
    ByteArrayReader m_reader;
    size_t m_rowsCount;
};
} // namespace RedatamLib

#endif // REDATAMLIB_ENTITY_HPP
