#pragma once

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
class JsonLoader {
public:
    nlohmann::json read_json(std::string file_path){
        std::ifstream ifs(file_path);
        if(!ifs.is_open()){
          throw std::runtime_error("Failed to open file...");
        }
        nlohmann::json j;
        ifs >> j;
        return j;
    }
    

private:

};