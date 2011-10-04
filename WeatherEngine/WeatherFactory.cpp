#include "WeatherFactory.h"
#ifdef WEATHERSERVICE_WEATHERBUG
#include "WeatherbugInfo.h"
#endif

// TODO: This is a dummy factory impl. Move to a proper impl.
LocationSuggestions * WeatherFactory::suggestionProvider() {
    LocationSuggestions* sp = NULL;
    if(!sp)
#ifdef WEATHERSERVICE_WEATHERBUG
        sp = new WeatherBugLocationSuggestions; // ownership is transferred to caller.
#endif
    return sp;
}

WeatherDataHandler * WeatherFactory::weatherEngine() {
    WeatherInfo* w = NULL;
#ifdef WEATHERSERVICE_WEATHERBUG
    w = new WeatherBugInfo;
#endif
    return w;
}

// eof
