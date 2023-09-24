# Weather forecast

This is a console programm that displays weather forecast

Project uses external libraries such as:
- cpr (https://github.com/libcpr/cpr)
- json (https://github.com/nlohmann/json)

Also project use an API for weather forecast.
Used websites:
- open-meteo (https://open-meteo.com/en/docs#latitude=59.94&longitude=30.31&hourly=temperature_2m&forecast_days=16)
- API-ninja (https://api-ninjas.com/api/city)

# Configuration

'config.txt' has initial data, namely cities (or city) and the number of days in the forecast. This file is OMFL file

While the program is running, you can:
- switch cities using a key 'p' (previous city) and 'n' (next city)
- increase and decrease the number of days in the forecast using a key '+' and '-'. Max - 16 days, min - 1 day
- stop the programm using a key 'Esc'
