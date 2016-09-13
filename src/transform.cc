/*
* transform
* --------------------------------
*
* Copyright 2015 Kai Zhao <loverszhao@gmail.com>
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*/

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

std::string model = "";
std::string class_name = "";
std::string case_path = "";
std::string keywords_path = "";
std::string output_path = "";
int cur_num = 0;
std::map<std::string, int> keywords;

void Usage() {
  std::cout << "transform [train|test] class_name case_path keywords_path output_path" << std::endl;
}

int InitKeywords() {
  std::fstream fs (keywords_path.c_str(), std::fstream::in);
  if (!fs.is_open()) {
    return 0;
  }

  std::string line, word, num;
  while (getline(fs, line)) {
    word = line.substr(0, line.find('='));
    num = line.substr(line.find('=') + 1);
    cur_num = atoi(num.c_str());
    keywords[word] = cur_num;
    std::cout << "line='" << line << "'" << std::endl;
    std::cout << "word='" << word << "'" << std::endl;
    std::cout << "num='" << num << "'" << std::endl << std::endl;
  }

  fs.close();
  return 0;
}

int ParseCase(std::vector<std::string>* result) {
  std::fstream kw_out (keywords_path.c_str(),
                       std::fstream::out | std::fstream::app);
  if (!kw_out.is_open()) {
    std::cout << "Failed to find keywords_path=" << keywords_path << "||" << std::endl;
    return -1;
  }

  std::fstream fs_case (case_path.c_str(), std::fstream::in);
  if (!fs_case.is_open()) {
    std::cout << "Failed to find case_path=" << case_path << "||" << std::endl;
    return -1;
  }

  std::cout << "Begin to parse "<< case_path << std::endl;
  
  std::string line;
  char id_buffer[10];
  while (getline(fs_case, line)) {
    std::cout << "line=" << line << std::endl;
    size_t begin_index = 0;
    std::vector<int> numbers;
    for (size_t i = 0; i < line.size(); i++) {
      if (!isalpha(line[i]) || i + 1 == line.size()) {
        std::string word = line.substr(begin_index, i - begin_index);
        if (isalpha(line[i]) && i + 1 == line.size()) {
          word = line.substr(begin_index);
        }
        begin_index = i + 1;
        if (word.size() != 0) {
          int id = 0;
          if (keywords.find(word) != keywords.end()) {
            id = keywords[word];
          } else if (model == "train") {
            keywords[word] = ++cur_num;
            id = keywords[word];
            kw_out << word << "=" << keywords[word] << std::endl;
          }
          if (id != 0) {
            numbers.push_back(id);
          }
        }
      }
    }
    std::sort(numbers.begin(), numbers.end());
    std::string numbers_str = "";
    for (const auto iter : numbers) {
      snprintf(id_buffer, sizeof(id_buffer), "%d:1", iter);
      numbers_str += " " + std::string(id_buffer);
    }
    result->push_back(numbers_str);
  }

  fs_case.close();
  kw_out.close();
  return 0;
}

int main(int argc, char** argv) {
  if (argc != 6) {
    Usage();
    return -1;
  }

  model = argv[1];
  class_name = argv[2];
  case_path = argv[3];
  keywords_path = argv[4];
  output_path = argv[5];

  if (InitKeywords() != 0) {
    return -1;
  }

  std::string output = "";
  std::vector<std::string> outputs;
  if (ParseCase(&outputs) != 0) {
    return -1;
  }

  std::fstream out_fs (output_path.c_str(),
                       std::fstream::out | std::fstream::app);
  if (!out_fs.is_open()) {
    std::cout << "Failed to find output_path=" << output_path << "||" << std::endl;
    return -1;
  }

  for (const auto &iter : outputs) {
    out_fs << class_name << iter << std::endl;
  }
  out_fs.close();

  return 0;
}
