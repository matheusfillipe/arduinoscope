import QtQuick 2.11
import QtQuick.Controls 2.2
import QtCharts 2.2
import io.qt.arduinoscope.serialport 1.0


ApplicationWindow {
    visible: true
    width: 1024
    height: 640
    color: "#23262a"
    title: qsTr("ArduinOscope")

    Rectangle{

    id: mainRec
    focus: true
    anchors.fill: parent
    border.width: 0
    color: "#23262a"
    border.color: "#23262a"

    property bool paused: false

    signal pause()

    Keys.onPressed: {

        if (event.key == Qt.Key_Space ){
            pause()
            event.accepted = true
        }
    }

    onPause: {
        mainRec.paused = ! mainRec.paused
    }


    SerialPort {
        id: backend
        property real x: 0

        onMessageReceived: {

            if(!mainRec.paused ){

            line.append(x,Number(message))
            x+=0.1
            axisX.max=x
            axisX.min=x-5
            }
        }
    }


    ChartView {
        id: chart
        title: backend.status
        anchors.fill: parent
        antialiasing: true
        theme: ChartView.ChartThemeDark

        ValueAxis {
            id: axisX
            min: 0
            max: 5+backend.x
            tickCount: 6

        }


        ValueAxis {
            id: axisY
            min: -5.0
            max: 5.0
            tickCount: 11
            minorTickCount: 1
        }

        LineSeries {
            id: line
            name: backend.message + " V"

            axisX: axisX
            axisY: axisY




        }

        TextField {
            id: textField
            placeholderText: "Usb device Name"

            ToolTip{
                id: toolTip
                text: "Enter the arduino device usb file path or name"
                parent: textField
            }

            onHoveredChanged: {
                toolTip.visible = !toolTip.visible
            }

            onAccepted:  {
                backend.device = textField.text
                textField.focus = false
                mainRec.focus = true
            }



           text: backend.device
            anchors.top: parent.top
            anchors.topMargin: 45
            anchors.left: parent.left
            anchors.leftMargin: 39


        }

        Label {
            id: label
            x: 920
            y: 602
            color: "#a5a9ad"
            text: qsTr("Hit Space to pause")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 61
        }
    }
    }
}
