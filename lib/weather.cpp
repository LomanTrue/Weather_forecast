#include <vector>
#include <windows.h>
#include <codecvt>
#include <fcntl.h>
#include <cstdlib>

#include "OMFLparser.cpp"
#include "request.cpp"
#include "weather.h"

std::wstring HourInfo::DisplayCode() {
  std::wstring res;
  if (code == 0) {
    res = L"Солнечно     ";
  } else if (code == 1) {
    res = L"Ясно         ";
  } else if (code == 2) {
    res = L"Перем. облачн";
  } else if (code == 3) {
    res = L"Облачно      ";
  } else if (code == 45) {
    res = L"Дымка        ";
  } else if (code == 61) {
    res = L"Небол. дождь ";
  } else if (code == 63) {
    res = L"Дождь        ";
  } else if (code == 65) {
    res = L"Сильный дождь";
  } else if (code == 80) {
    res = L"Слабый ливень";
  } else if (code == 81) {
    res = L"Ливень       ";
  } else if (code == 82) {
    res = L"Сил. ливень  ";
  } else if (code == 85) {
    res = L"Снег         ";
  } else if (code == 86) {
    res = L"Сильный снег ";
  } else if (code == 95) {
    res = L"Гроза        ";
  }

  return res;
}

std::wstring HourInfo::DisplayImage(uint8_t i) {
  std::wstring res;
  if (code == 0 || code == 1) {
    std::wstring image[5] = {L"     \\   /     "
                            ,L"      .-.      "
                            ,L"   ─ (   ) ─   "
                            ,L"      `-'      "
                            ,L"     /   \\     "};
    return image[i];
  } else if (code == 2) {
    std::wstring image[5] = {L"    \\  /       "
                            ,L"  _ /\"\".-.     "
                            ,L"    \\_(   ).   "
                            ,L"    /(___(__)  "
                            ,L"               "};
    return image[i];
  } else if (code == 3) {
    std::wstring image[5] = {L"               "
                            ,L"      .--.     "
                            ,L"   .-(    ).   "
                            ,L"  (___.__)__)  "
                            ,L"               "};
    return image[i];
  } else if (code == 45) {
    std::wstring image[5] = {L"               "
                            ,L"  _ - _ - _ -  "
                            ,L"   _ - _ - _   "
                            ,L"  _ - _ - _ -  "
                            ,L"               "};
    return image[i];
  } else if (code == 61 || code == 63 || code == 65 || code == 80 || code == 81 || code == 82) {
    std::wstring image[5] = {L"      .-.      "
                            ,L"    (    ).    "
                            ,L"   (___(__)    "
                            ,L"    / / / /    "
                            ,L"  / / / /      "};
    return image[i];
  } else if (code == 85 || code == 86) {
    std::wstring image[5] = {L"      .-.      "
                            ,L"    (    ).    "
                            ,L"   (___(__)    "
                            ,L"   * * * *     "
                            ,L"  * * * *      "};
    return image[i];
  } else if (code == 95 || code == 96 || code == 99) {
    std::wstring image[5] = {L"      .-.      "
                            ,L"    (    ).    "
                            ,L"   (___(__)    "
                            ,L"     /__       "
                            ,L"      /        "};
    return image[i];
  }
}

std::wstring HourInfo::DisplayTemp() {
  std::wstring res = std::to_wstring(temp);
  if (temp != ap_temp) {
    res += L'(';
    res += std::to_wstring(ap_temp);
    res += L')';
  }
  res += L" °C";

  for (int i = res.size(); i < 13; i++) {
    res += L' ';
  }

  return res;
}

