

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Timeline 1.0

Item {
    width: 1920
    height: 720

    Image {
        id: chedaoxian
        x: 761
        y: 328
        source: "images/line/line" + property0.toString() + ".png"
        property int property0: 0
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id: car
        x: 810
        y: 354
        width: 297
        height: 366
        source: "images/car.png"
        fillMode: Image.PreserveAspectFit
    }

    Timeline {
        id: timeline
        animations: [
            TimelineAnimation {
                id: timelineAnimation
                loops: -1
                duration: 2500
                running: true
                to: 2500
                from: 0
            }
        ]
        startFrame: 0
        enabled: true
        endFrame: 2500

        KeyframeGroup {
            target: chedaoxian
            property: "property0"
            Keyframe {
                frame: 0
                value: 0
            }

            Keyframe {
                frame: 2500
                value: 25
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.5}D{i:2}D{i:4}
}
##^##*/

