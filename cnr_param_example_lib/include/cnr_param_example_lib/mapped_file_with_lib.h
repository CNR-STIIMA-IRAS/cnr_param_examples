/**
 * @file test_yaml_server.cpp
 * @brief This file contains the unit tests for the YAML server functionality.
 */
#include <cstdlib>
#include <ostream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <iostream>

#if MAPPED_FILE_MODULE

#include <boost/interprocess/detail/os_file_functions.hpp>

#include <cnr_yaml/eigen.h>

#include <cnr_param/cnr_param.h>
#include <cnr_param/core/string.h>

#include <cnr_param/mapped_file/args_parser.h>
#include <cnr_param/mapped_file/yaml_parser.h>
#include <cnr_param/mapped_file/yaml_manager.h>

// ====================================================================================================================
// === DeveloperTest, GetComplexType ==================================================================================
// First we define a complex type
// then we inherit the get_map function to extract the complex type from the YAML node
// finally, we run the test
// ====================================================================================================================
struct ComplexType
{
  std::string name;
  double value;
};

namespace YAML
{
template <>
struct convert<ComplexType>
{
  static Node encode(const ComplexType& rhs)
  {
    Node node;
    node["name"] = rhs.name;
    node["value"] = rhs.value;
    return node;
  }

  static bool decode(const Node& node, ComplexType& rhs)
  {
    if (!node.IsMap() || !node["name"] || !node["value"])
    {
      return false;
    }
    rhs.name = node["name"].as<std::string>();
    rhs.value = node["value"].as<double>();
    return true;
  }
};
}  // namespace YAML

namespace std
{
inline std::string to_string(const ComplexType& n)
{
  return "Name: " + n.name + ", Value: " + std::to_string(n.value);
}

template <>
inline std::string to_string(const std::vector<ComplexType>& vv)
{
  std::string ret = "[ ";
  for (auto const& v : vv)
    ret += std::to_string(v) + " ";
  ret += "]";
  return ret;
}

}  // namespace std

struct ParamManager
{
  const std::string default_param_root_directory_ = boost::interprocess::ipcdetail::get_temporary_path();

  std::string param_root_directory_;

  ParamManager(int argc, const char* arv[]);

  template <typename T, typename O = T>
  bool call(const std::string& key, T& value, O* o = nullptr)
  {
    std::string what;
    if (o)
    {
      if (!cnr::param::get(key, value, what, *o))
      {
        std::cerr << "Key: " << key << ", What: " << what << std::endl;
        return false;
      }
    }
    else
    {
      if (!cnr::param::mapped_file::get(key, value, what))
      {
        std::cerr << "Key: " << key << ", What: " << what << std::endl;
        return false;
      }
    }

    std::cout << "Key: " << key << ", Value: " << std::to_string(value) << std::endl;
    return true;
  }

  template <typename T>
  bool send(const std::string& key, const T& value)
  {
    std::string what;
    if (!cnr::param::mapped_file::set(key, value, what))
    {
      std::cerr << "Key: " << key << ", What: " << what << std::endl;
      return false;
    }

    return true;
  }
};

#else

struct ParamManager
{
  ParamManager()
  {
      throw std::runtime_error("This test is the yaml server test, availalbe for the MAPPED_FILE_MODULE, but the MAPPED_FILE_MODULE is not defined.\n");
  }
  template <typename T, typename O = T>
  bool call(const std::string& key, T& value, O* o = nullptr)
  {
    return false;
  }
  template <typename T>
  bool send(const std::string& key, const T& value)
  {
    return false;
  }
};
#endif