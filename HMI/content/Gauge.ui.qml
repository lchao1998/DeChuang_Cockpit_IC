
/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    width: 1920
    height: 720
    opacity: 1

    Item {
        id: left_gauge
        width: 1920
        height: 720

        Image {
            id: gauge1
            x: 97
            y: 0
            source: "images/gauge1.png"
            fillMode: Image.PreserveAspectFit
        }

        Image {
            id: gauge2
            x: 97
            y: 14
            source: "images/gauge2.png"
            fillMode: Image.PreserveAspectFit
        }

        ShaderEffects {
            id: shaderEffects
            x: 102
            y: 19
        }

        DIgitDisplay {
            id: dIgitDisplay
            x: 120
            y: 232
            number: gaugedatamodel.gauge_VCU_VehSpd
        }

        Text {
            id: km
            x: 472
            y: 297
            width: 136
            height: 55
            opacity: 0.85
            color: "#f2f6fc"
            text: qsTr("km/h")
            font.pixelSize: 64
            horizontalAlignment: Text.AlignHCenter
            font.weight: Font.Light
            font.family: "D-DIN-PRO"
        }
    }

    Item {
        id: right_gauge
        width: 1920
        height: 720

        Image {
            id: gauge1_right
            x: 1543
            y: 0
            source: "images/gauge1_right.png"
            fillMode: Image.PreserveAspectFit
        }

        Image {
            id: gauge2_right
            x: 1589
            y: 14
            source: "images/gauge2_right.png"
            fillMode: Image.PreserveAspectFit
        }

        ShaderEffectsRight {
            id: shaderEffectsRight
            x: 1572
            y: 19
        }

        DIgitDisplay {
            id: dIgitDisplay1
            x: 1368
            y: 230
            number: gaugedatamodel.gauge_VCU_VehSOC
        }

        Text {
            id: unit
            x: 1700
            y: 300
            width: 45
            height: 47
            opacity: 0.85
            color: "#f2f6fc"
            text: qsTr("%")
            font.pixelSize: 64
            horizontalAlignment: Text.AlignHCenter
            font.weight: Font.Light
            font.family: "D-DIN-PRO"
        }

        Text {
            id: soc
            x: 1331
            y: 315
            width: 91
            height: 32
            opacity: 0.85
            color: "#f2f6fc"
            text: qsTr("SOC")
            font.pixelSize: 64
            horizontalAlignment: Text.AlignHCenter
            font.weight: Font.Light
            font.family: "D-DIN-PRO"
        }
    }

    Image {
        id: rectangle
        x: 911
        y: 3
        source: "images/rectangle.png"
        fillMode: Image.PreserveAspectFit
    }

    Text {
        id: gear
        x: 934
        y: 8
        width: 55
        height: 37
        color: "#ffffff"
        text: gaugedatamodel.gauge_VCU_VehShiftLevel
        font.pixelSize: 51
        horizontalAlignment: Text.AlignHCenter
        font.weight: Font.Medium
        font.family: "D-DIN-PRO Medium"
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.5;height:0;width:0}
}
##^##*/

