//
// Created by brian on 9/8/22.
//

#include <pinocchio/algorithm/joint-configuration.hpp>
#include <pinocchio/algorithm/kinematics.hpp>
#include <pinocchio/algorithm/rnea-derivatives.hpp>
#include <pinocchio/algorithm/rnea.hpp>
// #include <pinocchio/codegen/cppadcg.hpp>
#include <pinocchio/codegen/code-generator-algo.hpp>
#include <pinocchio/parsers/urdf.hpp>
// #include <pinocchio/algorithm/
#include <stdlib.h>

#include "fmt/core.h"
#include "formatting.hpp"

#ifdef PINOCCHIO_MODEL_DIR
const char *MODEL_DIR = PINOCCHIO_MODEL_DIR;
#else
const char *MODEL_DIR = "None";
#endif

using namespace pinocchio;

void calcDynamicsJacobians(CodeGenRNEA<double> rnea,
                       CodeGenRNEADerivatives<double> &drnea,
                       CodeGenMinv<double> &minv, Eigen::MatrixXd &A,
                       Eigen::MatrixXd &B, const Eigen::VectorXd &q,
                       const Eigen::VectorXd &v, const Eigen::VectorXd &u) {
  // Evaluate Minv
  minv.evalFunction(q);
  minv.Minv.template triangularView<Eigen::StrictlyLower>() =
      minv.Minv.template triangularView<Eigen::StrictlyLower>();

  // Calculate accelerations
  int n = v.size();
  Eigen::VectorXd qdd = Eigen::VectorXd::Zero(n);
  rnea.evalFunction(q, v, qdd);
  qdd = minv.Minv * (u - rnea.res);
}

int main() {

  // URDF file location
  const std::string urdf_filename =
      PINOCCHIO_MODEL_DIR +
      std::string(
          "/example-robot-data/robots/ur_description/urdf/ur5_robot.urdf");

  // Load the model
  Model model;
  pinocchio::urdf::buildModel(urdf_filename, model);
  fmt::print("model name: {}\n", model.name);

  // Create some data
  Data data(model);

  // Sample a random configuration
  Eigen::VectorXd q = randomConfiguration(model);
  fmt::print("q: {}\n", q.transpose().eval());

  // Forward kinematics
  forwardKinematics(model, data, q);
  for (JointIndex joint_id = 0; joint_id < (JointIndex)model.njoints;
       ++joint_id) {
    fmt::print("{:<25s}: {}\n", model.names[joint_id],
               data.oMi[joint_id].translation().transpose().eval());
  }

  // Code gen
  CodeGenRNEA<double> rnea_codegen(model.cast<double>());
  CodeGenMinv<double> minv_codegen(model.cast<double>());
  CodeGenRNEADerivatives<double> drnea_codegen(model.cast<double>());

  return EXIT_SUCCESS;
}
