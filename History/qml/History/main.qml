import QtQuick 1.0

Rectangle {
     id: historyView;
     width: 360;
//     height: 2000;
     color: "#00000000"

    VisualItemModel {
        id: itemModel
        Text {
             id: title
             text: "THIS DAY IN HISTORY"
             smooth: true
             font.pointSize: 20
             wrapMode: Text.WordWrap
             width: historyView.width
             color: "white"
        }

        Item {
            Text {
                id: eventTitle
                text:  "Election results out for multiple states in India."
                font.pointSize: 9
                font.bold: true
                wrapMode: Text.WordWrap
                smooth: true
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                opacity: 1
                color: "white"
                width: 360
            }

            Text {
                id: eventDescription
                width: 340
                x: 10
                y: eventTitle.height + 10
                text:  "Things are a bit vague with LG -- no specific resolutions are mentioned in the pre-show announcement, but we're told that the company will introduce a full line-up of ultra-high resolution Advanced High Performance In-Plane Switching (AH-IPS) products, including 3.5-, 4.5-, 7-, 9.7, 55-, and 84-inch panels, with a greater number of pixels than the PPI that can be recognized by the human eye at a typical distance -- a proclamation typically reserved for the iPhone 4's 326ppi Retina Display. Of course, LG could be misleading here -- the 9.7-inch panel brought up in the press release could just be the exact same 1,024 x 768 IPS display on the iPad, but we'd be surprised if LG doesn't have a similarly-sized prototype to fire back at Samsung's 10.1-inch 300ppi panel. Well, keep an eye out for our SID 2011 coverage next week and we'll let you know what goodies we find.Things are a bit vague with LG -- no specific resolutions are mentioned in the pre-show announcement, but we're told that the company will introduce a full line-up of ultra-high resolution Advanced High Performance In-Plane Switching (AH-IPS) products, including 3.5-, 4.5-, 7-, 9.7, 55-, and 84-inch panels, with a greater number of pixels than the PPI that can be recognized by the human eye at a typical distance -- a proclamation typically reserved for the iPhone 4's 326ppi Retina Display. Of course, LG could be misleading here -- the 9.7-inch panel brought up in the press release could just be the exact same 1,024 x 768 IPS display on the iPad, but we'd be surprised if LG doesn't have a similarly-sized prototype to fire back at Samsung's 10.1-inch 300ppi panel. Well, keep an eye out for our SID 2011 coverage next week and we'll let you know what goodies we find.Things are a bit vague with LG -- no specific resolutions are mentioned in the pre-show announcement, but we're told that the company will introduce a full line-up of ultra-high resolution Advanced High Performance In-Plane Switching (AH-IPS) products, including 3.5-, 4.5-, 7-, 9.7, 55-, and 84-inch panels, with a greater number of pixels than the PPI that can be recognized by the human eye at a typical distance -- a proclamation typically reserved for the iPhone 4's 326ppi Retina Display. Of course, LG could be misleading here -- the 9.7-inch panel brought up in the press release could just be the exact same 1,024 x 768 IPS display on the iPad, but we'd be surprised if LG doesn't have a similarly-sized prototype to fire back at Samsung's 10.1-inch 300ppi panel. Well, keep an eye out for our SID 2011 coverage next week and we'll let you know what goodies we find."
                font.pointSize: 6.8;
                smooth: true
                horizontalAlignment: Text.AlignLeft
                color: "white"
                wrapMode: Text.WordWrap
                opacity: 1

                Component.onCompleted: {console.log('test h:'+eventDescription.height);}
               }

            Rectangle {
                id: contentBackground
                y: eventTitle.y-15;
                width: historyView.width;
                height: eventTitle.height+eventDescription.height+40 // fix magic numbers
                radius: 10
                opacity: 0.3
                color: "white"
            }

        height: title.height + eventTitle.height + eventDescription.height + 10;
        }

    }

    ListView {
        id: contentListView
        orientation: ListView.Vertical
        anchors.fill: parent
        model: itemModel
        height: historyView.height
        contentHeight: historyView.height
        spacing: 20
    }

    BorderImage {
        id: border_image1
        anchors.fill: parent
        source: "qrc:/images/bkg.png"
        z: -1
    }


    //Floating menu bar
    Rectangle {
        width: 360;
        height: 80
        color: "green"
        opacity: 0.5
        x:0;y:640-80;

    }

    Component.onCompleted: {
        console.log("h:"+historyView.height)
        console.log("lv h:"+contentListView.height)
    }

}

