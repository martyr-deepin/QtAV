import QtQuick 2.0

Rectangle {
    id: root
    property int itemHeight: 32
    property string selected: ""
    color: "#80101010"

    signal clicked

    QtObject {
        id: d
        property Item selectedItem
        property string detail: ""
    }

    ListModel {
        id: itemModel
        ListElement { name: "Video codec" }
        ListElement { name: "Color" }
        ListElement { name: "Aspect ratio" }
        ListElement { name: "Audio channels" }
        ListElement { name: "Audio track" }
        ListElement { name: "Speed" }
        ListElement { name: "Repeat" }
    }

    Component {
        id: itemDelegate
        Rectangle {
            id: delegateItem
            anchors.horizontalCenter: parent.horizontalCenter
            width: root.width - 2
            height: itemHeight
            color: "#99000000"

            Text {
                text: name
                color: "white"
                font.pixelSize: 16
                anchors.centerIn: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    //ListView.currentItem = parent
                    if (d.selectedItem)
                        d.selectedItem.state = "baseState"
                    d.selectedItem = delegateItem
                    d.selectedItem.state = "selected"
                    selected = name
                    if (typeof escription != "undefined")
                        d.detail = description
                    root.clicked()
                }
            }
            states: [
                State {
                    name: "selected"
                    PropertyChanges {
                        target: delegateItem
                        color: "#20ffffff"
                    }
                }
            ]
            Component.onCompleted: {

            }
            transitions: [
                Transition {
                    from: "*"
                    to: "*"
                    ColorAnimation {
                        properties: "color"
                        easing.type: Easing.OutQuart
                        duration: 500
                    }
                }
            ]
        }
    }

    ListView {
        anchors.fill: parent
        anchors.topMargin: 40
        spacing: 1
        focus: true

        delegate: itemDelegate
        model: itemModel
    }
}
