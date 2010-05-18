Statemachine framework is newly added to Qt 4.6 and simplifies application logic and flow. In this example of horoscope widget, all the logical steps (activities) are made into states and transitions are added for logic to flow through.

The following simple activities/states are modeled using statemachine

-	Getting saved settings
-	Initiating and handing network request
-	Parsing resultant xml 
-	Display content

Hence the entire app is a single finite statemachine running through the above mentioned states. Also to make things simple entire UI is displayed by dynamically filling html source template (like webOS) and populating on a QGraphicsWebView contained in a graphics view.

As going through the hardships of getting it running on Symbian device, I came through an interesting (for some ppl - horrible) bug in QHttp, QNetworkAccessManger classes for Symbian port. 
http://bugreports.qt.nokia.com/browse/QTBUG-8687. Quickly found a solution at http://api.witinside.net/mobile/qt/symbian-s60/HostNotFoundError.php. In short all http requests will return “Host Not Found” error (fails mostly at host resolver ) all across Qt for Symbian (device only). Interesting isn’t it?  Hence don’t panic if you come across and just apply the above mentioned fix. Issue will be addressed by Qt 4.6.3 or Qt 4.7

Try sources with your svn at http://decii.googlecode.com/svn/trunk/
