#include <iostream>

#include "yaml-cpp/yaml.h"

template <typename T>
std::unique_ptr<std::vector<T>> parse_vector(YAML::Node &config,
                                             std::string &&key) {
  if (config[key] && config[key].IsSequence()) {
    auto msg = std::make_unique<std::vector<T>>();
    for (int i = 0; i < config[key].size(); ++i) {
      msg->emplace_back(config[key][i].as<T>());
    }
    for (const auto &message : *msg) {
      std::cout << message << std::endl;
    }
    return std::move(msg);
  }
  return nullptr;
}

int main() {
  YAML::Node config = YAML::LoadFile("../config.yaml");
  if (config["name"]) {
    std::cout << "Name: " << config["name"].as<std::string>() << std::endl;
  }
  parse_vector<int>(config, "numbers");
  parse_vector<std::string>(config, "strings");
  if (config["class"]) {
    std::cout << "Class: " << config["class"] << std::endl;
    const auto &classes = config["class"];
    if (classes.IsMap())
    {
      std::cout << "this is a map" << std::endl;
    }
  }
  return 0;
}