std::wstring HourInfo::DisplayWind() {
  std::wstring res;
  if (wind_dir == 1) {
    res += L'↑';
  } else if (wind_dir == 2) {
    res += L'↗';
  } else if (wind_dir == 3) {
    res += L'→';
  } else if (wind_dir == 4) {
    res += L'↘';
  } else if (wind_dir == 5) {
    res += L'↓';
  } else if (wind_dir == 6) {
    res += L'↙';
  } else if (wind_dir == 7) {
    res += L'←';
  } else if (wind_dir == 8) {
    res += L'↖';
  }

  res += L' ';
  res += std::to_wstring(wind_speed_min);
  if (wind_speed_min != wind_speed_max) {
    res += L'-';
    res += std::to_wstring(wind_speed_max);
  }
  res += L" км/ч";

  for (int i = res.size(); i < 13; i++) {
    res += L' ';
  }

  return res;
}

std::wstring HourInfo::DisplayVision() {
  std::wstring res = std::to_wstring(vis);
  res += L" км";

  for (int i = res.size(); i < 13; i++) {
    res += L' ';
  }

  return res;
}

std::wstring HourInfo::DisplayPrec() {
  std::wstring res;
  std::wstring temp = std::to_wstring(prec);
  res.push_back(temp[0]);
  res.push_back(temp[1]);
  res.push_back(temp[2]);
  res += L" мм | ";
  res += std::to_wstring(prec_prob);
  res += L'%';

  for (int i = res.size(); i < 13; i++) {
    res += L' ';
  }

  return res;
}

std::wstring DayInfo::DayWeek() {
  std::wstring res;
  if (d_of_week == 1) {
    res = L"Пн.";
  } else if (d_of_week == 2) {
    res = L"Вт.";
  } else if (d_of_week == 3) {
    res = L"Ср.";
  } else if (d_of_week == 4) {
    res = L"Чт.";
  } else if (d_of_week == 5) {
    res = L"Пт.";
  } else if (d_of_week == 6) {
    res = L"Сб.";
  } else if (d_of_week == 7) {
    res = L"Вс.";
  }

  return res;
}

std::wstring DayInfo::DayName() {
  std::wstring res = DayWeek();
  res += L' ';
  if (day < 10) {
    res += L'0';
  }
  res += std::to_wstring(day);
  res += L' ';
  if (month == 1) {
    res += L"Янв.";
  } else if (month == 2) {
    res += L"Фев.";
  } else if (month == 3) {
    res += L"Мар.";
  } else if (month == 4) {
    res += L"Апр.";
  } else if (month == 5) {
    res += L"Май.";
  } else if (month == 6) {
    res += L"Июн.";
  } else if (month == 7) {
    res += L"Июл.";
  } else if (month == 8) {
    res += L"Авг.";
  } else if (month == 9) {
    res += L"Сен.";
  } else if (month == 10) {
    res += L"Окт.";
  } else if (month == 11) {
    res += L"Ноя.";
  } else if (month == 12) {
    res += L"Дек.";
  }

  return res;
}

void DayInfo::DisplayDay(std::wostream& ostream) {
  ostream << L"                                                    ┌───────────┐" << L'\n';
  ostream << L"┌────────────────────────────┬──────────────────────┤" << DayName() << L"├──────────────────────┬────────────────────────────┐" << L'\n';
  ostream << L"│            Утро            │            День      └─────┬─────┘      Вечер           │            Ночь            │" << L'\n';
  ostream << L"├────────────────────────────┼────────────────────────────┼────────────────────────────┼────────────────────────────┤" << L'\n';

  for (int i = 0; i <= 3; i++) {
    ostream << L'│' << hours[i].DisplayImage(0) << hours[i].DisplayCode();
  }
  ostream << L'│' << L'\n';

  for (int i = 0; i <= 3; i++) {
    ostream << L'│' << hours[i].DisplayImage(1) << hours[i].DisplayTemp();
  }
  ostream << L'│' << L'\n';

  for (int i = 0; i <= 3; i++) {
    ostream << L'│' << hours[i].DisplayImage(2) << hours[i].DisplayWind();
  }
  ostream << L'│' << L'\n';

  for (int i = 0; i <= 3; i++) {
    ostream << L'│' << hours[i].DisplayImage(3) << hours[i].DisplayVision();
  }
  ostream << L'│' << L'\n';

  for (int i = 0; i <= 3; i++) {
    ostream << L'│' << hours[i].DisplayImage(4) << hours[i].DisplayPrec();
  }
  ostream << L'│' << L'\n';

  ostream << L"└────────────────────────────┴────────────────────────────┴────────────────────────────┴────────────────────────────┘" << L'\n';

  ostream << L'\n';
}

