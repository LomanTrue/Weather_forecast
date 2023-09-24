#pragma once

#include <filesystem>
#include <istream>
#include <map>
#include <variant>
#include <vector>

class Value {
 public:
  char type;
  std::variant<int, float, std::string, bool> val;
  std::vector<Value> val_array;
  int ParseValue(std::string& str);

  bool IsInt() const;
  bool IsFloat() const;
  bool IsString() const;
  bool IsArray() const;
  bool IsBool() const;

  int AsInt() const;
  int AsIntOrDefault(int value) const;
  float AsFloat() const;
  float AsFloatOrDefault(float value) const;
  std::string AsString() const;
  std::string AsStringOrDefault(std::string value) const;
  bool AsBool() const;

  Value& Get(const std::string& key);

  Value operator[](int i) const;
};

class Section {
 public:
  std::map<std::string, Value> values;
  std::map<std::string, Section> sub_sections;
};

class OMFLParser {
  bool valid_parser = true;
  Section f_section; //fst (global) section

 public:
  bool valid() const;
  Value& Get(const std::string& key);
  void ParseFile(const std::filesystem::path& path, const std::string& input_str);
};

OMFLParser parse(const std::filesystem::path& path);
OMFLParser parse(const std::string& str);