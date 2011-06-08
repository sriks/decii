import QtQuick 1.0

Rectangle{
    id:parentRect;
    width: 600
    height:700
    color:"red"
    Rectangle{
        id:button;
        width: 100
        height: 40
        color: "green"
        Text {
            id: buttontext
            text: "click me"
        }

       MouseArea {
           anchors.fill: parent
           onClicked: {

               label.text="welcome pandi"
           }

       }
    }

    Text {
        id: label
        y:200
        x:30
        text: "welcome"
    }

}
