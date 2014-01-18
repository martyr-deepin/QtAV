import QtQuick 2.0

Rectangle {
    color: "#aa000000"
    property alias title: title.text
    property string codecName: "FFmpeg"

    QtObject {
        id: d
        property Item selectedItem
        property string detail: ""
    }
    Text {
        id: title
        anchors.top: parent.top
        width: parent.width
        height: 40
        color: "white"
        font.pixelSize: 16
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
    Canvas {
        anchors.fill: parent
        onPaint: {
            var ctx = getContext('2d')
            var g = ctx.createLinearGradient(0, 0, parent.width, 0)
            g.addColorStop(0, "#00ffffff")
            g.addColorStop(0.3, "#f0ffffff")
            g.addColorStop(0.7, "#f0ffffff")
            g.addColorStop(1, "#00ffffff")
            ctx.fillStyle = g
            ctx.fillRect(0, title.height, parent.width, 1)
        }
    }
    Text {
        id: detail
        anchors.top: title.bottom
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 60
        width: parent.width
        text: d.detail
        color: "white"
        font.pixelSize: 18
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    ListView {
        anchors {
            top: detail.bottom
            bottom: parent.bottom
            topMargin: 10
            horizontalCenter: parent.horizontalCenter
        }
        onContentWidthChanged: {
            anchors.leftMargin = Math.max(10, (parent.width - contentWidth)/2)
            anchors.rightMargin = anchors.leftMargin
            width = parent.width - 2*anchors.leftMargin
        }
        orientation: ListView.Horizontal
        spacing: 6
        focus: true

        delegate: contentDelegate
        model: contentModel
    }

    ListModel {
        id: contentModel
        ListElement { name: "DXVA"; description: "DXVA Hardware" }
        ListElement { name: "VAAPI"; description: "VAAPI Hardware" }
        ListElement { name: "FFmpeg"; description: "FFmpeg Software" }
    }

    Component {
        id: contentDelegate
        Rectangle {
            id: delegateItem
            width: 70
            height: 40
            //opacity: 0.8
            color: "#aa000000"
            //color: ListView.isCurrentItem ? "blue" : "black"
            border.color: "white"
            Text {
                text: name
                color: "white"
                font.pixelSize: 16
                anchors.centerIn: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                onContentSizeChanged: {
                    parent.width = contentWidth + 8
                    parent.height = contentHeight + 8
                }
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    //ListView.currentItem = parent
                    if (d.selectedItem)
                        d.selectedItem.state = "baseState"
                    d.selectedItem = delegateItem
                    d.selectedItem.state = "selected"
                    d.detail = description
                }
            }
            states: [
                State {
                    name: "selected"
                    PropertyChanges {
                        target: delegateItem
                        color: "#990000ff"
                    }
                }
            ]
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
}
