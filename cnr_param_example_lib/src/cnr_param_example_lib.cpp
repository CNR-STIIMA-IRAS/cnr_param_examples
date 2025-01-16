/**
 * @file test_yaml_server.cpp
 * @brief This file contains the unit tests for the YAML server functionality.
 */
#include <cstdlib>
#include <stdexcept>
#include <string>

#include <cnr_param_example_lib/mapped_file_with_lib.h>

#if MAPPED_FILE_MODULE

ParamManager::ParamManager(int argc, const char* argv[])
{
  const char* env_p = std::getenv("CNR_PARAM_ROOT_DIRECTORY");
  param_root_directory_ = (env_p) ? std::string(env_p) : default_param_root_directory_;
  int rc = setenv("CNR_PARAM_ROOT_DIRECTORY", param_root_directory_.c_str(), true);
  if (rc != 0)
  {
    throw std::runtime_error(("Errono" + std::to_string(rc) + ": " + strerror(rc)).c_str());
  }

  const std::string default_shmem_name = "param_server_default_shmem";

  cnr::param::mapped_file::ArgParser args(argc, argv, default_shmem_name);
  cnr::param::mapped_file::YAMLParser yaml_parser(args.getNamespacesMap());
  cnr::param::mapped_file::YAMLStreamer yaml_streamer(yaml_parser.root(), param_root_directory_);
}

#endif