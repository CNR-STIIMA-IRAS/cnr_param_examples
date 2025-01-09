/**
 * @file test_yaml_server.cpp
 * @brief This file contains the unit tests for the YAML server functionality.
 */
#include <cstdlib>
#include <string>

#include <cnr_param_example_lib/mapped_file_with_lib.h>

/**
 * @brief The main function of the program.
 *
 * This function initializes the parameter root directory based on the value of the environment variable
 * "CNR_PARAM_ROOT_DIRECTORY". If the environment variable is not set, it uses the default parameter root directory. It
 * then sets the environment variable "CNR_PARAM_ROOT_DIRECTORY" to the chosen value. If setting the environment
 * variable fails, it prints an error message. Finally, it initializes the Google Test framework and runs all the tests.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line arguments.
 * @return The exit code of the program.
 */
int main(int, char**)
{


  const std::string default_shmem_name = "param_server_default_shmem";

  // Parsing of program inputs
  const int argc = 13;
  const char* argv[] = {
    "test",
    "--ns-and-path-to-file",
    "/," TEST_DIR "/config/mqtt_config.yaml",
    "--ns-and-path-to-file",
    "/," TEST_DIR "/config/mqtt_config.yaml",
    "--ns-and-path-to-file",
    "/ns1," TEST_DIR "/config/drape_cell_hw.yaml",
    "--ns-and-path-to-file",
    "/ns2," TEST_DIR "/config/drape_cell_hw.yaml",
    "--ns-and-path-to-file",
    "/ns1/ns2," TEST_DIR "/config/drape_cell_hw.yaml",
    "--ns-and-path-to-file",
    "/," TEST_DIR "/config/par.yaml",
  };

  ParamManager pm(argc, argv);

  // BASIC USAGE OF THE CLIENT LIBRARY
  std::string what;
  std::string value;
  pm.call("/ns1/ns2/plan_hw/feedback_joint_state_topic", value);
  value = value + "_CIAO";

  pm.send("/ns1/ns2/plan_hw/feedback_joint_state_topic", value);
  pm.call("/ns1/ns2/plan_hw/feedback_joint_state_topic", value);

  pm.send("/ns1/ns3/plan_hw_NEW_NOT_IN_FILE/feedback_joint_state_topic", value);
  pm.call("/ns1/ns3/plan_hw_NEW_NOT_IN_FILE/feedback_joint_state_topic", value);

  int val = 9, before, after;
  pm.send("/a", val);
  pm.call("/a", before);
  pm.send("/a", "0x0009");
  pm.call("/a", after);
  pm.call("a", before);

  std::string defval = "DEFAULT";
  pm.call("/ns1/ns2/plan_hw/feedback_joint_state_topic__NOT_EXIST", value);
  pm.call("/ns1/ns2/plan_hw/feedback_joint_state_topic__NOT_EXIST", value, &defval);

  YAML::Node root;
  pm.call("/ns1/ns2/plan_hw/", root);

  std::vector<std::string> vv;
  pm.call("/n1/n3/v1", vv);

  std::vector<double> dd;
  pm.call("/n1/n3/v1", dd);
  pm.call("/n1/n3/v10", dd);

  Eigen::VectorXd ee;
  pm.call("/n1/n3/v10", ee);

  std::vector<std::vector<std::string>> vvv;
  pm.call("/n1/n4/vv1", vvv);

  std::vector<std::vector<double>> ddd;
  pm.call("/n1/n4/vv10", ddd);

  Eigen::MatrixXd eee;
  pm.call("/n1/n4/vv10", eee);

  std::vector<ComplexType> ct;
  pm.call("/n1/n4/test_vector_complex_type", ct);

  return 0;
}
