add_library(request request.cpp request.h)
add_library(parser OMFLparser.cpp OMFLparser.h)
add_library(weather weather.cpp weather.h)

target_link_libraries(request PUBLIC cpr::cpr)
target_link_libraries(request PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(weather PUBLIC nlohmann_json::nlohmann_json)