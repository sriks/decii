WeatherWidget2

The main focus of this app is to exercise on Qt Graphics View Framework. Hence the entire app is done using only Graphics View , QGraphicsWidget instead QWidget - not a single paintEvent is used! Though the main focus is on Graphics View, I got a chance to exercise few more features like

All new Anchor layout system (introduced in 4.6) 
Animation Framework (introduced in 4.6) 
QSettings for storing settings persistently 
Transparent backgrounds, drawing outside widget area and many more 
Coming to its features

Communicates with WeatherBug to get weather info. 
Fetaches 4 day forecast 
Displays current condition as icons 
Supports switching of temperature units 
Maintains local time of the selected location 
Integrates location to google maps 
Coming to UI, took the freedom to try a clean interface. For example if you want to change the location just click/tap on the location or,  to refresh weather info just click/tap on current temperature - HTC Andriod widget dont have this feature yet ;) . So the UI is free of boring update buttons. 

One more thing to mention is self closing dialogs. These are information dialogs that dont have OK or Cancel buttons, but closes themselves if you click/tap at any point on the dialog. These are not standard dialogs but customized QGraphicsWidget.

The app is far from completion with minor bugs and occasional crashes , you can drive into the code and contribute if you feel. 

Note: This is a exercise project to practice Qt Graphics View. Hence code is very meshy and bad.
Requires Qt 4.6 or above