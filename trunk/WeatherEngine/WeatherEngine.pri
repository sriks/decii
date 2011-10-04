INCLUDEPATH +=  $$PWD
DEPENDPATH += $$PWD
HEADERS += $$PWD/WeatherInfo.h \
    ../WeatherEngine/WeatherForecastInfo.h \
    ../WeatherEngine/WeatherbugInfo.h \
    ../WeatherEngine/NetworkEngine.h \
    ../WeatherEngine/XQueryEngine.h \
    ../WeatherEngine/WeatherDataHandler.h \
    ../WeatherEngine/Constants.h \
    ../WeatherEngine/WeatherManager.h \
    ../WeatherEngine/LocationSuggestions.h \
    ../WeatherEngine/WeatherFactory.h
SOURCES += $$PWD/WeatherInfo.cpp \
    ../WeatherEngine/WeatherForecastInfo.cpp \
    ../WeatherEngine/WeatherbugInfo.cpp \
    ../WeatherEngine/NetworkEngine.cpp \
    ../WeatherEngine/XQueryEngine.cpp \
    ../WeatherEngine/WeatherDataHandler.cpp \
    ../WeatherEngine/WeatherManager.cpp \
    ../WeatherEngine/LocationSuggestions.cpp \
    ../WeatherEngine/WeatherFactory.cpp

QT += network xmlpatterns
# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices






