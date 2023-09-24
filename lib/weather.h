#include <iostream>

class HourInfo {
 public:
  int code;
  int temp;
  int ap_temp;
  int prec_prob;
  double prec;
  int vis;
  int wind_dir;
  int wind_speed_min;
  int wind_speed_max;

  std::wstring DisplayCode();
  std::wstring DisplayImage(uint8_t);
  std::wstring DisplayTemp();
  std::wstring DisplayWind();
  std::wstring DisplayVision();
  std::wstring DisplayPrec();
};

class DayInfo {
 public:
  char d_of_week;
  int day;
  int month;
  HourInfo hours[4];

  std::wstring DayWeek();
  std::wstring DayName();
  void DisplayDay(std::wostream& ostream);
};

class WeatherInfo {
 public:
  std::string city_name;
  int num_d = 3;
  std::vector<DayInfo> days;

  friend std::wostream& operator<<(std::wostream& ostream, WeatherInfo city);
};