std::wostream& operator<<(std::wostream& ostream, WeatherInfo city) {
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
  ostream << L"Прогноз погоды: " << converter.from_bytes(city.city_name) << L"\n";

  /*
  if (city.response_.status_code != kSuccessCode) {
    ostream << L"Error\n";
    ostream << converter.from_bytes(city.response_.reason) << std::endl;
    return ostream;
  }
  */

  for (int i = 0; i < city.num_d; i++) {
    city.days[i].DisplayDay(ostream);
  }

  return ostream;
}

int JsonParse(nlohmann::json* data, WeatherInfo* city) {
  int cur_i = 2;
  for (int i = 0; i < city->num_d; i++) {
    DayInfo day_info;

    std::string temp;
    try {
      temp = (*data)["hourly"]["time"][i * 24];
    } catch (const std::exception& e) {
      return 1;
    }
    std::string num;
    num += temp[8];
    num += temp[9];
    day_info.day = std::stoi(num);

    num.clear();
    num += temp[5];
    num += temp[6];
    day_info.month = std::stoi(num);
    day_info.month = 2;

    num.clear();
    num += temp[0];
    num += temp[1];
    uint8_t first_half_year = std::stoi(num);
    uint8_t a;
    if (first_half_year % 4 == 0) {
      a = 6;
    } else if (first_half_year % 4 == 1) {
      a = 4;
    } else if (first_half_year % 4 == 2) {
      a = 2;
    } else if (first_half_year % 4 == 3) {
      a = 0;
    }
    num.clear();
    num += temp[2];
    num += temp[3];
    uint8_t second_half_year = std::stoi(num);
    int year = first_half_year * 100 + second_half_year;
    uint8_t code_year = 6 + (second_half_year % 7) + (second_half_year / 4) % 7;
    uint8_t code_month;
    if (day_info.month == 1 || day_info.month == 10) {
      code_month = 1;
    } else if (day_info.month == 5) {
      code_month = 2;
    } else if (day_info.month == 8) {
      code_month = 3;
    } else if (day_info.month == 2 || day_info.month == 3 || day_info.month == 11) {
      code_month = 4;
    } else if (day_info.month == 6) {
      code_month = 5;
    } else if (day_info.month == 12 || day_info.month == 9) {
      code_month = 6;
    } else if (day_info.month == 4 || day_info.month == 7) {
      code_month = 0;
    }
    uint8_t result = day_info.day + code_month + code_year;
    if ((year % 400 == 0 || (year % 100 != 0 && year % 4 == 0)) && day_info.month <= 2) {
      result -= 1;
    }
    day_info.d_of_week = (result + 5) % 7 + 1;

    HourInfo hour_info;
    for (int j = 0; j <= 3; j++) {
      float temp_f = (*data)["hourly"]["temperature_2m"][cur_i];
      hour_info.temp = std::round(temp_f);

      float temp_af = (*data)["hourly"]["apparent_temperature"][cur_i];
      hour_info.ap_temp = std::round(temp_af);

      int temp_pp = (*data)["hourly"]["precipitation_probability"][cur_i];
      hour_info.prec_prob = temp_pp;

      hour_info.prec = (*data)["hourly"]["precipitation"][cur_i];

      int temp_code = (*data)["hourly"]["weathercode"][cur_i];
      hour_info.code = temp_code;

      float temp_vis = (*data)["hourly"]["visibility"][cur_i];
      hour_info.vis = std::round(temp_vis) / 1000;

      double speed_min = 1000;
      double speed_max = 0;
      double temp_speed;
      for (int k = cur_i - 1; k <= cur_i + 1; k++) {
        temp_speed = (*data)["hourly"]["windspeed_10m"][k];
        if (temp_speed < speed_min) {
          speed_min = temp_speed;
        }
        if (temp_speed > speed_max) {
          speed_max = temp_speed;
        }
      }
      hour_info.wind_speed_min = std::round(speed_min);
      hour_info.wind_speed_max = std::round(speed_max);

      int temp_w_dir = (*data)["hourly"]["winddirection_10m"][cur_i];
      if (temp_w_dir <= 22) {
        hour_info.wind_dir = 1;
      } else if (temp_w_dir <= 67) {
        hour_info.wind_dir = 2;
      } else if (temp_w_dir <= 112) {
        hour_info.wind_dir = 3;
      } else if (temp_w_dir <= 157) {
        hour_info.wind_dir = 4;
      } else if (temp_w_dir <= 202) {
        hour_info.wind_dir = 5;
      } else if (temp_w_dir <= 247) {
        hour_info.wind_dir = 6;
      } else if (temp_w_dir <= 292) {
        hour_info.wind_dir = 7;
      } else if (temp_w_dir <= 337) {
        hour_info.wind_dir = 8;
      } else {
        hour_info.wind_dir = 1;
      }

      day_info.hours[j] = hour_info;
      cur_i += 6;
    }

    if (city->days.size() < i + 1) {
      city->days.push_back(day_info);
    } else {
      city->days[i] = day_info;
    }
  }

  return 0;
}

