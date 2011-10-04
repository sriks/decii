#ifndef WEATHERFACTORY_H
#define WEATHERFACTORY_H

class LocationSuggestions;
class WeatherDataHandler;
class WeatherFactory
{
public:
    static LocationSuggestions* suggestionProvider();
    static WeatherDataHandler* weatherEngine();
};

#endif // WEATHERFACTORY_H
