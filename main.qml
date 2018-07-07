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


    SerialPort {
        id: backend
        property real x: 0

        onMessageReceived: {
            console.log(message)
            line.append(x,Number(message))
            x+=0.1
            axisX.max=x
            axisX.min=x-5

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
        }


        ValueAxis {
            id: axisY
            min: -5.0
            max: 5.0
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
            }

           text: backend.device
            anchors.top: parent.top
            anchors.topMargin: 45
            anchors.left: parent.left
            anchors.leftMargin: 39


        }
    }
}
