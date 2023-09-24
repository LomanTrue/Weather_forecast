#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <iostream>

class Req {
 private:
  std::string city;

 public:
  std::string lat;
  std::string lon;

  Req(std::string _city) {
    city = _city;
  }

  int ReqLoc();
  void ReqWeather(nlohmann::json* data, uint8_t num_days);
};