int Weather(std::filesystem::path path) {
  OMFLParser parser = parse(path);
  WeatherInfo cur_city;
  int cur_i = 0;

  cur_city.num_d = parser.Get("day").AsInt();
  bool is_array = parser.Get("city").IsArray();
  std::vector<std::string> arr_city;
  if (is_array) {
    for (int i = 0; i < parser.Get("city").val_array.size(); i++) {
      arr_city.push_back(parser.Get("city").val_array[i].AsString());
    }
  } else {
    arr_city.push_back(parser.Get("city").AsString());
  }

  bool is_esc = false;
  nlohmann::json data;
  while (true) {
    Req req_city(arr_city[cur_i]);
    if (!req_city.ReqLoc()) {
      req_city.ReqWeather(&data, cur_city.num_d);
    } else {
      std::cerr << "Error" << '\n';
    }

    cur_city.city_name = arr_city[cur_i];
    if (!JsonParse(&data, &cur_city)) {
      system("cls");
      _setmode(_fileno(stdout), _O_U8TEXT);
      std::wcout << cur_city;
    } else {
      std::cerr << "Error" << '\n';
    }

    while (true) {
      if (GetAsyncKeyState(78) == -32767) { //Pressed "n"
        if (cur_i == 0) {
          cur_i = arr_city.size() - 1;
        } else {
          cur_i--;
        }
        if (arr_city.size() != 1) {
          break;
        }
      } else if (GetAsyncKeyState(80) == -32767) { //Pressed "p"
        cur_i = (cur_i + 1) % arr_city.size();
        if (arr_city.size() != 1) {
          break;
        }
      } else if (GetAsyncKeyState(27) == -32767) { //Pressed "Esc"
        is_esc = true;
        break;
      } else if (GetAsyncKeyState(187) == -32767) { //Pressed "+"
        if (cur_city.num_d < 16) {
          cur_city.num_d += 1;
          break;
        }
      } else if (GetAsyncKeyState(189) == -32767) { //Pressed "-"
        if (cur_city.num_d != 1) {
          cur_city.num_d -= 1;
          break;
        }
      }
    }

    if (is_esc) {
      break;
    }
  }

  return 0;
}
