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

#include <fstream>
#include <iostream>
#include <map>


std::string case_path = "";
std::string keywords_path = "";
int cur_num = 0;
std::map<std::string, int> keywords;

void Usage() {
  std::cout << "train case_path keywords_path" << std::endl;
}

int InitKeywords() {
  std::fstream fs (keywords_path.c_str(), std::fstream::in);
  if (!fs.is_open()) {
    std::cout << "Failed to find keywords_path=" << keywords_path << "||" << std::endl;
    return -1;
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

int ParseCase(std::string* result) {
  std::fstream kw_out (keywords_path.c_str(), std::fstream::out | std::fstream::app);
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
          } else {
            keywords[word] = ++cur_num;
            id = keywords[word];
            kw_out << word << "=" << keywords[word] << std::endl;
          }
          snprintf(id_buffer, sizeof(id_buffer), "%d", id);
          result->append(std::string(id_buffer) + " ");
        }
      }
    }
  }

  fs_case.close();
  kw_out.close();
  return 0;
}

int main(int argc, char** argv) {
  if (argc != 3) {
    Usage();
    return -1;
  }

  case_path = argv[1];
  keywords_path = argv[2];

  if (InitKeywords() != 0) {
    return -1;
  }

  std::string transform_output = "";
  if (ParseCase(&transform_output) != 0) {
    return -1;
  }

  const std::string case_out_path = case_path + ".transform";
  std::fstream case_out (case_out_path.c_str(), std::fstream::out);
  if (!case_out.is_open()) {
    std::cout << "Failed to find case_out_path=" << case_out_path << "||" << std::endl;
    return -1;
  }
  case_out << transform_output << std::endl;
  case_out.close();

  return 0;
}
