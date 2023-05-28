#include "OpenSim/Common/TimeSeriesTable.h"
#include <OpenSim/Auxiliary/auxiliaryTestFunctions.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

int main() {
    using namespace OpenSim;

    std::ifstream file("rodentData.csv");
    if (!file.is_open()) {
        std::cerr << "Failed to open rodentData.csv" << std::endl;
        return 1;
    }
    
    std::vector<SimTK::RowVector_<double>> rows;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        double value;
        SimTK::RowVector_<double> row;
        while (iss >> value) {
            row.push_back(value);
            if (iss.peek() == ',')
                iss.ignore();
        }
        rows.push_back(row);
    }
    file.close();

    DataTable dataTable;
    for (size_t i = 0; i < rows[0].size(); ++i)
        dataTable.setColumnLabel(i, std::to_string(i));

    for (const auto& row : rows)
        dataTable.appendRow(row);

    TimeSeriesTable timeSeriesTable(dataTable);

    std::cout << dataTable << std::endl;
    std::cout << timeSeriesTable << std::endl;

    return 0;
}
