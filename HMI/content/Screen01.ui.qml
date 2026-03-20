

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick 2.15
import QtQuick.Controls 2.15

//import DechuangDemo 1.0
Rectangle {
    width: 1920
    height: 720
    color: "#000000"

    Image {
        id: bg
        x: 0
        y: 0
        source: "images/bg.png"
        fillMode: Image.PreserveAspectFit
    }

    Telltale {
        id: telltale
    }

    Adas {
        id: adas
    }

    Gauge {
        id: gauge
    }

    BaseInfo {
        id: baseInfo
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.5;height:1080;width:1920}
}
##^##*/

