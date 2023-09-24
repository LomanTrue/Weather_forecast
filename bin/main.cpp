#include "../lib/weather.cpp"

int main(int argc, char** argv) {

  std::filesystem::path path = argv[1];

  Weather(path);

  return 0;
}
