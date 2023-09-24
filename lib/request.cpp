#include "request.h"

int Req::ReqLoc() {
  cpr::Response r = cpr::Get(cpr::Url{"https://api.api-ninjas.com/v1/city"},
                             cpr::Header{{"X-Api-Key", "Dq+UmRTnmmQ+43FJIhYDWA==3TwImS3NkQL3Cunq"}},
                             cpr::Parameters{{"name", city}});

  if (r.text == "[]") {
    return 1;
  }

  bool flag_lat = false;
  bool flag_lon = false;
  for (int i = 7; i < r.text.size(); i++) {
    if (!flag_lat && !flag_lon && r.text[i - 7] == 'l' && r.text[i - 6] == 'a' && r.text[i - 5] == 't' && r.text[i - 4] == 'i' && r.text[i - 3] == 't' && r.text[i - 2] == 'u' && r.text[i - 1] == 'd' && r.text[i] == 'e') {
      i += 4;
      flag_lat = true;
    }

    if (flag_lat) {
      if (r.text[i] == ',') {
        i += 14;
        flag_lon = true;
        flag_lat = false;
        continue;
      }
      lat += r.text[i];
      continue;
    }

    if (flag_lon) {
      if (r.text[i] == ',') {
        break;
      }
      lon += r.text[i];
    }
  }

  return 0;
}

void Req::ReqWeather(nlohmann::json* data, uint8_t num_days) {
  std::string hour_param = "temperature_2m,apparent_temperature,precipitation_probability,precipitation,weathercode,visibility,windspeed_10m,winddirection_10m";
  cpr::Response r = cpr::Get(cpr::Url{"https://api.open-meteo.com/v1/forecast"},
                             cpr::Parameters{{"latitude", lat}, {"longitude", lon},
                                             {"hourly", hour_param}, {"windspeed_unit", "ms"},
                                             {"forecast_days", std::to_string(num_days)}});

  *data = nlohmann::json::parse(r.text);

  return;
}
