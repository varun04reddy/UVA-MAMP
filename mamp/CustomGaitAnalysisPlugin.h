#ifndef CUSTOM_GAIT_ANALYSIS_PLUGIN_H
#define CUSTOM_GAIT_ANALYSIS_PLUGIN_H

#include <OpenSim/OpenSim.h>
#include <fstream>
#include <iostream>
#include <Eigen/Dense>
#include <cmath>
#include <sstream>
#include <string>

namespace OpenSim {

    class CustomGaitAnalysisPlugin : public Object {
    public:
        CustomGaitAnalysisPlugin();
        CustomGaitAnalysisPlugin(const std::string& fileName);

        void begin(const Model& model) const;

    private:
        const int m_crit = 10;
        double d_alpha;

        Eigen::MatrixXd loadDataMatrix(const std::string& fileName) const;
        double computeGDI(const Eigen::VectorXd& gaitDistances) const;

    };

}

#endif // CUSTOM_GAIT_ANALYSIS_PLUGIN_H
