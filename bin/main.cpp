#include "../lib/weather.cpp"

int main(int argc, char** argv) {

  std::filesystem::path config_path = argv[1];
  std::filesystem::path api_key_path = argv[2];

  Weather(config_path, api_key_path);

  return 0;
}
