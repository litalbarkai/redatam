#include <sstream>          //  std::ostringstream
#include <fstream>          //  std::ofstream
#include <iostream>         //  std::endl
#include <thread>

#include "CSVExporter.hpp"
#include "ParentIDCalculator.hpp"
#include "utils.hpp"

namespace RedatamLib
{
using std::endl;
using std::ofstream;
using std::ostringstream;

CSVExporter::CSVExporter(const string& outputDirectory) : m_path(outputDirectory)
{
    if ('/' != m_path.back())
    {
        m_path.append("/");
    }

    std::string labels_path = m_path + "Labels/";
    if (!exists(labels_path)) {
      create_directories(labels_path);
    }
}

void CSVExporter::ExportAll(vector<Entity>& entities)
{
    size_t numThreads = std::thread::hardware_concurrency();
    numThreads = std::min(entities.size(), numThreads);

    size_t chunkSize = entities.size() / numThreads;

    std::vector<std::thread> threads;
    for (size_t i = 0; i < numThreads; ++i)
    {
        size_t start = i * chunkSize;
        size_t end = (i == numThreads - 1) ? entities.size() : start + chunkSize;

        threads.push_back(std::thread(ThreadExport,
                                        std::ref(m_mtx),
                                        start, end,
                                        std::ref(entities),
                                        m_path));
    }

    for (auto& t : threads) {
        t.join();
    }
}

//  static
void CSVExporter::CreateVariablesLegend(std::mutex& mutex, Entity& e, const string& outputDirectory)
{
    string entityName = e.GetName();
    ostringstream os;

    os << "variable;label" << endl
        << entityName << "_REF_ID;Identifier for " << entityName << endl;
    
    if (!e.GetParentName().empty())
    {
        os << e.GetParentName() << "_REF_ID;Parent entity Id" << endl;
    }
    
    for (Variable& v : *(e.GetVariables().get()))
    {
        os << v.GetName() << ";" << v.GetDescription() << endl;
    }

    std::lock_guard<std::mutex> lock(mutex);
    ofstream fs(outputDirectory + "Labels/" + entityName + "-VARIABLES.csv");
    ThrowIfBad<std::ios_base::failure>(fs.is_open(),
        std::ios_base::failure("Error: Failed to create file."));
    fs << os.str();
}

//  static
void CSVExporter::CreateVariablesLabels(std::mutex& mutex, Entity& e, const string& outputDirectory)
{
    string path = outputDirectory + "Labels/" + e.GetName() + "-";

    for (Variable& v : *(e.GetVariables().get()))
    {
        if (!v.GetTags().empty())
        {
            ostringstream os;

            os << v.GetName() << ";label" << endl;
            for (Tag t : v.GetTags())
            {
                os << t.first << ";" << t.second << endl;
            }

            std::lock_guard<std::mutex> lock(mutex);
            ofstream fs(path + v.GetName() + "-LABELS.csv");
            ThrowIfBad<std::ios_base::failure>(fs.is_open(),
                std::ios_base::failure("Error: Failed to create file."));
            fs << os.str();
        }
    }
}

//  static
void CSVExporter::CreateVariablesData(std::mutex& mutex, Entity& e, const string& outputDirectory)
{
    ostringstream os;
    bool hasParent = !e.GetParentName().empty();
    ParentIDCalculator pIDCalc(&e);

    os << e.GetName() << "_REF_ID";
    if (hasParent)
    {
        os << ";" << e.GetParentName() << "_REF_ID";
    }

    for (Variable& v : *(e.GetVariables().get()))
    {
        os << ";" << v.GetName();
    }

    size_t rows = 1 + e.GetRowsCount();
    for (size_t row = 1; row < rows; ++row)
    {
        os << endl << row;
        if (hasParent)
        {
            os << ";" << pIDCalc.GetParentID(row);
        }

        for (Variable& v : *(e.GetVariables().get()))
        {
            switch (v.GetType())
            {
            case BIN:
            case PCK:
            case INT:
            case LNG:
                os << ";" << (*(static_cast<vector<uint32_t>*>(v.GetValues().get())))[row - 1];
                break;

            case CHR:
                os << ";" << (*(static_cast<vector<string>*>(v.GetValues().get())))[row - 1];
                break;

            case DBL:
                os << ";" << (*(static_cast<vector<double>*>(v.GetValues().get())))[row - 1];
                break;
            
            default:
                break;
            }
        }
    }

    std::lock_guard<std::mutex> lock(mutex);
    ofstream fs(outputDirectory + e.GetName() + ".csv");
    ThrowIfBad<std::ios_base::failure>(fs.is_open(),
        std::ios_base::failure("Error: Failed to create file."));
    fs << os.str();
}

//  static
void CSVExporter::ThreadExport(std::mutex& mutex,
                    size_t start, size_t end,
                    vector<Entity>& entities,
                    const string& outputDirectory)
{
    for (size_t i = start; i < end; ++i)
    {
        std::cout << "Exporting " << entities[i].GetName() << "..." << std::endl;
        CreateVariablesLegend(mutex, entities[i], outputDirectory);
        CreateVariablesLabels(mutex, entities[i], outputDirectory);
        CreateVariablesData(mutex, entities[i], outputDirectory);
    }
}
} // namespace RedatamLib