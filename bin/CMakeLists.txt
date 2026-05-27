add_executable(weather_forecast main.cpp)

target_link_libraries(weather_forecast request)
target_include_directories(weather_forecast PRIVATE ${PROJECT_SOURCE_DIR})
