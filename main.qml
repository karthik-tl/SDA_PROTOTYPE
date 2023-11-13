import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 400
    height: 400

    Row {
        Text {
            y:20
            id: port
            text: "Ports"
        }
        ComboBox {

            y:10
            id: comboBox
            width: 150
            model: SerialAquisition.availablePorts()
            onCurrentTextChanged: {
                SerialAquisition.handleComboBoxSelection(currentText)
            }
        }
        Button{
            x:200
            y:10
            id: connect
            text: "Connect"
            onClicked: {
                SerialAquisition.connectPort()
            }
        }
    }

    Column {

        x:0
        y:100

        ListView {
            id: listView
            height: 300
            width: 400
            ScrollBar.vertical: ScrollBar {
                active: true
            }
            model: SerialAquisition.receivedData
            onCountChanged: {
                listView.currentIndex = count - 1
            }
            delegate: Item {
                width: ListView.view.width
                height: textItem.contentHeight

                Text {
                    id: textItem
                    text: modelData
                }
            }
        }


    }
}

