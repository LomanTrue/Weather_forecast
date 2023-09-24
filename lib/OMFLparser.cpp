#include "OMFLparser.h"

#include <fstream>

bool Value::IsInt() const {
  if (type == 'i') {
    return true;
  } else {
    return false;
  }
}

bool Value::IsFloat() const {
  if (type == 'f') {
    return true;
  } else {
    return false;
  }
}

bool Value::IsString() const {
  if (type == 's') {
    return true;
  } else {
    return false;
  }
}

bool Value::IsArray() const {
  if (type == 'a') {
    return true;
  } else {
    return false;
  }
}

bool Value::IsBool() const {
  if (type == 'b') {
    return true;
  } else {
    return false;
  }
}

int Value::AsInt() const {
  return std::get<int>(val);
}

int Value::AsIntOrDefault(int value) const {
  if (type == 'i') {
    return std::get<int>(val);
  } else {
    return value;
  }
}

float Value::AsFloat() const {
  return std::get<float>(val);
}

float Value::AsFloatOrDefault(float value) const {
  if (type == 'f') {
    return std::get<float>(val);
  } else {
    return value;
  }
}

std::string Value::AsString() const {
  return std::get<std::string>(val);
}

std::string Value::AsStringOrDefault(std::string value) const {
  if (type == 'f') {
    return std::get<std::string>(val);
  } else {
    return value;
  }
}

bool Value::AsBool() const {
  return std::get<bool>(val);
}

Value Value::operator[](int i) const {
  if (i >= val_array.size() || i < 0) {
    return *this;
  } else {
    return val_array[i];
  }
}

int Value::ParseValue(std::string& str) {
  bool is_str = false;
  for (int i = 0; i < str.size(); i++) {
    if (str[i] == '\"') {
      is_str = !is_str;
    }
    if (str[i] == '#' && !is_str) {
      str = str.substr(0,i - 1);
    }
  }

  while (str[0] == ' ') {
    str = str.substr(1,str.size() - 1);
    if (str.empty()) {
      return 1;
    }
  }

  while (str[str.size() - 1] == ' ') {
    str.pop_back();
    if (str.empty()) {
      return 1;
    }
  }

  if (str == "true" || str == "false") {
    this->type = 'b';
    if (str == "true") {
      this->val = true;
    } else {
      this->val = false;
    }
  } else if (str[0] == '\"' && str[str.size() - 1] == '\"') {
    this->type = 's';
    this->val = str.substr(1,str.size() - 2);
    for (int i = 1; i < str.size() - 1; i++) {
      if (str[i] == '\"') {
        return 1;
      }
    }
  } else if (str[0] == '[' && str[str.size() - 1] == ']') {
    this->type = 'a';
    std::string sub_val_str;
    uint32_t count = 0;
    is_str = false;

    if (str.size() == 2) {
      return 0;
    }

    for (int i = 1; i < str.size() - 1; i++) {
      if (str[i] == '\"') {
        is_str = !is_str;
      }
      if (!is_str) {
        if (str[i] == '[') {
          count++;
        }
        if (str[i] == ']') {
          count--;
        }

        if (str[i] == ',') {
          if (count < 0) {
            return 1;
          } else if (count == 0) {
            Value sub_val;
            if (sub_val.ParseValue(sub_val_str)) {
              return 1;
            }
            val_array.push_back(sub_val);

            sub_val_str.clear();
            continue;
          }
        }
      }

      sub_val_str += str[i];
    }

    if (sub_val_str.empty()) {
      return 1;
    } else {
      Value sub_val;
      if (sub_val.ParseValue(sub_val_str)) {
        return 1;
      }
      val_array.push_back(sub_val);
    }
  } else if (str[0] == '+' || str[0] == '-' || isdigit(str[0])) {
    if (str == "+" || str == "-") {
      return 1;
    }
    bool has_point = false;
    for (int i = 1; i < str.size(); i++) {
      if (str[i] == '.') {
        if (!isdigit(str[i - 1]) || has_point || i + 1 == str.size()) {
          return 1;
        }
        has_point = true;
        continue;
      }
      if (!isdigit(str[i])) {
        return 1;
      }
    }

    if (has_point) {
      this->type = 'f';
      this->val = std::stof(str);
    } else {
      this->type = 'i';
      this->val = std::stoi(str);
    }
  } else {
    return 1;
  }

  return 0;
}

