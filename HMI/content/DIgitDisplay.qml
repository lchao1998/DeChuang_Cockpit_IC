

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: digitDisplay
    width: 376
    height: 192
    property real number: 0

    Row {
        id: row
        x: 94
        width: 192
        height: 192
        spacing: -100

        Image {
            id: _2
            height: 192
            visible: false
            source: "images/number/0.png"
            fillMode: Image.PreserveAspectFit
        }

        Image {
            id: _1
            x: 0
            y: 0
            height: 192
            visible: false
            source: "images/number/0.png"
            fillMode: Image.PreserveAspectFit
        }

        Image {
            id: _0
            x: 0
            y: 0
            height: 192
            source: "images/number/0.png"
            fillMode: Image.PreserveAspectFit
        }
    }

    Connections {
        target: digitDisplay
        function onNumberChanged() {
            var str = number.toString()
            if (str.length == 3) {
                _2.source = "images/number/" + str[0] + ".png"
                _1.source = "images/number/" + str[1] + ".png"
                _0.source = "images/number/" + str[2] + ".png"
                digitDisplay.state = "three"
            } else if (str.length == 2) {
                _1.source = "images/number/" + str[0] + ".png"
                _0.source = "images/number/" + str[1] + ".png"
                digitDisplay.state = "two"
            } else if (str.length == 1) {
                _0.source = "images/number/" + str[0] + ".png"
                digitDisplay.state = "base state"
            }
        }
    }

    Component.onCompleted: {
        var str = number.toString()
        console.log("Qml: ----------------speed" + str)
        if (str.length == 3) {
            _2.source = "images/number/" + str[0] + ".png"
            _1.source = "images/number/" + str[1] + ".png"
            _0.source = "images/number/" + str[2] + ".png"
            digitDisplay.state = "three"
        } else if (str.length == 2) {
            _1.source = "images/number/" + str[0] + ".png"
            _0.source = "images/number/" + str[1] + ".png"
            digitDisplay.state = "two"
        } else if (str.length == 1) {
            _0.source = "images/number/" + str[0] + ".png"
            digitDisplay.state = "base state"
        }
    }

    states: [
        State {
            name: "two"

            PropertyChanges {
                target: _1
                visible: true
            }

            PropertyChanges {
                target: row
                x: 46
                width: 284
            }
        },
        State {
            name: "three"

            PropertyChanges {
                target: _2
                visible: true
            }

            PropertyChanges {
                target: _1
                visible: true
            }

            PropertyChanges {
                target: row
                x: 0
                width: 376
            }
        }
    ]
}

/*##^##
Designer {
    D{i:0;height:192;width:376}
}
##^##*/

