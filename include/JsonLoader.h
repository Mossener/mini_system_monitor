#pragma once

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <exception>
#include <nlohmann/json.hpp>
class OpenFileException final : public std::logic_error{
public:
  OpenFileException(): std::logic_error("read file is not ac open"){

  }
};
class JsonLoader {
public:
  nlohmann::json read_json(const std::string &file_path) const {
      std::ifstream ifs(file_path);
      if(!ifs.is_open()){
        throw OpenFileException{};
      }
      nlohmann::json j;
      ifs >> j;
      return j;
  }

};