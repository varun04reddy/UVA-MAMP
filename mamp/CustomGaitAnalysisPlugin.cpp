#include "CustomGaitAnalysisPlugin.h"
#include <fstream>
#include <iostream>
#include <Eigen/Dense>
#include <cmath>
#include <sstream>
#include <string>

using namespace OpenSim;

CustomGaitAnalysisPlugin::CustomGaitAnalysisPlugin() : Object() {}

CustomGaitAnalysisPlugin::CustomGaitAnalysisPlugin(const std::string& fileName) : Object(fileName) {}

void CustomGaitAnalysisPlugin::begin(const Model& model) const {
    const int m_crit = 10;

    std::ifstream inputFile("GaitRodentdata.csv");
    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open the file." << std::endl;
        return;
    }

    std::vector<std::vector<double>> data;
    std::string line;
    while (std::getline(inputFile, line)) {
        std::vector<double> row;
        std::stringstream ss(line);
        std::string value;
        while (std::getline(ss, value, ',')) {
            row.push_back(std::stod(value));
        }
        data.push_back(row);
    }
    inputFile.close();

    int numRows = static_cast<int>(data.size());
    int numColumns = static_cast<int>(data[0].size());

    Eigen::MatrixXd G(numRows, numColumns);
    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numColumns; ++j) {
            G(i, j) = data[i][j];
        }
    }

    Eigen::JacobiSVD<Eigen::MatrixXd> svd(G, Eigen::ComputeThinU | Eigen::ComputeThinV);
    Eigen::VectorXd singularValues = svd.singularValues();
    Eigen::MatrixXd U = svd.matrixU();
    Eigen::MatrixXd V = svd.matrixV();

    Eigen::MatrixXd f_basis = U.block(0, 0, numRows, numRows);
    Eigen::VectorXd singularValues_m = singularValues.head(numRows);

    Eigen::VectorXd g(numRows);
    for (int i = 0; i < numRows; ++i) {
        g(i) = 1.0;
    }

    Eigen::VectorXd g_tilde = Eigen::VectorXd::Zero(numRows);
    Eigen::VectorXd feature_components = Eigen::VectorXd::Zero(numRows);

    for (int k = 0; k < numRows; ++k) {
        double c_k = g.dot(f_basis.col(k));
        g_tilde += c_k * f_basis.col(k);
        feature_components(k) = c_k;
    }

    double vaf_m = singularValues_m.head(m_crit).array().square().sum() / singularValues.array().square().sum();

    double phi = g.dot(g_tilde) / (g.norm() * g_tilde.norm());

    Eigen::VectorXd gaitDistances(numRows);
    for (int i = 0; i < numRows; ++i) {
        double distance = std::abs(feature_components(i) - mean_GDI_raw);
        gaitDistances(i) = std::log(distance);
    }

    double GDI_raw = std::log(d_alpha);
    double mean_GDI_raw = gaitDistances.mean();
    double sd_GDI_raw = gaitDistances.array().sqrt().mean();

    double z_GDI_raw = (GDI_raw - mean_GDI_raw) / sd_GDI_raw;
    double GDI = 100.0 - 10.0 * z_GDI_raw;

    std::cout << "VAF_m: " << vaf_m << std::endl;
    std::cout << "Phi: " << phi << std::endl;
    std::cout << "GDI: " << GDI << std::endl;
}