void OMFLParser::ParseFile(const std::filesystem::path& path, const std::string& input_str) {
  bool has_path = true;

  if (path.empty()) {
    has_path = false;
  }
  std::ifstream file(path);
  std::string cur_str;
  std::string orig_str;
  if (!has_path) {
    if (input_str[0] == '\n') {
      orig_str = input_str.substr(1, input_str.size());
    } else {
      orig_str = input_str.substr(0, input_str.size());
    }
  }
  Section* cur_sect = &f_section;
  while (!has_path || std::getline(file, cur_str)) {
    if (!has_path) {
      std::size_t temp = orig_str.find('\n');
      if (temp == std::string::npos) {
        has_path = true;
        cur_str = orig_str;
      } else {
        cur_str = orig_str.substr(0,temp);
        orig_str = orig_str.substr(temp + 1, orig_str.size() - 1);
      }
    }

    if (cur_str[0] == '#') {
      continue;
    }

    bool not_space = false;
    for (int i = 0; i < cur_str.size(); i++) {
      if (cur_str[i] != ' ' && cur_str[i] != '#') {
        not_space = true;
      }
      if (cur_str[i] == '#') {
        cur_str = cur_str.substr(0,i - 1);
      }
    }

    if (!not_space) {
      continue;
    }

    while (cur_str[0] == ' ') {
      cur_str = cur_str.substr(1,cur_str.size() - 1);
    }

    while (cur_str[cur_str.size() - 1] == ' ') {
      cur_str.pop_back();
    }

    std::string sect_str;

    if (cur_str[0] == '[') {
      cur_sect = &f_section;
      for (int i = 1; i < cur_str.size(); i++) {
        if (cur_str[i] == ' ') {
          valid_parser = false;
          return;
        }
        if (cur_str[i] == '.') {
          sect_str = cur_str.substr(1,i - 1);
          cur_str = cur_str.substr(i + 1, cur_str.size() - 1);
          if (sect_str.size() == 0) {
            valid_parser = false;
            return;
          } else if (cur_sect->sub_sections.find(sect_str) != cur_sect->sub_sections.end()) {
            cur_sect = &cur_sect->sub_sections[sect_str];
          } else {
            Section temp_sect;
            cur_sect->sub_sections.emplace(sect_str,temp_sect);
            cur_sect = &cur_sect->sub_sections[sect_str];
          }
        }
      }

      if (cur_str[0] == '[') {
        cur_str = cur_str.substr(1,cur_str.size() - 1);
      }
      if (cur_str[cur_str.size() - 1] == ']') {
        cur_str.pop_back();
      }

      if (cur_sect->sub_sections.find(cur_str) != cur_sect->sub_sections.end()) {
        cur_sect = &cur_sect->sub_sections[cur_str];
      } else {
        Section temp_sect;
        cur_sect->sub_sections.emplace(cur_str,temp_sect);
        cur_sect = &cur_sect->sub_sections[cur_str];
      }
    } else {
      std::string key;
      std::string val;
      bool is_val = false;
      for (int i = 0; i < cur_str.size(); i++) {
        if (cur_str[i] == '=') {
          if (is_val) {
            valid_parser = false;
            return;
          } else {
            while (key[key.size() - 1] == ' ') {
              key.pop_back();
            }

            if (key.empty()) {
              valid_parser = false;
              return;
            }

            for (int j = 0; j < key.size(); j++) {
              if (!((key[j] >= 'a' && key[j] <= 'z') || (key[j] >= 'A' && key[j] <= 'Z') || (key[j] >= '0' && key[j] <= '9') || key[j] == '-' || key[j] == '_')) {
                valid_parser = false;
                return;
              }
            }
            is_val = true;
          }
        } else if (cur_str[i] == ' ') {
          if (is_val) {
            if (val.empty()) {
              continue;
            } else {
              val += ' ';
            }
          } else if (key.empty()) {
            continue;
          } else {
            key += ' ';
          }
        } else if (is_val) {
          val += cur_str[i];
        } else {
          key += cur_str[i];
        }
      }

      while (key[key.size() - 1] == ' ') {
        key.pop_back();
      }

      if (key.empty()) {
        valid_parser = false;
        return;
      }

      for (int i = 0; i < key.size(); i++) {
        if (key[i] == ' ') {
          valid_parser = false;
          return;
        }
      }

      Value cur_val;

      if (cur_val.ParseValue(val)) {
        valid_parser = false;
        return;
      } else {
        if (cur_sect->values.find(key) == cur_sect->values.end()) {
          cur_sect->values.emplace(key, cur_val);
        } else {
          valid_parser = false;
          return;
        }
      }
    }
  }
}

OMFLParser parse(const std::filesystem::path& path) {
  OMFLParser parser;
  std::string input_str;
  parser.ParseFile(path, input_str);

  return parser;
}

OMFLParser parse(const std::string& str) {
  OMFLParser parser;

  if (!str.empty()) {
    parser.ParseFile("",str);
  } else {

  }

  return parser;
}

bool OMFLParser::valid() const {
  return valid_parser;
}

Value& OMFLParser::Get(const std::string& key) {
  std::string str;
  Section* cur_sect = &f_section;
  for (int i = 0; i < key.size(); i++) {
    if (key[i] == '.') {
      if (cur_sect->sub_sections.find(str) != cur_sect->sub_sections.end()) {
        cur_sect = &cur_sect->sub_sections[str];
        str.erase();
      } else {
        std::invalid_argument("Error: Invalid Argument");
      }
    } else {
      str += key[i];
    }
  }

  if (cur_sect->values.find(str) != cur_sect->values.end()) {
    return cur_sect->values[str];
  } else {
    std::invalid_argument("Error: Invalid Argument");
  }
}